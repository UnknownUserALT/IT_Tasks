// Order.hpp
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include "OrderItem.cpp"
#include "Payment.cpp"

class Order {
private:
    int orderId;
    int userId;
    std::string status;
    double totalPrice;
    std::chrono::system_clock::time_point orderDate;
    std::vector<std::unique_ptr<OrderItem>> items;
    std::unique_ptr<Payment> payment;

public:
    Order(int id, int userId);
    ~Order();

    // Методы работы с элементами заказа
    bool addItem(int productId, const std::string& productName, double price, int quantity);
    bool removeItem(int productId);
    bool updateItemQuantity(int productId, int newQuantity);

    // Методы расчета
    void calculateTotal();
    void applyDiscount(double discountPercent);

    // Методы оплаты
    bool processPayment(double amount, const std::string& method);
    std::string getPaymentStatus() const;

    // Методы работы со статусом
    void setStatus(const std::string& newStatus);
    bool canBeCanceled() const;
    bool canBeReturned() const;

    // Геттеры
    int getOrderId() const { return orderId; }
    int getUserId() const { return userId; }
    std::string getStatus() const { return status; }
    double getTotalPrice() const { return totalPrice; }
    std::chrono::system_clock::time_point getOrderDate() const { return orderDate; }
    const std::vector<std::unique_ptr<OrderItem>>& getItems() const { return items; }
    size_t getItemCount() const { return items.size(); }

    // Для сериализации в базу данных
    std::string serialize() const;
};