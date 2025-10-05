#include <iostream>
using namespace std;

int CmToM() {
    int cm;

    // Ввод длины в сантиметрах
    cout << "Введите длину в сантиметрах: ";
    cin >> cm;

    // Проверка на отрицательное значение
    if (cm < 0) {
        cout << "Ошибка: длина не может быть отрицательной!" << endl;
        return 1;
    }

    // Вычисление метров и сантиметров
    int meters = cm / 100;      // Полные метры
    int centimeters = cm % 100; // Оставшиеся сантиметры

    // Вывод результата
    cout << cm << " см = " << meters << " м " << centimeters << " см" << endl;

    return 0;
}