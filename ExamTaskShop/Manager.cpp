// Manager.hpp
#include "User.cpp"
#include "DatabaseConnection.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

class Manager : public User {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    Manager(int id, const std::string& name, const std::string& email,
            std::shared_ptr<DatabaseConnection> database)
        : User(id, name, email, "manager", 0), db(database) {
        if (!db || !db->isConnected()) {
            throw std::runtime_error("Database connection is not available");
        }
    }

    // Реализация чисто виртуальных методов
    std::unique_ptr<Order> createOrder() override {
        throw std::runtime_error("Manager cannot create orders");
        return nullptr;
    }

    bool cancelOrder(int orderId) override {
        try {
            // Менеджер может отменять только pending заказы
            auto result = db->executeQuery(
                "SELECT status FROM orders WHERE order_id = $1", orderId);

            if (result.empty()) {
                std::cerr << "Order not found: " << orderId << std::endl;
                return false;
            }

            std::string currentStatus = result[0][0];
            if (currentStatus != "pending") {
                std::cerr << "Manager can only cancel pending orders. Current status: "
                         << currentStatus << std::endl;
                return false;
            }

            if (currentStatus == "canceled") {
                std::cerr << "Order already canceled" << std::endl;
                return false;
            }

            db->executeQuery(
                "UPDATE orders SET status = 'canceled' WHERE order_id = $1", orderId);

            db->executeQuery(
                "INSERT INTO order_status_history (order_id, old_status, new_status, changed_by) "
                "VALUES ($1, $2, $3, $4)",
                orderId, currentStatus, "canceled", this->getUserId());

            std::cout << "Order canceled successfully: " << orderId << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error canceling order: " << e.what() << std::endl;
            return false;
        }
    }

    std::string viewOrderStatus(int orderId) override {
        try {
            auto result = db->executeQuery(
                "SELECT status FROM orders WHERE order_id = $1", orderId);

            if (result.empty()) {
                return "Order not found";
            }

            return result[0][0];
        } catch (const std::exception& e) {
            return "Error: " + std::string(e.what());
        }
    }

    // Методы менеджера
    bool approveOrder(int orderId) {
        try {
            // Проверяем текущий статус заказа
            auto result = db->executeQuery(
                "SELECT status, user_id FROM orders WHERE order_id = $1", orderId);

            if (result.empty()) {
                std::cerr << "Order not found: " << orderId << std::endl;
                return false;
            }

            std::string currentStatus = result[0][0];
            int userId = std::stoi(result[0][1]);

            // Менеджер может утверждать только pending заказы
            if (currentStatus != "pending") {
                std::cerr << "Cannot approve order with status: " << currentStatus
                         << ". Only pending orders can be approved." << std::endl;
                return false;
            }

            // Проверяем наличие товаров на складе
            auto itemsResult = db->executeQuery(R"(
                SELECT p.product_id, p.name, p.stock_quantity, oi.quantity
                FROM order_items oi
                JOIN products p ON oi.product_id = p.product_id
                WHERE oi.order_id = $1
            )", orderId);

            if (itemsResult.empty()) {
                std::cerr << "Order has no items" << std::endl;
                return false;
            }

            std::vector<std::tuple<int, std::string, int, int>> items;
            for (const auto& row : itemsResult) {
                int productId = std::stoi(row[0]);
                std::string productName = row[1];
                int stockQuantity = std::stoi(row[2]);
                int orderedQuantity = std::stoi(row[3]);

                items.emplace_back(productId, productName, stockQuantity, orderedQuantity);

                // Проверяем, достаточно ли товара на складе
                if (stockQuantity < orderedQuantity) {
                    std::cerr << "Insufficient stock for product: " << productName
                             << " (Available: " << stockQuantity
                             << ", Ordered: " << orderedQuantity << ")" << std::endl;
                    return false;
                }
            }

            // Начинаем транзакцию для атомарного обновления
            db->beginTransaction();
            try {
                // Обновляем статус заказа
                db->getCurrentTransaction().exec(
                    "UPDATE orders SET status = 'completed' WHERE order_id = $1", pqxx::params(orderId));

                // Уменьшаем количество товаров на складе
                for (const auto& [productId, productName, stockQuantity, orderedQuantity] : items) {
                    db->getCurrentTransaction().exec(
                        "UPDATE products SET stock_quantity = stock_quantity - $1 WHERE product_id = $2",
                        pqxx::params(orderedQuantity, productId));
                }

                // Логируем изменение статуса
                db->getCurrentTransaction().exec(
                    "INSERT INTO order_status_history (order_id, old_status, new_status, changed_by) "
                    "VALUES ($1, $2, $3, $4)",
                    pqxx::params(orderId, currentStatus, "completed", this->getUserId()));

                // Логируем операцию в аудит
                db->getCurrentTransaction().exec(
                    "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
                    "VALUES ('order', $1, 'update', $2)",
                    pqxx::params(orderId, this->getUserId()));

                db->commitTransaction();

                // Проверяем уровень лояльности пользователя и обновляем при необходимости
                auto loyaltyCheck = db->executeQuery(
                    "SELECT COUNT(*) FROM orders WHERE user_id = $1 AND status = 'completed'",
                    userId);

                if (!loyaltyCheck.empty()) {
                    int completedOrders = std::stoi(loyaltyCheck[0][0]);
                    if (completedOrders >= 5) {
                        db->executeQuery(
                            "UPDATE users SET loyalty_level = 1 WHERE user_id = $1", userId);
                        std::cout << "User upgraded to premium loyalty level: " << userId << std::endl;
                    }
                }

                std::cout << "Order approved successfully: " << orderId << std::endl;
                std::cout << "Stock updated for " << items.size() << " products" << std::endl;
                return true;

            } catch (const std::exception& e) {
                db->rollbackTransaction();
                throw;
            }

        } catch (const std::exception& e) {
            std::cerr << "Error approving order: " << e.what() << std::endl;
            return false;
        }
    }

    bool updateStock(int productId, int newQuantity) {
        if (newQuantity < 0) {
            std::cerr << "Stock quantity cannot be negative" << std::endl;
            return false;
        }

        try {
            // Получаем текущее количество для лога
            auto currentResult = db->executeQuery(
                "SELECT name, stock_quantity FROM products WHERE product_id = $1", productId);

            if (currentResult.empty()) {
                std::cerr << "Product not found: " << productId << std::endl;
                return false;
            }

            std::string productName = currentResult[0][0];
            int currentQuantity = std::stoi(currentResult[0][1]);

            // Обновляем количество на складе
            db->executeQuery(
                "UPDATE products SET stock_quantity = $1 WHERE product_id = $2",
                newQuantity, productId);

            // Логируем операцию в аудит
            db->executeQuery(
                "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
                "VALUES ('product', $1, 'update', $2)",
                productId, this->getUserId());

            std::cout << "Stock updated for product: " << productName
                     << " (ID: " << productId << ")" << std::endl;
            std::cout << "Old quantity: " << currentQuantity
                     << ", New quantity: " << newQuantity << std::endl;

            // Если количество низкое, выводим предупреждение
            if (newQuantity <= 10 && newQuantity > 0) {
                std::cout << "WARNING: Low stock for product " << productName
                         << " (Quantity: " << newQuantity << ")" << std::endl;
            } else if (newQuantity == 0) {
                std::cout << "WARNING: Product " << productName << " is out of stock!" << std::endl;
            }

            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error updating stock: " << e.what() << std::endl;
            return false;
        }
    }
};