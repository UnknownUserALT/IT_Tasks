// order_operations.cpp
#include "order_operations.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <cstdlib>

// ================================
// OrderStatusManager implementations
// ================================

OrderStatusManager::OrderStatusManager(std::shared_ptr<DatabaseConnection> database)
    : db(database) {
    validTransitions = {
        {"pending", {"completed", "canceled"}},
        {"completed", {"returned"}},
        {"canceled", {}},
        {"returned", {}}
    };
}

bool OrderStatusManager::canChangeStatus(const std::string& currentStatus, const std::string& newStatus) {
    auto it = validTransitions.find(currentStatus);
    if (it != validTransitions.end()) {
        const auto& allowedStatuses = it->second;
        return std::find(allowedStatuses.begin(), allowedStatuses.end(), newStatus)
               != allowedStatuses.end();
    }
    return false;
}

std::string OrderStatusManager::getOrderStatus(int orderId) {
    try {
        auto result = db->executeQuery(
            "SELECT status FROM orders WHERE order_id = $1", orderId);
        if (!result.empty()) {
            return result[0][0];
        }
        return "not_found";
    } catch (const std::exception& e) {
        std::cerr << "Ошибка получения статуса заказа: " << e.what() << std::endl;
        return "error";
    }
}

bool OrderStatusManager::changeOrderStatus(int orderId, const std::string& newStatus, int changedBy) {
    try {
        std::string currentStatus = getOrderStatus(orderId);
        if (currentStatus == "not_found" || currentStatus == "error") {
            return false;
        }

        if (!canChangeStatus(currentStatus, newStatus)) {
            std::cerr << "Недопустимый переход статуса: " << currentStatus
                      << " -> " << newStatus << std::endl;
            return false;
        }

        db->executeQuery("CALL update_order_status($1, $2, $3)",
                           orderId, newStatus, changedBy);

        std::cout << "Статус заказа " << orderId << " изменен: "
                  << currentStatus << " -> " << newStatus << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка изменения статуса заказа: " << e.what() << std::endl;
        return false;
    }
}

// ================================
// ReturnManager implementations
// ================================

ReturnManager::ReturnManager(std::shared_ptr<DatabaseConnection> database)
    : db(database) {}

ReturnManager::ReturnCheckResult ReturnManager::checkProductReturn(int orderId, int productId, int quantity) {
    ReturnCheckResult result = {false, "", 0, 0.0};

    try {
        auto orderCheck = db->executeQuery(
            "SELECT status, order_date, total_price FROM orders WHERE order_id = $1",
            orderId);

        if (orderCheck.empty()) {
            result.reason = "Заказ не найден";
            return result;
        }

        std::string status = orderCheck[0][0];

        if (status != "completed") {
            result.reason = "Возврат возможен только для завершенных заказов";
            return result;
        }

        auto daysResult = db->executeQuery(
            "SELECT EXTRACT(DAY FROM (CURRENT_TIMESTAMP - order_date)) "
            "FROM orders WHERE order_id = $1", orderId);

        if (!daysResult.empty()) {
            int daysPassed = std::stoi(daysResult[0][0]);
            result.daysPassed = daysPassed;

            if (daysPassed > 30) {
                result.reason = "Прошло более 30 дней с момента покупки";
                return result;
            }
        }

        auto productInOrder = db->executeQuery(
            "SELECT quantity, price FROM order_items "
            "WHERE order_id = $1 AND product_id = $2",
            orderId, productId);

        if (productInOrder.empty()) {
            result.reason = "Товар не найден в заказе";
            return result;
        }

        int availableQuantity = std::stoi(productInOrder[0][0]);
        double price = std::stod(productInOrder[0][1]);

        if (quantity > availableQuantity) {
            result.reason = "Запрошенное количество превышает купленное";
            return result;
        }

        auto alreadyReturned = db->executeQuery(
            "SELECT SUM(quantity) FROM return_log "
            "WHERE order_id = $1 AND product_id = $2",
            orderId, productId);

        if (!alreadyReturned.empty() && !alreadyReturned[0][0].empty()) {
            int returnedQty = std::stoi(alreadyReturned[0][0]);
            if (returnedQty + quantity > availableQuantity) {
                result.reason = "Нельзя вернуть больше товара, чем было куплено";
                return result;
            }
        }

        result.canReturn = true;
        result.reason = "Возврат возможен";
        result.refundAmount = price * quantity;

        return result;

    } catch (const std::exception& e) {
        result.reason = std::string("Ошибка проверки: ") + e.what();
        return result;
    }
}

bool ReturnManager::returnProduct(int orderId, int productId, int quantity,
                                  const std::string& reason, int userId) {
    try {
        auto checkResult = checkProductReturn(orderId, productId, quantity);

        if (!checkResult.canReturn) {
            std::cerr << "Невозможно вернуть товар: " << checkResult.reason << std::endl;
            return false;
        }

        db->beginTransaction();

        try {
            db->getCurrentTransaction().exec(
                "UPDATE products SET stock_quantity = stock_quantity + $1 "
                "WHERE product_id = $2",
                pqxx::params(quantity, productId));

            db->getCurrentTransaction().exec(
                "INSERT INTO return_log (order_id, product_id, quantity, "
                "return_date, reason, processed_by, refund_amount) "
                "VALUES ($1, $2, $3, CURRENT_TIMESTAMP, $4, $5, $6)",
                pqxx::params(orderId, productId, quantity, reason, userId, checkResult.refundAmount));

            int refundPaymentId = std::rand() % 1000000;
            Payment refundPayment(refundPaymentId, orderId, checkResult.refundAmount, "refund");

            std::cout << "Обработка возврата средств..." << std::endl;
            bool refundSuccess = refundPayment.refund();

            if (!refundSuccess) {
                throw std::runtime_error("Не удалось обработать возврат средств");
            }

            auto remainingItems = db->getCurrentTransaction().exec(
                "SELECT oi.product_id, oi.quantity, "
                "COALESCE(SUM(rl.quantity), 0) as returned_qty "
                "FROM order_items oi "
                "LEFT JOIN return_log rl ON oi.order_id = rl.order_id "
                "AND oi.product_id = rl.product_id "
                "WHERE oi.order_id = $1 "
                "GROUP BY oi.product_id, oi.quantity",
                pqxx::params(orderId));

            bool allItemsReturned = true;
            for (const auto& row : remainingItems) {
                int totalQty = row[1].as<int>();
                int returnedQty = row[2].as<int>();

                if (totalQty > returnedQty) {
                    allItemsReturned = false;
                    break;
                }
            }

            if (allItemsReturned) {
                db->getCurrentTransaction().exec(
                    "UPDATE orders SET status = 'returned' WHERE order_id = $1",
                    pqxx::params(orderId));

                db->getCurrentTransaction().exec(
                    "INSERT INTO order_status_history (order_id, old_status, "
                    "new_status, changed_by, reason) "
                    "VALUES ($1, 'completed', 'returned', $2, 'Все товары возвращены')",
                    pqxx::params(orderId, userId));
            }

            db->getCurrentTransaction().exec(
                "INSERT INTO audit_log (entity_type, entity_id, operation, "
                "performed_by, performed_at) "
                "VALUES ('order', $1, 'update', $2, CURRENT_TIMESTAMP)",
                pqxx::params(orderId, userId));

            db->commitTransaction();

            std::cout << "Товар успешно возвращен:" << std::endl;
            std::cout << "  Заказ: #" << orderId << std::endl;
            std::cout << "  Товар: #" << productId << std::endl;
            std::cout << "  Количество: " << quantity << std::endl;
            std::cout << "  Сумма возврата: $" << checkResult.refundAmount << std::endl;
            std::cout << "  Причина: " << reason << std::endl;
            std::cout << "  Статус возврата средств: " << refundPayment.getStatus() << std::endl;

            return true;

        } catch (const std::exception& e) {
            db->rollbackTransaction();
            throw;
        }

    } catch (const std::exception& e) {
        std::cerr << "Ошибка возврата товара: " << e.what() << std::endl;
        return false;
    }
}

// ================================
// CreditCardPaymentStrategy implementations
// ================================

CreditCardPaymentStrategy::CreditCardPaymentStrategy(const std::string& cardNum,
                                                     const std::string& expiry,
                                                     const std::string& cvvCode,
                                                     const std::string& holderName)
    : cardNumber(cardNum), expiryDate(expiry), cvv(cvvCode), cardHolderName(holderName) {}

bool CreditCardPaymentStrategy::validateCardDetails() const {
    if (cardNumber.length() != 16) return false;
    if (expiryDate.length() != 5) return false;
    if (cvv.length() != 3 && cvv.length() != 4) return false;
    return true;
}

std::unique_ptr<Payment> CreditCardPaymentStrategy::createPayment(int paymentId, int orderId, double amount) const {
    return std::make_unique<Payment>(paymentId, orderId, amount, "credit_card");
}

bool CreditCardPaymentStrategy::processPayment(int paymentId, int orderId, double amount) {
    std::cout << "\nОбработка оплаты банковской картой:" << std::endl;
    std::cout << "  Сумма: $" << amount << std::endl;
    std::cout << "  Заказ: #" << orderId << std::endl;
    std::cout << "  Карта: **** **** **** "
              << (cardNumber.length() > 4 ? cardNumber.substr(cardNumber.length() - 4) : "****")
              << std::endl;

    if (!validateCardDetails()) {
        std::cerr << "  Ошибка: неверные данные карты" << std::endl;
        return false;
    }

    auto payment = createPayment(paymentId, orderId, amount);

    std::cout << "  Проверка карты..." << std::endl;
    std::cout << "  Авторизация платежа..." << std::endl;

    bool success = payment->process();

    if (success) {
        std::cout << "  Платеж успешен! ID транзакции: " << paymentId << std::endl;
        std::cout << "  Статус платежа: " << payment->getStatus() << std::endl;
    }

    return success;
}

std::string CreditCardPaymentStrategy::getMethodName() const {
    return "Банковская карта";
}

bool CreditCardPaymentStrategy::isAvailable() const {
    return validateCardDetails();
}

std::map<std::string, std::string> CreditCardPaymentStrategy::getPaymentDetails() const {
    return {
        {"card_number", cardNumber},
        {"expiry_date", expiryDate},
        {"cvv", cvv},
        {"card_holder", cardHolderName},
        {"type", "credit_card"}
    };
}

// ================================
// EWalletPaymentStrategy implementations
// ================================

EWalletPaymentStrategy::EWalletPaymentStrategy(const std::string& id, const std::string& type)
    : walletId(id), walletType(type) {}

bool EWalletPaymentStrategy::validateWalletDetails() const {
    std::vector<std::string> supportedTypes = {"yoomoney", "qiwi", "webmoney", "paypal"};
    bool typeValid = std::find(supportedTypes.begin(), supportedTypes.end(), walletType)
                     != supportedTypes.end();
    return typeValid && !walletId.empty() && walletId.length() >= 10;
}

std::unique_ptr<Payment> EWalletPaymentStrategy::createPayment(int paymentId, int orderId, double amount) const {
    return std::make_unique<Payment>(paymentId, orderId, amount, "ewallet");
}

bool EWalletPaymentStrategy::processPayment(int paymentId, int orderId, double amount) {
    std::cout << "\nОбработка оплаты электронным кошельком:" << std::endl;
    std::cout << "  Сумма: $" << amount << std::endl;
    std::cout << "  Заказ: #" << orderId << std::endl;
    std::cout << "  Кошелек: " << walletType << " ("
              << (walletId.length() > 8 ?
                  (walletId.substr(0, 4) + "..." + walletId.substr(walletId.length() - 4)) :
                  walletId)
              << ")" << std::endl;

    if (!validateWalletDetails()) {
        std::cerr << "  Ошибка: неверные данные кошелька" << std::endl;
        return false;
    }

    auto payment = createPayment(paymentId, orderId, amount);

    std::cout << "  Подключение к платежной системе " << walletType << "..." << std::endl;
    std::cout << "  Проверка баланса..." << std::endl;

    bool success = payment->process();

    if (success) {
        std::cout << "  Платеж успешен! ID транзакции: " << paymentId << std::endl;
        std::cout << "  Статус платежа: " << payment->getStatus() << std::endl;
    }

    return success;
}

std::string EWalletPaymentStrategy::getMethodName() const {
    return "Электронный кошелек (" + walletType + ")";
}

bool EWalletPaymentStrategy::isAvailable() const {
    return validateWalletDetails();
}

std::map<std::string, std::string> EWalletPaymentStrategy::getPaymentDetails() const {
    return {
        {"wallet_id", walletId},
        {"wallet_type", walletType},
        {"type", "ewallet"}
    };
}

// ================================
// SBPPaymentStrategy implementations
// ================================

SBPPaymentStrategy::SBPPaymentStrategy(const std::string& phone, const std::string& bank)
    : phoneNumber(phone), bankName(bank) {}

bool SBPPaymentStrategy::validateDetails() const {
    if (phoneNumber.length() != 11 || phoneNumber[0] != '7') return false;

    std::string bankLower = bankName;
    std::transform(bankLower.begin(), bankLower.end(), bankLower.begin(), ::tolower);
    std::vector<std::string> supportedBanks = {"сбербанк", "sberbank", "тинькофф",
                                              "tinkoff", "альфа", "alfa", "втб", "vtb"};

    return std::find(supportedBanks.begin(), supportedBanks.end(), bankLower)
           != supportedBanks.end();
}

std::unique_ptr<Payment> SBPPaymentStrategy::createPayment(int paymentId, int orderId, double amount) const {
    return std::make_unique<Payment>(paymentId, orderId, amount, "sbp");
}

bool SBPPaymentStrategy::processPayment(int paymentId, int orderId, double amount) {
    std::cout << "\nОбработка оплаты через СБП:" << std::endl;
    std::cout << "  Сумма: $" << amount << std::endl;
    std::cout << "  Заказ: #" << orderId << std::endl;
    std::cout << "  Телефон: " << phoneNumber << std::endl;
    std::cout << "  Банк: " << bankName << std::endl;

    if (!validateDetails()) {
        std::cerr << "  Ошибка: неверные данные для СБП" << std::endl;
        return false;
    }

    auto payment = createPayment(paymentId, orderId, amount);

    std::cout << "  Отправка запроса в банк " << bankName << "..." << std::endl;
    std::cout << "  Ожидание подтверждения..." << std::endl;

    bool success = payment->process();

    if (success) {
        std::cout << "  Платеж успешен! ID транзакции: " << paymentId << std::endl;
        std::cout << "  Статус платежа: " << payment->getStatus() << std::endl;
    }

    return success;
}

std::string SBPPaymentStrategy::getMethodName() const {
    return "СБП (" + bankName + ")";
}

bool SBPPaymentStrategy::isAvailable() const {
    return validateDetails();
}

std::map<std::string, std::string> SBPPaymentStrategy::getPaymentDetails() const {
    return {
        {"phone", phoneNumber},
        {"bank", bankName},
        {"type", "sbp"}
    };
}

// ================================
// PaymentContext implementations
// ================================

PaymentContext::PaymentContext(std::shared_ptr<DatabaseConnection> database)
    : db(database) {}

void PaymentContext::setPaymentStrategy(std::unique_ptr<PaymentStrategy> strategy) {
    paymentStrategy = std::move(strategy);
}

int PaymentContext::generatePaymentId() {
    return 1000000 + (std::rand() % 9000000);
}

void PaymentContext::savePaymentToDatabase(int paymentId, int orderId, double amount,
                                           const std::string& method,
                                           const std::map<std::string, std::string>& details) {
    try {
        db->executeNonQuery(R"(
            CREATE TABLE IF NOT EXISTS payments (
                payment_id INTEGER PRIMARY KEY,
                order_id INTEGER NOT NULL,
                amount DECIMAL(10,2) NOT NULL,
                method VARCHAR(50) NOT NULL,
                status VARCHAR(20) DEFAULT 'completed',
                payment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                details JSONB,
                CONSTRAINT fk_payment_order
                    FOREIGN KEY (order_id)
                    REFERENCES orders(order_id)
                    ON DELETE CASCADE
            )
        )");

        std::string detailsJson = "{";
        for (auto it = details.begin(); it != details.end(); ++it) {
            detailsJson += "\"" + it->first + "\": \"" + it->second + "\"";
            if (std::next(it) != details.end()) detailsJson += ", ";
        }
        detailsJson += "}";

        db->executeQuery(
            "INSERT INTO payments (payment_id, order_id, amount, method, details) "
            "VALUES ($1, $2, $3, $4, $5::JSONB)",
            paymentId, orderId, amount, method, detailsJson);

        std::cout << "  Информация о платеже сохранена в базе данных" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения платежа: " << e.what() << std::endl;
    }
}

bool PaymentContext::processPayment(int orderId, double amount) {
    if (!paymentStrategy) {
        std::cerr << "Стратегия оплаты не установлена" << std::endl;
        return false;
    }

    if (!paymentStrategy->isAvailable()) {
        std::cerr << "Метод оплаты недоступен" << std::endl;
        return false;
    }

    int paymentId = generatePaymentId();

    std::cout << "\n=== ОПЛАТА ЗАКАЗА ===" << std::endl;
    std::cout << "Метод: " << paymentStrategy->getMethodName() << std::endl;
    std::cout << "Заказ: #" << orderId << std::endl;
    std::cout << "Сумма: $" << amount << std::endl;

    bool success = paymentStrategy->processPayment(paymentId, orderId, amount);

    if (success) {
        savePaymentToDatabase(paymentId, orderId, amount,
                             paymentStrategy->getMethodName(),
                             paymentStrategy->getPaymentDetails());
    }

    return success;
}

std::string PaymentContext::getCurrentMethodName() const {
    if (paymentStrategy) {
        return paymentStrategy->getMethodName();
    }
    return "Не установлен";
}

// ================================
// OrderFacade implementations
// ================================

OrderFacade::OrderFacade(std::shared_ptr<DatabaseConnection> database)
    : db(database) {
    statusManager = std::make_shared<OrderStatusManager>(db);
    returnManager = std::make_shared<ReturnManager>(db);
    paymentContext = std::make_unique<PaymentContext>(db);
}

int OrderFacade::createOrder(int userId, const std::vector<std::pair<int, int>>& items) {
    try {
        std::string itemsJson = "[";
        for (size_t i = 0; i < items.size(); ++i) {
            itemsJson += R"({"product_id": )" + std::to_string(items[i].first) +
                        R"(, "quantity": )" + std::to_string(items[i].second) + "}";
            if (i < items.size() - 1) itemsJson += ", ";
        }
        itemsJson += "]";

        auto result = db->executeQuery(
            "SELECT create_order($1, $2::JSONB, 'pending')",
            userId, itemsJson);

        if (!result.empty()) {
            int orderId = std::stoi(result[0][0]);
            std::cout << "Заказ создан! ID: " << orderId << std::endl;
            return orderId;
        }

        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка создания заказа: " << e.what() << std::endl;
        return -1;
    }
}

bool OrderFacade::processOrderPayment(int orderId, const std::string& paymentMethod,
                                      const std::map<std::string, std::string>& paymentDetails) {
    try {
        auto orderResult = db->executeQuery(
            "SELECT total_price FROM orders WHERE order_id = $1", orderId);

        if (orderResult.empty()) {
            std::cerr << "Заказ не найден" << std::endl;
            return false;
        }

        double amount = std::stod(orderResult[0][0]);

        if (paymentMethod == "credit_card") {
            auto itCard = paymentDetails.find("card_number");
            auto itExpiry = paymentDetails.find("expiry_date");
            auto itCVV = paymentDetails.find("cvv");
            auto itHolder = paymentDetails.find("card_holder");

            if (itCard != paymentDetails.end() && itExpiry != paymentDetails.end()
                && itCVV != paymentDetails.end()) {
                paymentContext->setPaymentStrategy(
                    std::make_unique<CreditCardPaymentStrategy>(
                        itCard->second, itExpiry->second, itCVV->second,
                        itHolder != paymentDetails.end() ? itHolder->second : ""));
            }
        }
        else if (paymentMethod == "ewallet") {
            auto itWallet = paymentDetails.find("wallet_id");
            auto itType = paymentDetails.find("wallet_type");

            if (itWallet != paymentDetails.end() && itType != paymentDetails.end()) {
                paymentContext->setPaymentStrategy(
                    std::make_unique<EWalletPaymentStrategy>(itWallet->second, itType->second));
            }
        }
        else if (paymentMethod == "sbp") {
            auto itPhone = paymentDetails.find("phone");
            auto itBank = paymentDetails.find("bank");

            if (itPhone != paymentDetails.end() && itBank != paymentDetails.end()) {
                paymentContext->setPaymentStrategy(
                    std::make_unique<SBPPaymentStrategy>(itPhone->second, itBank->second));
            }
        }
        else {
            std::cerr << "Неизвестный метод оплаты: " << paymentMethod << std::endl;
            return false;
        }

        bool paymentSuccess = paymentContext->processPayment(orderId, amount);

        if (paymentSuccess) {
            return statusManager->changeOrderStatus(orderId, "completed", 0);
        }

        return false;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка обработки оплаты: " << e.what() << std::endl;
        return false;
    }
}

bool OrderFacade::returnOrderProduct(int orderId, int productId, int quantity,
                                     const std::string& reason, int userId) {
    return returnManager->returnProduct(orderId, productId, quantity, reason, userId);
}

void OrderFacade::demonstrateOrderSystem() {
    std::cout << "\n\n=== ДЕМОНСТРАЦИЯ СИСТЕМЫ ЗАКАЗОВ ===" << std::endl;

    try {
        db->executeNonQuery(R"(
            CREATE TABLE IF NOT EXISTS return_log (
                return_id SERIAL PRIMARY KEY,
                order_id INTEGER NOT NULL,
                product_id INTEGER NOT NULL,
                quantity INTEGER CHECK (quantity > 0) NOT NULL,
                return_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                reason TEXT,
                processed_by INTEGER,
                refund_amount DECIMAL(10,2) CHECK (refund_amount >= 0),
                CONSTRAINT fk_return_order
                    FOREIGN KEY (order_id)
                    REFERENCES orders(order_id)
                    ON DELETE CASCADE,
                CONSTRAINT fk_return_product
                    FOREIGN KEY (product_id)
                    REFERENCES products(product_id)
                    ON DELETE RESTRICT,
                CONSTRAINT fk_return_processed_by
                    FOREIGN KEY (processed_by)
                    REFERENCES users(user_id)
                    ON DELETE SET NULL
            )
        )");
        std::cout << "Таблица return_log создана/проверена" << std::endl;
    } catch (...) {
    }

    std::cout << "\n1. Демонстрация стратегий оплаты:" << std::endl;

    int testOrderId = createOrder(1, {{1, 2}});
    if (testOrderId != -1) {
        std::vector<std::tuple<std::string, std::string, std::map<std::string, std::string>>> testPayments = {
            {"credit_card", "Оплата банковской картой", {
                {"card_number", "1234567812345678"},
                {"expiry_date", "12/25"},
                {"cvv", "123"},
                {"card_holder", "IVAN IVANOV"}
            }},
            {"ewallet", "Оплата электронным кошельком", {
                {"wallet_id", "4100112345678901"},
                {"wallet_type", "yoomoney"}
            }},
            {"sbp", "Оплата через СБП", {
                {"phone", "79123456789"},
                {"bank", "sberbank"}
            }}
        };

        std::for_each(testPayments.begin(), testPayments.end(),
            [this, testOrderId](const auto& paymentTest) {
                auto [method, description, details] = paymentTest;
                std::cout << "\n" << description << ":" << std::endl;

                auto orderResult = db->executeQuery(
                    "SELECT total_price FROM orders WHERE order_id = $1", testOrderId);

                if (!orderResult.empty()) {
                    processOrderPayment(testOrderId, method, details);
                }
            }
        );
    }

    std::cout << "\n\n2. Демонстрация возврата товаров:" << std::endl;

    int returnOrderId = createOrder(1, {{1, 3}});
    if (returnOrderId != -1) {
        std::map<std::string, std::string> paymentDetails = {
            {"card_number", "1234567812345678"},
            {"expiry_date", "12/25"},
            {"cvv", "123"}
        };

        if (processOrderPayment(returnOrderId, "credit_card", paymentDetails)) {
            std::cout << "\nПопытка возврата товара:" << std::endl;
            returnOrderProduct(returnOrderId, 1, 2,
                              "Товар не соответствует описанию", 1);
        }
    }

    std::cout << "\n=== ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА ===" << std::endl;
}
