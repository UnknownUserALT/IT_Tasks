
#include <iostream>
using namespace std;

int arearec() {
    double length, width;

    // Ввод длины и ширины
    cout << "Введите длину прямоугольника: ";
    cin >> length;

    cout << "Введите ширину прямоугольника: ";
    cin >> width;

    // Проверка на корректность ввода
    if (length <= 0 || width <= 0) {
        cout << "Ошибка: длина и ширина должны быть положительными числами!" << endl;
        return 1;
    }

    // Вычисление площади
    double area = length * width;

    // Вывод результата
    cout << "Площадь прямоугольника: " << area << endl;

    return 0;
}