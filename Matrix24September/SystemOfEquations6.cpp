#include <iostream>
#include <iomanip>
using namespace std;
int systemofequations6() {
    setlocale(LC_ALL, "ru");
    // Система уравнений:
    // 2x + 3y = 13
    // 8x - y = 9

    // Коэффициенты первого уравнения
    double a1 = 2, b1 = 3, c1 = 13;

    // Коэффициенты второго уравнения
    double a2 = 8, b2 = -1, c2 = 9;

    // Вычисляем определитель (метод Крамера)
    double det = a1 * b2 - a2 * b1;

    if (det == 0) {
        cout << "Система не имеет единственного решения" << endl;
        return 1;
    }

    // Находим x и y по формулам Крамера
    double x = (c1 * b2 - c2 * b1) / det;
    double y = (a1 * c2 - a2 * c1) / det;

    cout << "Решение системы уравнений:" << endl;
    cout << "2x + 3y = 13" << endl;
    cout << "8x - y = 9" << endl;
    cout << endl;
    cout << fixed << setprecision(2);
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;

    // Проверка решения
    cout << endl << "Проверка:" << endl;
    cout << "2*" << x << " + 3*" << y << " = " << (2 * x + 3 * y) << endl;
    cout << "8*" << x << " - " << y << " = " << (8 * x - y) << endl;

    return 0;
}