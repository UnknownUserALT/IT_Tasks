// OrderItem.hpp
#pragma once
#include <string>
#include <memory>

class OrderItem {
private:
    int itemId;
    int productId;
    int orderId;
    std::string productName;
    double price;
    int quantity;

public:
    OrderItem(int itemId, int productId, int orderId,
              const std::string& productName, double price, int quantity);
    ~OrderItem() = default;

    // Методы
    double calculateSubtotal() const;
    void updatePrice(double newPrice);
    void updateQuantity(int newQuantity);

    // Геттеры
    int getItemId() const { return itemId; }
    int getProductId() const { return productId; }
    int getOrderId() const { return orderId; }
    std::string getProductName() const { return productName; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    // Сеттеры
    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
};