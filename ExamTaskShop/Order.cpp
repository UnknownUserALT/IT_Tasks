// Order.hpp
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <algorithm>
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

// Реализации методов Order
inline Order::Order(int id, int user)
    : orderId(id), userId(user), status("pending"), totalPrice(0.0),
      orderDate(std::chrono::system_clock::now()), payment(nullptr) {
}

inline Order::~Order() {
}

inline bool Order::addItem(int productId, const std::string& productName, double price, int quantity) {
    auto item = std::make_unique<OrderItem>(items.size() + 1, productId, orderId, productName, price, quantity);
    items.push_back(std::move(item));
    return true;
}

inline bool Order::removeItem(int productId) {
    auto it = std::find_if(items.begin(), items.end(),
        [productId](const std::unique_ptr<OrderItem>& item) {
            return item->getProductId() == productId;
        });

    if (it != items.end()) {
        items.erase(it);
        return true;
    }
    return false;
}

inline bool Order::updateItemQuantity(int productId, int newQuantity) {
    for (auto& item : items) {
        if (item->getProductId() == productId) {
            item->setQuantity(newQuantity);
            return true;
        }
    }
    return false;
}

inline void Order::calculateTotal() {
    totalPrice = 0.0;
    for (const auto& item : items) {
        totalPrice += item->getPrice() * item->getQuantity();
    }
}

inline void Order::applyDiscount(double discountPercent) {
    totalPrice *= (1.0 - discountPercent / 100.0);
}

inline bool Order::processPayment(double amount, const std::string& method) {
    payment = std::make_unique<Payment>(orderId, amount, method);
    return payment->processTransaction();
}

inline std::string Order::getPaymentStatus() const {
    if (payment) {
        return payment->getStatus();
    }
    return "not_paid";
}

inline void Order::setStatus(const std::string& newStatus) {
    status = newStatus;
}

inline bool Order::canBeCanceled() const {
    return status == "pending";
}

inline bool Order::canBeReturned() const {
    return status == "completed";
}

inline std::string Order::serialize() const {
    return "Order#" + std::to_string(orderId);
}