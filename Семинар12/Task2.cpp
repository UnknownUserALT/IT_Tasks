#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Структура для представления банковского счета
struct BankAccount {
    string accountNumber;
    string ownerName;
    double balance;
};

// Функция для поиска счета по номеру
int findAccount(vector<BankAccount>& accounts, string accountNumber) {
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1; // Если счет не найден
}

int Task2() {
    vector<BankAccount> accounts;
    int choice;
    
    while (true) {
        cout << "Выберите действие:" << endl;
        cout << "1. Создать новый банковский счет" << endl;
        cout << "2. Пополнить счет" << endl;
        cout << "3. Снять деньги со счета" << endl;
        cout << "4. Показать информацию о счете" << endl;
        cout << "5. Выйти" << endl;
        cin >> choice;
        cin.ignore();
        
        if (choice == 1) {
            // Создание нового счета
            BankAccount newAccount;
            cout << "Введите номер счета: ";
            getline(cin, newAccount.accountNumber);
            cout << "Введите имя владельца счета: ";
            getline(cin, newAccount.ownerName);
            newAccount.balance = 0; // Начальный баланс 0
            
            accounts.push_back(newAccount);
            cout << "Банковский счет создан." << endl;
            
        } else if (choice == 2) {
            // Пополнение счета
            string accountNumber;
            double amount;
            cout << "Введите номер счета для пополнения: ";
            getline(cin, accountNumber);
            
            int index = findAccount(accounts, accountNumber);
            if (index != -1) {
                cout << "Введите сумму для пополнения: ";
                cin >> amount;
                cin.ignore();
                accounts[index].balance += amount;
                cout << "Счет пополнен на " << amount << " рублей." << endl;
            } else {
                cout << "Счет не найден." << endl;
            }
            
        } else if (choice == 3) {
            // Снятие денег
            string accountNumber;
            double amount;
            cout << "Введите номер счета для снятия денег: ";
            getline(cin, accountNumber);
            
            int index = findAccount(accounts, accountNumber);
            if (index != -1) {
                cout << "Введите сумму для снятия: ";
                cin >> amount;
                cin.ignore();
                
                if (accounts[index].balance >= amount) {
                    accounts[index].balance -= amount;
                    cout << "Со счета снято " << amount << " рублей." << endl;
                } else {
                    cout << "Недостаточно средств на счете." << endl;
                }
            } else {
                cout << "Счет не найден." << endl;
            }
            
        } else if (choice == 4) {
            // Показать информацию о счете
            string accountNumber;
            cout << "Введите номер счета для показа информации: ";
            getline(cin, accountNumber);
            
            int index = findAccount(accounts, accountNumber);
            if (index != -1) {
                cout << "Номер счета: " << accounts[index].accountNumber << endl;
                cout << "Имя владельца счета: " << accounts[index].ownerName << endl;
                cout << "Баланс: " << accounts[index].balance << " рублей" << endl;
            } else {
                cout << "Счет не найден." << endl;
            }
            
        } else if (choice == 5) {
            // Выход из программы
            cout << "Программа завершена." << endl;
            break;
            
        } else {
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
        
        cout << endl; // Пустая строка для красоты
    }
    
    return 0;
}