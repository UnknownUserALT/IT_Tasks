#include <iostream>

using namespace std;

// Объединение для хранения разных типов данных
union Variant {
    int intValue;
    double doubleValue;
    char charValue;
};

int Task4() {
    Variant data;
    char choice;
    
    cout << "Выберите тип данных (i - целое число, d - число с плавающей запятой, c - символ): ";
    cin >> choice;
    
    if (choice == 'i') {
        // Работа с целым числом
        cout << "Введите целое число: ";
        cin >> data.intValue;
        cout << "Вы ввели: " << data.intValue << endl;
        
    } else if (choice == 'd') {
        // Работа с числом с плавающей запятой
        cout << "Введите число с плавающей запятой: ";
        cin >> data.doubleValue;
        cout << "Вы ввели: " << data.doubleValue << endl;
        
    } else if (choice == 'c') {
        // Работа с символом
        cout << "Введите символ: ";
        cin >> data.charValue;
        cout << "Вы ввели: " << data.charValue << endl;
        
    } else {
        cout << "Неверный выбор!" << endl;
    }
    
    return 0;
}