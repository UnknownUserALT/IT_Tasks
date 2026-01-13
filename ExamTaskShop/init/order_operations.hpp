//
// Created by Worker on 13/1/2026 AD.
//

#ifndef EXAMTASKSHOP_ORDER_OPERATIONS_HPP
#define EXAMTASKSHOP_ORDER_OPERATIONS_HPP

#include "../DatabaseConnection.hpp"
#include "../Payment.cpp"
#include <memory>
#include <vector>
#include <map>
#include <string>

// ================================
// 1. Классы для работы со статусами заказов
// ================================

class OrderStatusManager {
private:
    std::shared_ptr<DatabaseConnection> db;
    std::map<std::string, std::vector<std::string>> validTransitions;

public:
    OrderStatusManager(std::shared_ptr<DatabaseConnection> database);
    bool canChangeStatus(const std::string& currentStatus, const std::string& newStatus);
    std::string getOrderStatus(int orderId);
    bool changeOrderStatus(int orderId, const std::string& newStatus, int changedBy);
};

// ================================
// 2. Логика возврата товаров
// ================================

class ReturnManager {
public:
    struct ReturnCheckResult {
        bool canReturn;
        std::string reason;
        int daysPassed;
        double refundAmount;
    };

private:
    std::shared_ptr<DatabaseConnection> db;

public:
    ReturnManager(std::shared_ptr<DatabaseConnection> database);
    ReturnCheckResult checkProductReturn(int orderId, int productId, int quantity);
    bool returnProduct(int orderId, int productId, int quantity,
                      const std::string& reason, int userId);
};

// ================================
// 3. Паттерн Strategy для разных методов оплаты
// ================================

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual std::unique_ptr<Payment> createPayment(int paymentId, int orderId,
                                                   double amount) const = 0;
    virtual bool processPayment(int paymentId, int orderId, double amount) = 0;
    virtual std::string getMethodName() const = 0;
    virtual bool isAvailable() const { return true; }
    virtual std::map<std::string, std::string> getPaymentDetails() const = 0;
};

class CreditCardPaymentStrategy : public PaymentStrategy {
private:
    std::string cardNumber;
    std::string expiryDate;
    std::string cvv;
    std::string cardHolderName;
    bool validateCardDetails() const;

public:
    CreditCardPaymentStrategy(const std::string& cardNum, const std::string& expiry,
                              const std::string& cvvCode, const std::string& holderName = "");
    std::unique_ptr<Payment> createPayment(int paymentId, int orderId, double amount) const override;
    bool processPayment(int paymentId, int orderId, double amount) override;
    std::string getMethodName() const override;
    bool isAvailable() const override;
    std::map<std::string, std::string> getPaymentDetails() const override;
};

class EWalletPaymentStrategy : public PaymentStrategy {
private:
    std::string walletId;
    std::string walletType;
    bool validateWalletDetails() const;

public:
    EWalletPaymentStrategy(const std::string& id, const std::string& type);
    std::unique_ptr<Payment> createPayment(int paymentId, int orderId, double amount) const override;
    bool processPayment(int paymentId, int orderId, double amount) override;
    std::string getMethodName() const override;
    bool isAvailable() const override;
    std::map<std::string, std::string> getPaymentDetails() const override;
};

class SBPPaymentStrategy : public PaymentStrategy {
private:
    std::string phoneNumber;
    std::string bankName;
    bool validateDetails() const;

public:
    SBPPaymentStrategy(const std::string& phone, const std::string& bank);
    std::unique_ptr<Payment> createPayment(int paymentId, int orderId, double amount) const override;
    bool processPayment(int paymentId, int orderId, double amount) override;
    std::string getMethodName() const override;
    bool isAvailable() const override;
    std::map<std::string, std::string> getPaymentDetails() const override;
};

class PaymentContext {
private:
    std::unique_ptr<PaymentStrategy> paymentStrategy;
    std::shared_ptr<DatabaseConnection> db;
    int generatePaymentId();
    void savePaymentToDatabase(int paymentId, int orderId, double amount,
                               const std::string& method,
                               const std::map<std::string, std::string>& details);

public:
    PaymentContext(std::shared_ptr<DatabaseConnection> database);
    void setPaymentStrategy(std::unique_ptr<PaymentStrategy> strategy);
    bool processPayment(int orderId, double amount);
    std::string getCurrentMethodName() const;
};

// ================================
// 4. Фасад для работы с заказами
// ================================

class OrderFacade {
private:
    std::shared_ptr<DatabaseConnection> db;
    std::shared_ptr<OrderStatusManager> statusManager;
    std::shared_ptr<ReturnManager> returnManager;
    std::unique_ptr<PaymentContext> paymentContext;

public:
    OrderFacade(std::shared_ptr<DatabaseConnection> database);
    int createOrder(int userId, const std::vector<std::pair<int, int>>& items);
    bool processOrderPayment(int orderId, const std::string& paymentMethod,
                             const std::map<std::string, std::string>& paymentDetails);
    bool returnOrderProduct(int orderId, int productId, int quantity,
                            const std::string& reason, int userId);
    void demonstrateOrderSystem();
};

#endif //EXAMTASKSHOP_ORDER_OPERATIONS_HPP
