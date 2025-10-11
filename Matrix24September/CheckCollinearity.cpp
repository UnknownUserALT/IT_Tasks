#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
int chekcollinearity() {
    setlocale(LC_ALL, "ru");
    double x1, y1, x2, y2, x3, y3;

    // Ввод координат трех точек
    cout << "Введите координаты точки A (x1 y1): ";
    cin >> x1 >> y1;

    cout << "Введите координаты точки B (x2 y2): ";
    cin >> x2 >> y2;

    cout << "Введите координаты точки C (x3 y3): ";
    cin >> x3 >> y3;

    // Вычисление определителя матрицы 3x3
    // | x1  y1  1 |
    // | x2  y2  1 |
    // | x3  y3  1 |
    double determinant = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);

    // Проверка коллинеарности
    // Используем небольшой эпсилон для учета погрешности вычислений с плавающей точкой
    const double epsilon = 1e-10;

    cout << fixed << setprecision(10);
    cout << "Определитель матрицы: " << determinant << endl;

    if (abs(determinant) < epsilon) {
        cout << "Точки коллинеарны (лежат на одной прямой)" << endl;
    }
    else {
        cout << "Точки не коллинеарны" << endl;
    }

    return 0;
}