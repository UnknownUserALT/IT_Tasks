// Customer.hpp
#include "User.cpp"
#include "DatabaseConnection.hpp"
#include "Order.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

class Customer : public User {
private:
    std::shared_ptr<DatabaseConnection> db;
    std::unique_ptr<Order> currentCart;

public:
    Customer(int id, const std::string& name, const std::string& email,
             std::shared_ptr<DatabaseConnection> database, int loyalty = 0)
        : User(id, name, email, "customer", loyalty), db(database) {
        if (!db || !db->isConnected()) {
            throw std::runtime_error("Database connection is not available");
        }
        currentCart = nullptr;
    }

    ~Customer() {
        // При разрушении покупателя, если есть неоплаченная корзина, она очищается
        if (currentCart) {
            std::cout << "Clearing unpaid cart for customer: " << getName() << std::endl;
            currentCart.reset();
        }
    }

    // Реализация чисто виртуальных методов
    std::unique_ptr<Order> createOrder() override {
        try {
            // Проверяем, есть ли уже активная корзина
            if (currentCart) {
                std::cerr << "Customer already has an active cart. Please checkout or clear current cart first." << std::endl;
                return nullptr;
            }

            // Создаем новый заказ (корзину) с временным ID = 0
            currentCart = std::make_unique<Order>(0, this->getUserId());
            std::cout << "New cart created for customer: " << getName() << std::endl;

            return std::make_unique<Order>(0, this->getUserId());
        } catch (const std::exception& e) {
            std::cerr << "Error creating order: " << e.what() << std::endl;
            return nullptr;
        }
    }

    bool cancelOrder(int orderId) override {
        try {
            // Покупатель может отменять только свои заказы
            auto result = db->executeQuery(
                "SELECT status FROM orders WHERE order_id = $1 AND user_id = $2",
                orderId, this->getUserId());

            if (result.empty()) {
                std::cerr << "Order not found or you don't have permission to cancel this order: "
                         << orderId << std::endl;
                return false;
            }

            std::string currentStatus = result[0][0];

            // Покупатель может отменять только pending заказы
            if (currentStatus != "pending") {
                std::cerr << "Cannot cancel order with status: " << currentStatus
                         << ". Only pending orders can be canceled." << std::endl;
                return false;
            }

            // Начинаем транзакцию
            db->beginTransaction();
            try {
                // Обновляем статус заказа
                db->getCurrentTransaction().exec(
                    "UPDATE orders SET status = 'canceled' WHERE order_id = $1", pqxx::params(orderId));

                // Возвращаем товары на склад
                auto items = db->getCurrentTransaction().exec(
                    "SELECT product_id, quantity FROM order_items WHERE order_id = $1", pqxx::params(orderId));

                for (const auto& row : items) {
                    int productId = row[0].as<int>();
                    int quantity = row[1].as<int>();

                    db->getCurrentTransaction().exec(
                        "UPDATE products SET stock_quantity = stock_quantity + $1 WHERE product_id = $2",
                        pqxx::params(quantity, productId));
                }

                // Логируем изменение статуса
                db->getCurrentTransaction().exec(
                    "INSERT INTO order_status_history (order_id, old_status, new_status, changed_by) "
                    "VALUES ($1, $2, $3, $4)",
                    pqxx::params(orderId, currentStatus, "canceled", this->getUserId()));

                db->commitTransaction();

                std::cout << "Order canceled successfully: " << orderId << std::endl;
                std::cout << "Products returned to stock" << std::endl;
                return true;

            } catch (const std::exception& e) {
                db->rollbackTransaction();
                throw;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error canceling order: " << e.what() << std::endl;
            return false;
        }
    }

    std::string viewOrderStatus(int orderId) override {
        try {
            // Покупатель может видеть только свои заказы
            auto result = db->executeQuery(
                "SELECT status FROM orders WHERE order_id = $1 AND user_id = $2",
                orderId, this->getUserId());

            if (result.empty()) {
                return "Order not found or access denied";
            }

            return result[0][0];
        } catch (const std::exception& e) {
            return "Error: " + std::string(e.what());
        }
    }

    // Методы покупателя
    bool addToOrder(int productId, int quantity) {
        if (quantity <= 0) {
            std::cerr << "Quantity must be greater than 0" << std::endl;
            return false;
        }

        try {
            // Проверяем, есть ли активная корзина
            if (!currentCart) {
                std::cerr << "No active cart. Please create an order first." << std::endl;
                return false;
            }

            // Получаем информацию о продукте
            auto productResult = db->executeQuery(
                "SELECT name, price, stock_quantity FROM products WHERE product_id = $1",
                productId);

            if (productResult.empty()) {
                std::cerr << "Product not found: " << productId << std::endl;
                return false;
            }

            std::string productName = productResult[0][0];
            double price = std::stod(productResult[0][1]);
            int stockQuantity = std::stoi(productResult[0][2]);

            // Проверяем наличие товара на складе
            if (stockQuantity < quantity) {
                std::cerr << "Insufficient stock for product: " << productName
                         << " (Available: " << stockQuantity
                         << ", Requested: " << quantity << ")" << std::endl;
                return false;
            }

            // Добавляем товар в корзину
            bool added = currentCart->addItem(productId, productName, price, quantity);
            if (added) {
                std::cout << "Added to cart: " << productName
                         << " x" << quantity
                         << " at $" << price << " each" << std::endl;
                currentCart->calculateTotal();
                std::cout << "Cart total: $" << currentCart->getTotalPrice() << std::endl;
                return true;
            } else {
                std::cerr << "Failed to add product to cart" << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error adding to order: " << e.what() << std::endl;
            return false;
        }
    }

    bool removeFromOrder(int productId) {
        try {
            // Проверяем, есть ли активная корзина
            if (!currentCart) {
                std::cerr << "No active cart" << std::endl;
                return false;
            }

            // Удаляем товар из корзины
            bool removed = currentCart->removeItem(productId);
            if (removed) {
                std::cout << "Product removed from cart: ID " << productId << std::endl;
                currentCart->calculateTotal();
                std::cout << "Cart total: $" << currentCart->getTotalPrice() << std::endl;
                return true;
            } else {
                std::cerr << "Product not found in cart: " << productId << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error removing from order: " << e.what() << std::endl;
            return false;
        }
    }

    bool makePayment(const std::string& paymentMethod) {
        try {
            // Проверяем, есть ли активная корзина
            if (!currentCart) {
                std::cerr << "No active cart to checkout" << std::endl;
                return false;
            }

            // Проверяем, что корзина не пуста
            if (currentCart->getItemCount() == 0) {
                std::cerr << "Cart is empty" << std::endl;
                return false;
            }

            double total = currentCart->getTotalPrice();

            // Применяем скидку для премиум пользователей
            double discount = 0.0;
            if (this->getLoyaltyLevel() == 1) {
                discount = 0.1; // 10% скидка для премиум
                total *= (1 - discount);
                std::cout << "Premium discount applied: 10%" << std::endl;
            }

            // Начинаем транзакцию
            db->beginTransaction();
            try {
                // Создаем запись заказа в базе данных
                auto orderResult = db->getCurrentTransaction().exec(
                    "INSERT INTO orders (user_id, status, total_price) "
                    "VALUES ($1, 'pending', $2) RETURNING order_id",
                    pqxx::params(this->getUserId(), total));

                int orderId = orderResult[0][0].as<int>();

                // Добавляем товары из корзины в базу данных
                const auto& items = currentCart->getItems();
                for (const auto& item : items) {
                    // Обновляем количество на складе
                    db->getCurrentTransaction().exec(
                        "UPDATE products SET stock_quantity = stock_quantity - $1 "
                        "WHERE product_id = $2",
                        pqxx::params(item->getQuantity(), item->getProductId()));

                    // Добавляем запись в order_items
                    db->getCurrentTransaction().exec(
                        "INSERT INTO order_items (order_id, product_id, quantity, price) "
                        "VALUES ($1, $2, $3, $4)",
                        pqxx::params(orderId, item->getProductId(), item->getQuantity(), item->getPrice()));
                }

                // Создаем запись оплаты
                db->getCurrentTransaction().exec(
                    "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
                    "VALUES ('order', $1, 'insert', $2)",
                    pqxx::params(orderId, this->getUserId()));

                db->commitTransaction();

                // Обрабатываем оплату
                bool paymentSuccess = currentCart->processPayment(total, paymentMethod);
                if (!paymentSuccess) {
                    std::cerr << "Payment processing failed" << std::endl;
                    // Если оплата не прошла, меняем статус заказа
                    db->executeQuery(
                        "UPDATE orders SET status = 'canceled' WHERE order_id = $1", orderId);
                    return false;
                }

                // Обновляем статус заказа на completed
                db->executeQuery(
                    "UPDATE orders SET status = 'completed' WHERE order_id = $1", orderId);

                // Логируем изменение статуса
                db->executeQuery(
                    "INSERT INTO order_status_history (order_id, old_status, new_status, changed_by) "
                    "VALUES ($1, $2, $3, $4)",
                    orderId, "pending", "completed", this->getUserId());

                std::cout << "Payment successful!" << std::endl;
                std::cout << "Order #" << orderId << " created" << std::endl;
                std::cout << "Total amount: $" << total << std::endl;
                std::cout << "Payment method: " << paymentMethod << std::endl;
                std::cout << "Items purchased: " << items.size() << std::endl;

                // Очищаем корзину
                currentCart.reset();

                // Увеличиваем счетчик заказов для уровня лояльности
                auto orderCount = db->executeQuery(
                    "SELECT COUNT(*) FROM orders WHERE user_id = $1 AND status = 'completed'",
                    this->getUserId());

                if (!orderCount.empty() && std::stoi(orderCount[0][0]) >= 5) {
                    db->executeQuery(
                        "UPDATE users SET loyalty_level = 1 WHERE user_id = $1",
                        this->getUserId());
                    this->setLoyaltyLevel(1);
                    std::cout << "Congratulations! You've been upgraded to Premium loyalty level!" << std::endl;
                }

                return true;

            } catch (const std::exception& e) {
                db->rollbackTransaction();
                throw;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error during payment: " << e.what() << std::endl;
            return false;
        }
    }

};