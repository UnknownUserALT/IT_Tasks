// PaymentStrategy.hpp
// Паттерн Стратегия для различных способов оплаты
#pragma once
#include <string>
#include <memory>
#include <iostream>

// Абстрактный класс стратегии оплаты
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;

    // Чисто виртуальный метод для обработки платежа
    virtual bool processPayment(double amount, const std::string& orderInfo) = 0;

    // Получить название способа оплаты
    virtual std::string getPaymentMethodName() const = 0;
};

// Конкретная стратегия: Оплата банковской картой
class CreditCardPayment : public PaymentStrategy {
private:
    std::string cardNumber;
    std::string cardHolder;
    std::string cvv;
    std::string expiryDate;

public:
    CreditCardPayment() : cardNumber(""), cardHolder(""), cvv(""), expiryDate("") {}

    CreditCardPayment(const std::string& number, const std::string& holder,
                     const std::string& cvv, const std::string& expiry)
        : cardNumber(number), cardHolder(holder), cvv(cvv), expiryDate(expiry) {}

    bool processPayment(double amount, const std::string& orderInfo) override {
        std::cout << "\n=== Оплата банковской картой ===" << std::endl;
        std::cout << "Заказ: " << orderInfo << std::endl;
        std::cout << "Сумма к оплате: $" << amount << std::endl;

        // Симуляция валидации карты
        if (cardNumber.length() < 16) {
            std::cout << "Введите номер карты (16 цифр): ";
            std::cin >> cardNumber;
            std::cout << "Введите имя держателя карты: ";
            std::cin.ignore();
            std::getline(std::cin, cardHolder);
            std::cout << "Введите CVV (3 цифры): ";
            std::cin >> cvv;
            std::cout << "Введите срок действия (MM/YY): ";
            std::cin >> expiryDate;
        }

        // Валидация данных карты
        if (cardNumber.length() != 16) {
            std::cerr << "Ошибка: Некорректный номер карты" << std::endl;
            return false;
        }

        if (cvv.length() != 3) {
            std::cerr << "Ошибка: Некорректный CVV" << std::endl;
            return false;
        }

        // Симуляция обработки платежа
        std::cout << "Обработка платежа..." << std::endl;
        std::cout << "Карта: **** **** **** " << cardNumber.substr(12) << std::endl;
        std::cout << "Держатель: " << cardHolder << std::endl;
        std::cout << "✓ Платеж успешно обработан через банковскую карту" << std::endl;

        return true;
    }

    std::string getPaymentMethodName() const override {
        return "credit_card";
    }
};

// Конкретная стратегия: Оплата через электронный кошелек
class EWalletPayment : public PaymentStrategy {
private:
    std::string walletId;
    std::string walletType; // "yandex_money", "qiwi", "webmoney" и т.д.

public:
    EWalletPayment() : walletId(""), walletType("") {}

    EWalletPayment(const std::string& id, const std::string& type)
        : walletId(id), walletType(type) {}

    bool processPayment(double amount, const std::string& orderInfo) override {
        std::cout << "\n=== Оплата через электронный кошелек ===" << std::endl;
        std::cout << "Заказ: " << orderInfo << std::endl;
        std::cout << "Сумма к оплате: $" << amount << std::endl;

        if (walletId.empty()) {
            std::cout << "\nВыберите тип кошелька:" << std::endl;
            std::cout << "1. Яндекс.Деньги" << std::endl;
            std::cout << "2. QIWI Кошелек" << std::endl;
            std::cout << "3. WebMoney" << std::endl;
            std::cout << "Ваш выбор: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: walletType = "yandex_money"; break;
                case 2: walletType = "qiwi"; break;
                case 3: walletType = "webmoney"; break;
                default:
                    std::cerr << "Некорректный выбор" << std::endl;
                    return false;
            }

            std::cout << "Введите ID кошелька: ";
            std::cin >> walletId;
        }

        // Валидация ID кошелька
        if (walletId.length() < 6) {
            std::cerr << "Ошибка: Некорректный ID кошелька" << std::endl;
            return false;
        }

        // Симуляция обработки платежа
        std::cout << "Обработка платежа..." << std::endl;
        std::cout << "Кошелек: " << walletType << std::endl;
        std::cout << "ID: " << walletId << std::endl;
        std::cout << "✓ Платеж успешно обработан через электронный кошелек" << std::endl;

        return true;
    }

    std::string getPaymentMethodName() const override {
        return "e_wallet";
    }
};

// Конкретная стратегия: Оплата через Систему Быстрых Платежей (СБП)
class SBPPayment : public PaymentStrategy {
private:
    std::string phoneNumber;
    std::string bankName;

public:
    SBPPayment() : phoneNumber(""), bankName("") {}

    SBPPayment(const std::string& phone, const std::string& bank)
        : phoneNumber(phone), bankName(bank) {}

    bool processPayment(double amount, const std::string& orderInfo) override {
        std::cout << "\n=== Оплата через СБП (Система Быстрых Платежей) ===" << std::endl;
        std::cout << "Заказ: " << orderInfo << std::endl;
        std::cout << "Сумма к оплате: $" << amount << std::endl;

        if (phoneNumber.empty()) {
            std::cout << "Введите номер телефона (11 цифр, начиная с 7 или 8): ";
            std::cin >> phoneNumber;

            std::cout << "\nВыберите банк:" << std::endl;
            std::cout << "1. Сбербанк" << std::endl;
            std::cout << "2. Тинькофф" << std::endl;
            std::cout << "3. Альфа-Банк" << std::endl;
            std::cout << "4. ВТБ" << std::endl;
            std::cout << "5. Другой банк" << std::endl;
            std::cout << "Ваш выбор: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: bankName = "Сбербанк"; break;
                case 2: bankName = "Тинькофф"; break;
                case 3: bankName = "Альфа-Банк"; break;
                case 4: bankName = "ВТБ"; break;
                case 5:
                    std::cout << "Введите название банка: ";
                    std::cin.ignore();
                    std::getline(std::cin, bankName);
                    break;
                default:
                    std::cerr << "Некорректный выбор" << std::endl;
                    return false;
            }
        }

        // Валидация номера телефона
        if (phoneNumber.length() != 11 || (phoneNumber[0] != '7' && phoneNumber[0] != '8')) {
            std::cerr << "Ошибка: Некорректный номер телефона" << std::endl;
            return false;
        }

        // Симуляция обработки платежа
        std::cout << "Обработка платежа..." << std::endl;
        std::cout << "Телефон: +" << phoneNumber << std::endl;
        std::cout << "Банк: " << bankName << std::endl;
        std::cout << "Генерация QR-кода для оплаты..." << std::endl;
        std::cout << "✓ Платеж успешно обработан через СБП" << std::endl;

        return true;
    }

    std::string getPaymentMethodName() const override {
        return "sbp";
    }
};

// Контекст для использования стратегии оплаты
class PaymentContext {
private:
    std::unique_ptr<PaymentStrategy> strategy;

public:
    PaymentContext() : strategy(nullptr) {}

    // Установка стратегии оплаты
    void setPaymentStrategy(std::unique_ptr<PaymentStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    // Выполнение платежа с использованием выбранной стратегии
    bool executePayment(double amount, const std::string& orderInfo) {
        if (!strategy) {
            std::cerr << "Ошибка: Стратегия оплаты не выбрана" << std::endl;
            return false;
        }

        return strategy->processPayment(amount, orderInfo);
    }

    // Получить название текущей стратегии оплаты
    std::string getCurrentPaymentMethod() const {
        if (!strategy) {
            return "none";
        }
        return strategy->getPaymentMethodName();
    }
};
