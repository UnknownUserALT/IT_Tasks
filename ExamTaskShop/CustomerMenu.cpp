// CustomerMenu.cpp
// Интерактивное меню покупателя
#include "DatabaseConnection.hpp"
#include "Customer.cpp"
#include "PaymentStrategy.hpp"
#include <iostream>
#include <memory>
#include <limits>
#include <iomanip>

class CustomerMenu {
private:
    std::shared_ptr<DatabaseConnection> db;
    std::unique_ptr<Customer> customer;
    PaymentContext paymentContext;

    // Очистка буфера ввода
    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Пауза для чтения сообщений
    void pause() {
        std::cout << "\nНажмите Enter для продолжения...";
        clearInputBuffer();
        std::cin.get();
    }

    // Отображение заголовка меню
    void displayHeader() {
        std::cout << "\n╔════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║       ИНТЕРНЕТ-МАГАЗИН - МЕНЮ ПОКУПАТЕЛЯ       ║" << std::endl;
        std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
        std::cout << "Пользователь: " << customer->getName()
                  << " (ID: " << customer->getUserId() << ")" << std::endl;
        if (customer->getLoyaltyLevel() == 1) {
            std::cout << "★ PREMIUM статус - скидка 10% на все заказы ★" << std::endl;
        }
        std::cout << std::endl;
    }

    // Отображение меню
    void displayMenu() {
        std::cout << "┌────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│  1. Создать новый заказ                        │" << std::endl;
        std::cout << "│  2. Добавить товар в заказ                     │" << std::endl;
        std::cout << "│  3. Удалить товар из заказа                    │" << std::endl;
        std::cout << "│  4. Просмотр моих заказов                      │" << std::endl;
        std::cout << "│  5. Просмотр статуса заказа                    │" << std::endl;
        std::cout << "│  6. Оплатить заказ                             │" << std::endl;
        std::cout << "│  7. Оформить возврат заказа                    │" << std::endl;
        std::cout << "│  8. Просмотр истории статусов заказа           │" << std::endl;
        std::cout << "│  9. Выход                                      │" << std::endl;
        std::cout << "└────────────────────────────────────────────────┘" << std::endl;
        std::cout << "\nВыберите действие (1-9): ";
    }

public:
    CustomerMenu(std::shared_ptr<DatabaseConnection> database, int userId)
        : db(database) {
        // Получаем информацию о покупателе из базы данных
        auto userInfo = db->executeQuery(
            "SELECT name, email, loyalty_level FROM users WHERE user_id = $1 AND role = 'customer'",
            userId);

        if (userInfo.empty()) {
            throw std::runtime_error("Покупатель не найден или доступ запрещен");
        }

        std::string name = userInfo[0][0];
        std::string email = userInfo[0][1];
        int loyaltyLevel = std::stoi(userInfo[0][2]);

        customer = std::make_unique<Customer>(userId, name, email, db, loyaltyLevel);
    }

    // Основной цикл меню
    void run() {
        int choice = 0;
        bool running = true;

        while (running) {
            system("clear || cls"); // Очистка экрана
            displayHeader();
            displayMenu();

            std::cin >> choice;

            if (std::cin.fail()) {
                clearInputBuffer();
                std::cout << "\n❌ Ошибка: Введите число от 1 до 9" << std::endl;
                pause();
                continue;
            }

            clearInputBuffer();

            switch (choice) {
                case 1:
                    createNewOrder();
                    break;
                case 2:
                    addProductToOrder();
                    break;
                case 3:
                    removeProductFromOrder();
                    break;
                case 4:
                    viewMyOrders();
                    break;
                case 5:
                    viewOrderStatus();
                    break;
                case 6:
                    payForOrder();
                    break;
                case 7:
                    processReturn();
                    break;
                case 8:
                    viewOrderHistory();
                    break;
                case 9:
                    std::cout << "\n👋 До свидания!" << std::endl;
                    running = false;
                    break;
                default:
                    std::cout << "\n❌ Ошибка: Неверный выбор. Введите число от 1 до 9" << std::endl;
                    pause();
            }
        }
    }

private:
    // 1. Создать новый заказ
    void createNewOrder() {
        std::cout << "\n═══ СОЗДАНИЕ НОВОГО ЗАКАЗА ═══" << std::endl;

        auto order = customer->createOrder();
        if (order) {
            std::cout << "✓ Новый заказ (корзина) создан успешно!" << std::endl;
            std::cout << "Теперь вы можете добавлять товары через меню." << std::endl;
        } else {
            std::cout << "❌ Не удалось создать заказ." << std::endl;
        }

        pause();
    }

    // 2. Добавить товар в заказ
    void addProductToOrder() {
        std::cout << "\n═══ ДОБАВЛЕНИЕ ТОВАРА В ЗАКАЗ ═══" << std::endl;

        // Показываем доступные товары
        try {
            auto products = db->executeQuery(
                "SELECT product_id, name, price, stock_quantity FROM products "
                "WHERE stock_quantity > 0 ORDER BY name");

            if (products.empty()) {
                std::cout << "❌ Нет доступных товаров" << std::endl;
                pause();
                return;
            }

            std::cout << "\nДоступные товары:" << std::endl;
            std::cout << "┌────┬──────────────────────┬──────────┬──────────┐" << std::endl;
            std::cout << "│ ID │ Название             │ Цена     │ Остаток  │" << std::endl;
            std::cout << "├────┼──────────────────────┼──────────┼──────────┤" << std::endl;

            for (const auto& product : products) {
                std::cout << "│ " << std::setw(2) << product[0]
                         << " │ " << std::setw(20) << std::left << product[1]
                         << " │ $" << std::setw(7) << std::right << product[2]
                         << " │ " << std::setw(8) << product[3] << " │" << std::endl;
            }
            std::cout << "└────┴──────────────────────┴──────────┴──────────┘" << std::endl;

            int productId;
            int quantity;

            std::cout << "\nВведите ID товара: ";
            std::cin >> productId;

            std::cout << "Введите количество: ";
            std::cin >> quantity;

            if (std::cin.fail() || quantity <= 0) {
                clearInputBuffer();
                std::cout << "❌ Некорректное количество" << std::endl;
                pause();
                return;
            }

            if (customer->addToOrder(productId, quantity)) {
                std::cout << "✓ Товар добавлен в корзину!" << std::endl;
            } else {
                std::cout << "❌ Не удалось добавить товар" << std::endl;
            }

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    // 3. Удалить товар из заказа
    void removeProductFromOrder() {
        std::cout << "\n═══ УДАЛЕНИЕ ТОВАРА ИЗ ЗАКАЗА ═══" << std::endl;

        int productId;
        std::cout << "Введите ID товара для удаления: ";
        std::cin >> productId;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный ID" << std::endl;
            pause();
            return;
        }

        if (customer->removeFromOrder(productId)) {
            std::cout << "✓ Товар удален из корзины!" << std::endl;
        } else {
            std::cout << "❌ Не удалось удалить товар" << std::endl;
        }

        pause();
    }

    // 4. Просмотр моих заказов
    void viewMyOrders() {
        std::cout << "\n═══ МОИ ЗАКАЗЫ ═══" << std::endl;

        try {
            auto orders = db->executeQuery(
                "SELECT o.order_id, o.status, o.total_price, o.order_date, "
                "COUNT(oi.order_item_id) as items_count "
                "FROM orders o "
                "LEFT JOIN order_items oi ON o.order_id = oi.order_id "
                "WHERE o.user_id = $1 "
                "GROUP BY o.order_id "
                "ORDER BY o.order_date DESC",
                customer->getUserId());

            if (orders.empty()) {
                std::cout << "У вас пока нет заказов." << std::endl;
                pause();
                return;
            }

            std::cout << "\n┌─────────┬────────────┬───────────┬─────────────────────┬─────────┐" << std::endl;
            std::cout << "│ Заказ # │ Статус     │ Сумма     │ Дата                │ Позиций │" << std::endl;
            std::cout << "├─────────┼────────────┼───────────┼─────────────────────┼─────────┤" << std::endl;

            for (const auto& order : orders) {
                std::cout << "│ " << std::setw(7) << std::left << order[0]
                         << " │ " << std::setw(10) << order[1]
                         << " │ $" << std::setw(8) << std::right << order[2]
                         << " │ " << std::setw(19) << std::left << order[3].substr(0, 19)
                         << " │ " << std::setw(7) << std::right << order[4] << " │" << std::endl;
            }
            std::cout << "└─────────┴────────────┴───────────┴─────────────────────┴─────────┘" << std::endl;

            std::cout << "\nВсего заказов: " << orders.size() << std::endl;

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    // 5. Просмотр статуса заказа
    void viewOrderStatus() {
        std::cout << "\n═══ ПРОСМОТР СТАТУСА ЗАКАЗА ═══" << std::endl;

        int orderId;
        std::cout << "Введите номер заказа: ";
        std::cin >> orderId;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный номер заказа" << std::endl;
            pause();
            return;
        }

        std::string status = customer->viewOrderStatus(orderId);
        std::cout << "\nСтатус заказа #" << orderId << ": " << status << std::endl;

        // Дополнительная информация о заказе
        try {
            auto orderInfo = db->executeQuery(
                "SELECT o.total_price, o.order_date, "
                "COUNT(oi.order_item_id) as items_count "
                "FROM orders o "
                "LEFT JOIN order_items oi ON o.order_id = oi.order_id "
                "WHERE o.order_id = $1 AND o.user_id = $2 "
                "GROUP BY o.order_id",
                orderId, customer->getUserId());

            if (!orderInfo.empty()) {
                std::cout << "Сумма: $" << orderInfo[0][0] << std::endl;
                std::cout << "Дата: " << orderInfo[0][1] << std::endl;
                std::cout << "Количество позиций: " << orderInfo[0][2] << std::endl;
            }
        } catch (...) {
            // Игнорируем ошибки дополнительной информации
        }

        pause();
    }

    // 6. Оплатить заказ
    void payForOrder() {
        std::cout << "\n═══ ОПЛАТА ЗАКАЗА ═══" << std::endl;

        std::cout << "\nВыберите способ оплаты:" << std::endl;
        std::cout << "1. Банковская карта" << std::endl;
        std::cout << "2. Электронный кошелек" << std::endl;
        std::cout << "3. СБП (Система Быстрых Платежей)" << std::endl;
        std::cout << "\nВаш выбор: ";

        int paymentChoice;
        std::cin >> paymentChoice;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный выбор" << std::endl;
            pause();
            return;
        }

        clearInputBuffer();

        // Создание стратегии оплаты
        std::unique_ptr<PaymentStrategy> strategy;

        switch (paymentChoice) {
            case 1:
                strategy = std::make_unique<CreditCardPayment>();
                break;
            case 2:
                strategy = std::make_unique<EWalletPayment>();
                break;
            case 3:
                strategy = std::make_unique<SBPPayment>();
                break;
            default:
                std::cout << "❌ Некорректный выбор способа оплаты" << std::endl;
                pause();
                return;
        }

        paymentContext.setPaymentStrategy(std::move(strategy));

        // Получаем название метода оплаты
        std::string paymentMethod = paymentContext.getCurrentPaymentMethod();

        // Вызываем метод оплаты из Customer с использованием стратегии
        if (customer->makePayment(paymentMethod)) {
            std::cout << "\n✓ Заказ успешно оплачен!" << std::endl;

            // Записываем в аудит
            try {
                db->executeNonQuery("SELECT set_config('app.user_id', '" +
                    std::to_string(customer->getUserId()) + "', FALSE)");
            } catch (...) {}

        } else {
            std::cout << "\n❌ Оплата не прошла" << std::endl;
        }

        pause();
    }

    // 7. Оформить возврат заказа
    void processReturn() {
        std::cout << "\n═══ ОФОРМЛЕНИЕ ВОЗВРАТА ЗАКАЗА ═══" << std::endl;

        int orderId;
        std::cout << "Введите номер заказа для возврата: ";
        std::cin >> orderId;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный номер заказа" << std::endl;
            pause();
            return;
        }

        clearInputBuffer();

        // Проверка возможности возврата
        try {
            auto canReturn = db->executeQuery(
                "SELECT * FROM can_return_order($1)", orderId);

            if (canReturn.empty()) {
                std::cout << "❌ Не удалось проверить возможность возврата" << std::endl;
                pause();
                return;
            }

            bool canReturnFlag = (canReturn[0][0] == "t");
            std::string reason = canReturn[0][1];

            std::cout << "\nПроверка возможности возврата..." << std::endl;
            std::cout << "Заказ #" << orderId << std::endl;
            std::cout << "Статус: " << canReturn[0][2] << std::endl;
            std::cout << "Дата заказа: " << canReturn[0][3] << std::endl;
            std::cout << "Прошло дней: " << canReturn[0][4] << std::endl;
            std::cout << "\nРезультат: " << reason << std::endl;

            if (!canReturnFlag) {
                std::cout << "\n❌ Возврат невозможен" << std::endl;
                pause();
                return;
            }

            // Подтверждение возврата
            std::cout << "\n⚠ Вы уверены, что хотите оформить возврат? (y/n): ";
            char confirm;
            std::cin >> confirm;

            if (confirm != 'y' && confirm != 'Y') {
                std::cout << "Возврат отменен" << std::endl;
                pause();
                return;
            }

            clearInputBuffer();

            std::cout << "Введите причину возврата: ";
            std::string returnReason;
            std::getline(std::cin, returnReason);

            // Вызов процедуры возврата
            db->executeQuery(
                "CALL process_order_return($1, $2, $3)",
                orderId, returnReason, customer->getUserId());

            std::cout << "\n✓ Возврат оформлен успешно!" << std::endl;
            std::cout << "Средства будут возвращены в течение 7-10 рабочих дней." << std::endl;

            // Запись в аудит
            try {
                db->executeNonQuery("SELECT set_config('app.user_id', '" +
                    std::to_string(customer->getUserId()) + "', FALSE)");
            } catch (...) {}

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    // 8. Просмотр истории статусов заказа
    void viewOrderHistory() {
        std::cout << "\n═══ ИСТОРИЯ СТАТУСОВ ЗАКАЗА ═══" << std::endl;

        int orderId;
        std::cout << "Введите номер заказа: ";
        std::cin >> orderId;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный номер заказа" << std::endl;
            pause();
            return;
        }

        try {
            // Проверка, что заказ принадлежит пользователю
            auto orderCheck = db->executeQuery(
                "SELECT EXISTS(SELECT 1 FROM orders WHERE order_id = $1 AND user_id = $2)",
                orderId, customer->getUserId());

            if (orderCheck.empty() || orderCheck[0][0] != "t") {
                std::cout << "❌ Заказ не найден или доступ запрещен" << std::endl;
                pause();
                return;
            }

            // Получение истории
            auto history = db->executeQuery(
                "SELECT * FROM get_order_status_history($1)", orderId);

            if (history.empty()) {
                std::cout << "История для заказа #" << orderId << " не найдена" << std::endl;
                pause();
                return;
            }

            std::cout << "\n┌─────┬──────────────┬───────────┬─────────────────────┬──────────────────────┐" << std::endl;
            std::cout << "│ ID  │ Старый       │ Новый     │ Дата изменения      │ Изменил              │" << std::endl;
            std::cout << "├─────┼──────────────┼───────────┼─────────────────────┼──────────────────────┤" << std::endl;

            for (const auto& record : history) {
                std::string oldStatus = record[2].empty() ? "NULL" : record[2];
                std::string changedBy = record[6].empty() ? "Система" : record[6];

                std::cout << "│ " << std::setw(3) << std::left << record[0]
                         << " │ " << std::setw(12) << oldStatus
                         << " │ " << std::setw(9) << record[3]
                         << " │ " << std::setw(19) << record[4].substr(0, 19)
                         << " │ " << std::setw(20) << changedBy.substr(0, 20) << " │" << std::endl;
            }
            std::cout << "└─────┴──────────────┴───────────┴─────────────────────┴──────────────────────┘" << std::endl;

            std::cout << "\nВсего изменений: " << history.size() << std::endl;

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }
};

// Функция для запуска меню покупателя
void runCustomerMenu(std::shared_ptr<DatabaseConnection> db, int userId) {
    try {
        CustomerMenu menu(db, userId);
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
    }
}
