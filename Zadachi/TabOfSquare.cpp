#include <iostream>
using namespace std;

int tabsq() {
    int n;

    // Ввод числа n
    cout << "Введите число n: ";
    cin >> n;

    // Проверка корректности ввода
    if (n <= 0) {
        cout << "Ошибка: n должно быть положительным числом!" << endl;
        return 1;
    }

    // Вывод квадратов чисел от 1 до n
    cout << "Квадраты чисел от 1 до " << n << ":" << endl;
    for (int i = 1; i <= n; i++) {
        cout << i << "^2 = " << i * i << endl;
    }

    return 0;
}
