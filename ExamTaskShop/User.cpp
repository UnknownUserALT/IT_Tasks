// User.hpp
#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Order.cpp"

class User {
protected:
    int userId;
    std::string name;
    std::string email;
    std::string role;
    int loyaltyLevel;
    std::vector<std::shared_ptr<Order>> orders;

public:
    User(int id, const std::string& name, const std::string& email, const std::string& role, int loyalty = 0);
    virtual ~User() = default;

    // Чисто виртуальные методы
    virtual std::unique_ptr<Order> createOrder() = 0;
    virtual bool cancelOrder(int orderId) = 0;
    virtual std::string viewOrderStatus(int orderId) = 0;

    // Геттеры
    int getUserId() const { return userId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getRole() const { return role; }
    int getLoyaltyLevel() const { return loyaltyLevel; }
    void setLoyaltyLevel(int level) { loyaltyLevel = level; }
};