// Admin.hpp
#include "User.cpp"
#include "DatabaseConnection.hpp"
#include <memory>
#include <iostream>
#include <stdexcept>
#include <vector>

class Admin : public User {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    Admin(int id, const std::string& name, const std::string& email,
          std::shared_ptr<DatabaseConnection> database)
        : User(id, name, email, "admin", 0), db(database) {
        if (!db || !db->isConnected()) {
            throw std::runtime_error("Database connection is not available");
        }
    }

    // Реализация чисто виртуальных методов
    std::unique_ptr<Order> createOrder() override {
        throw std::runtime_error("Admin cannot create orders");
        return nullptr;
    }

    bool cancelOrder(int orderId) override {
        try {
            auto result = db->executeQuery(
                "SELECT status FROM orders WHERE order_id = $1", orderId);

            if (result.empty()) {
                std::cerr << "Order not found: " << orderId << std::endl;
                return false;
            }

            std::string currentStatus = result[0][0];
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

    // Методы администратора
    bool addProduct(const std::string& name, double price, int stockQuantity) {
        if (price <= 0) {
            std::cerr << "Price must be greater than 0" << std::endl;
            return false;
        }

        if (stockQuantity < 0) {
            std::cerr << "Stock quantity cannot be negative" << std::endl;
            return false;
        }

        try {
            db->executeQuery(
                "INSERT INTO products (name, price, stock_quantity) "
                "VALUES ($1, $2, $3)",
                name, price, stockQuantity);

            db->executeQuery(
                "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
                "VALUES ('product', (SELECT currval('products_product_id_seq')), 'insert', $1)",
                this->getUserId());

            std::cout << "Product added successfully: " << name << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error adding product: " << e.what() << std::endl;
            return false;
        }
    }

    bool updateProduct(int productId, const std::string& name = "",
                       double price = -1, int stockQuantity = -1) {
        try {
            auto currentProduct = db->executeQuery(
                "SELECT name, price, stock_quantity FROM products WHERE product_id = $1",
                productId);

            if (currentProduct.empty()) {
                std::cerr << "Product not found: " << productId << std::endl;
                return false;
            }

            std::string query = "UPDATE products SET ";
            std::vector<std::string> updates;

            if (!name.empty()) {
                updates.push_back("name = '" + name + "'");
            }
            if (price >= 0) {
                if (price == 0) {
                    std::cerr << "Price must be greater than 0" << std::endl;
                    return false;
                }
                updates.push_back("price = " + std::to_string(price));
            }
            if (stockQuantity >= 0) {
                updates.push_back("stock_quantity = " + std::to_string(stockQuantity));
            }

            if (updates.empty()) {
                std::cerr << "No fields to update" << std::endl;
                return false;
            }

            for (size_t i = 0; i < updates.size(); ++i) {
                query += updates[i];
                if (i < updates.size() - 1) query += ", ";
            }
            query += " WHERE product_id = " + std::to_string(productId);

            db->executeNonQuery(query);

            db->executeQuery(
                "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
                "VALUES ('product', $1, 'update', $2)",
                productId, this->getUserId());

            std::cout << "Product updated successfully: ID " << productId << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error updating product: " << e.what() << std::endl;
            return false;
        }
    }

    bool deleteProduct(int productId) {
        try {
            auto activeOrders = db->executeQuery(
                "SELECT COUNT(*) FROM order_items oi "
                "JOIN orders o ON oi.order_id = o.order_id "
                "WHERE oi.product_id = $1 AND o.status != 'canceled' AND o.status != 'returned'",
                productId);

            if (!activeOrders.empty() && std::stoi(activeOrders[0][0]) > 0) {
                std::cerr << "Cannot delete product with active orders" << std::endl;
                return false;
            }

            db->executeQuery("DELETE FROM products WHERE product_id = $1", productId);

            db->executeQuery(
                "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
                "VALUES ('product', $1, 'delete', $2)",
                productId, this->getUserId());

            std::cout << "Product deleted successfully: ID " << productId << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error deleting product: " << e.what() << std::endl;
            return false;
        }
    }

    std::vector<std::vector<std::string>> viewAllOrders() {
        try {
            return db->executeQuery(R"(
                SELECT o.order_id, u.name as customer_name, o.status,
                       o.total_price, o.order_date, COUNT(oi.order_item_id) as item_count
                FROM orders o
                JOIN users u ON o.user_id = u.user_id
                LEFT JOIN order_items oi ON o.order_id = oi.order_id
                GROUP BY o.order_id, u.name
                ORDER BY o.order_date DESC
            )");
        } catch (const std::exception& e) {
            std::cerr << "Error viewing orders: " << e.what() << std::endl;
            return {};
        }
    }

    bool updateOrderStatus(int orderId, const std::string& newStatus) {
        std::vector<std::string> validStatuses = {"pending", "completed", "canceled", "returned"};
        bool isValid = false;
        for (const auto& status : validStatuses) {
            if (status == newStatus) {
                isValid = true;
                break;
            }
        }

        if (!isValid) {
            std::cerr << "Invalid status: " << newStatus << std::endl;
            return false;
        }

        try {
            auto currentStatusResult = db->executeQuery(
                "SELECT status FROM orders WHERE order_id = $1", orderId);

            if (currentStatusResult.empty()) {
                std::cerr << "Order not found: " << orderId << std::endl;
                return false;
            }

            std::string currentStatus = currentStatusResult[0][0];

            db->executeQuery(
                "UPDATE orders SET status = $1 WHERE order_id = $2",
                newStatus, orderId);

            db->executeQuery(
                "INSERT INTO order_status_history (order_id, old_status, new_status, changed_by) "
                "VALUES ($1, $2, $3, $4)",
                orderId, currentStatus, newStatus, this->getUserId());

            db->executeQuery(
                "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
                "VALUES ('order', $1, 'update', $2)",
                orderId, this->getUserId());

            std::cout << "Order status updated: " << orderId
                      << " from " << currentStatus
                      << " to " << newStatus << std::endl;

            if (newStatus == "returned" && currentStatus != "returned") {
                auto items = db->executeQuery(
                    "SELECT product_id, quantity FROM order_items WHERE order_id = $1", orderId);

                for (const auto& item : items) {
                    int productId = std::stoi(item[0]);
                    int quantity = std::stoi(item[1]);

                    db->executeQuery(
                        "UPDATE products SET stock_quantity = stock_quantity + $1 "
                        "WHERE product_id = $2",
                        quantity, productId);
                }

                std::cout << "Products returned to stock for order: " << orderId << std::endl;
            }

            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error updating order status: " << e.what() << std::endl;
            return false;
        }
    }
};