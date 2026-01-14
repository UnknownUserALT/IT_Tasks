// Payment.hpp (с реализацией)
#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include <random>

class Payment {
private:
    int paymentId;
    int orderId;
    double amount;
    std::string method;
    std::string status;
    std::chrono::system_clock::time_point paymentDate;

    // Генератор случайных чисел для симуляции платежей
    static std::string generateTransactionId() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(100000, 999999);
        return std::to_string(dis(gen));
    }

public:
    Payment(int paymentId, int orderId, double amount, const std::string& method)
        : paymentId(paymentId), orderId(orderId), amount(amount), method(method) {
        status = "ожидание";
        paymentDate = std::chrono::system_clock::now();
    }

    // Конструктор для вызова из Order без paymentId
    Payment(int orderId, double amount, const std::string& method)
        : paymentId(0), orderId(orderId), amount(amount), method(method) {
        status = "ожидание";
        paymentDate = std::chrono::system_clock::now();
    }

    ~Payment() = default;

    // Метод для обработки транзакции (алиас для process)
    bool processTransaction() {
        return process();
    }

    // Методы оплаты
    bool process() {
        if (amount <= 0) {
            std::cerr << "Сумма платежа должна быть больше 0" << std::endl;
            status = "ошибка";
            return false;
        }

        if (method.empty()) {
            std::cerr << "Метод оплаты не указан" << std::endl;
            status = "ошибка";
            return false;
        }

        // Симуляция обработки платежа
        std::cout << "Обработка платежа #" << paymentId
                  << " для заказа #" << orderId
                  << " - Сумма: $" << amount
                  << " - Метод: " << method << std::endl;

        // Простая логика успешного платежа
        if (method == "credit_card" || method == "debit_card" || method == "paypal") {
            std::string transactionId = generateTransactionId();
            std::cout << "Платеж успешен! ID транзакции: " << transactionId << std::endl;
            status = "завершен";
            paymentDate = std::chrono::system_clock::now();
            return true;
        } else {
            std::cerr << "Неподдерживаемый метод оплаты: " << method << std::endl;
            status = "ошибка";
            return false;
        }
    }

    bool refund() {
        if (status != "завершен") {
            std::cerr << "Невозможно вернуть платеж со статусом: " << status << std::endl;
            return false;
        }

        if (amount <= 0) {
            std::cerr << "Неверная сумма для возврата" << std::endl;
            return false;
        }

        std::cout << "Обработка возврата для платежа #" << paymentId
                  << " - Сумма: $" << amount
                  << " - Заказ: #" << orderId << std::endl;

        // Симуляция возврата средств
        std::string refundId = generateTransactionId();
        std::cout << "Возврат успешен! ID возврата: " << refundId << std::endl;
        status = "возвращен";
        return true;
    }

    bool verify() {
        if (status == "завершен") {
            std::cout << "Платеж #" << paymentId << " проверен и завершен" << std::endl;
            return true;
        } else if (status == "ожидание") {
            std::cout << "Платеж #" << paymentId << " ожидает проверки" << std::endl;
            return false;
        } else if (status == "ошибка") {
            std::cout << "Платеж #" << paymentId << " не прошел проверку" << std::endl;
            return false;
        } else if (status == "возвращен") {
            std::cout << "Платеж #" << paymentId << " был возвращен" << std::endl;
            return false;
        } else {
            std::cerr << "Неизвестный статус платежа: " << status << std::endl;
            return false;
        }
    }

    // Дополнительный метод для отмены pending платежа
    bool cancel() {
        if (status == "ожидание") {
            std::cout << "Платеж #" << paymentId << " отменен" << std::endl;
            status = "отменен";
            return true;
        } else {
            std::cerr << "Невозможно отменить платеж со статусом: " << status << std::endl;
            return false;
        }
    }

    // Геттеры
    int getPaymentId() const { return paymentId; }
    int getOrderId() const { return orderId; }
    double getAmount() const { return amount; }
    std::string getMethod() const { return method; }
    std::string getStatus() const { return status; }
    std::chrono::system_clock::time_point getPaymentDate() const { return paymentDate; }

    // Сеттеры
    void setStatus(const std::string& newStatus) { status = newStatus; }

    // Вспомогательный метод для получения даты в строковом формате
    std::string getPaymentDateString() const {
        auto time = std::chrono::system_clock::to_time_t(paymentDate);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
        return std::string(buffer);
    }
};