#include <iostream>
using namespace std;

int div() {
    int number;

    // Ввод числа
    cout << "Введите целое число: ";
    cin >> number;

    // Вывод результата
    cout << "Число " << number;

    if (number % 3 == 0 && number % 5 == 0) {
        cout << " делится на 3 и на 5 одновременно" << endl;
    }
    else if (number % 3 == 0) {
        cout << " делится на 3" << endl;
    }
    else if (number % 5 == 0) {
        cout << " делится на 5" << endl;
    }
    else {
        cout << " не делится ни на 3, ни на 5" << endl;
    }

    return 0;
}