#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class BankAccount {
protected:
    string accountNumber;
    string ownerName;
    double balance;
public:
    BankAccount(string accNum, string name, double initialBalance)
        : accountNumber(accNum), ownerName(name), balance(initialBalance) {
        if (initialBalance < 0) {
            cout << "Ошибка: начальный баланс не может быть отрицательным. Установлен 0." << endl;
            balance = 0;
        }
    }

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Успешно пополнено: " << amount << " руб." << endl;
        } else {
            cout << "Ошибка: сумма пополнения должна быть положительной." << endl;
        }
    }

    virtual bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Успешно снято: " << amount << " руб." << endl;
            return true;
        } else if (amount > balance) {
            cout << "Ошибка: недостаточно средств на счете." << endl;
            cout << "Запрошено: " << amount << " руб., доступно: " << balance << " руб." << endl;
            return false;
        } else {
            cout << "Ошибка: сумма снятия должна быть положительной." << endl;
            return false;
        }
    }

    virtual void displayInfo() const {
        cout << "=== Информация о банковском счете ===" << endl;
        cout << "Номер счета: " << accountNumber << endl;
        cout << "Владелец: " << ownerName << endl;
        cout << "Баланс: " << fixed << setprecision(2) << balance << " руб." << endl;
    }

    // Геттеры
    string getAccountNumber() const { return accountNumber; }
    string getOwnerName() const { return ownerName; }
    double getBalance() const { return balance; }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(string accNum, string name, double initialBalance, double rate)
        : BankAccount(accNum, name, initialBalance), interestRate(rate) {
        if (rate < 0) {
            cout << "Ошибка: процентная ставка не может быть отрицательной. Установлена 0%." << endl;
            interestRate = 0;
        }
    }

    void applyInterest() {
        double interest = balance * (interestRate / 100);
        if (interest > 0) {
            balance += interest;
            cout << "Начислены проценты: " << fixed << setprecision(2)
                 << interest << " руб. по ставке " << interestRate << "%" << endl;
        }
    }

    void displayInfo() const override {
        cout << "=== Информация о сберегательном счете ===" << endl;
        cout << "Номер счета: " << accountNumber << endl;
        cout << "Владелец: " << ownerName << endl;
        cout << "Баланс: " << fixed << setprecision(2) << balance << " руб." << endl;
        cout << "Процентная ставка: " << interestRate << "%" << endl;
    }

    // Геттер для процентной ставки
    double getInterestRate() const { return interestRate; }
};

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=== Моделирование работы банка ===\n" << endl;

    // Создание обычного банковского счета
    cout << "1. Создание обычного банковского счета:" << endl;
    BankAccount regularAccount("40817810099910004321", "Иванов Иван Иванович", 5000.0);
    regularAccount.displayInfo();

    cout << "\nОперации по обычному счету:" << endl;
    regularAccount.deposit(1500.50);    // Пополнение
    regularAccount.withdraw(2000.75);   // Успешное снятие
    regularAccount.withdraw(6000.00);   // Попытка снять больше, чем есть
    regularAccount.displayInfo();

    cout << "\n" << string(50, '=') << "\n" << endl;

    // Создание сберегательного счета
    cout << "2. Создание сберегательного счета:" << endl;
    SavingsAccount savingsAccount("42307810290009876543", "Петрова Мария Сергеевна", 10000.0, 5.5);
    savingsAccount.displayInfo();

    cout << "\nОперации по сберегательному счету:" << endl;
    savingsAccount.deposit(3000.00);    // Пополнение
    savingsAccount.withdraw(1500.00);   // Снятие

    cout << "\nНачисление процентов:" << endl;
    savingsAccount.applyInterest();      // Начисление процентов

    cout << "\nЕще одно начисление процентов после пополнения:" << endl;
    savingsAccount.deposit(2000.00);     // Еще одно пополнение
    savingsAccount.applyInterest();      // Начисление процентов

    cout << "\nИтоговая информация по сберегательному счету:" << endl;
    savingsAccount.displayInfo();

    cout << "\n" << string(50, '=') << "\n" << endl;

    return 0;
}