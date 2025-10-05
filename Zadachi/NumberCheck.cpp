#include <iostream>
using namespace std;

int check() {
    int number;

    // Ввод числа
    cout << "Введите целое число: ";
    cin >> number;

    // Проверка четности
    if (number % 2 == 0) {
        cout << number << " - четное число" << endl;
    }
    else {
        cout << number << " - нечетное число" << endl;
    }

    return 0;
}