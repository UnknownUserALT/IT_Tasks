#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
int movedot() {
    setlocale(LC_ALL, "ru");

    double x, y, theta_degrees;

    // Ввод координат точки и угла поворота
    cout << "Введите координаты точки A (x y): ";
    cin >> x >> y;

    cout << "Введите угол поворота в градусах: ";
    cin >> theta_degrees;

    // Преобразование градусов в радианы
    double theta_radians = theta_degrees * acos(-1) / 180.0;

    // Вычисление элементов матрицы поворота
    double cos_theta = cos(theta_radians);
    double sin_theta = sin(theta_radians);

    // Умножение матрицы поворота на вектор точки
    // x' = x*cos(θ) + y*sin(θ)
    // y' = -x*sin(θ) + y*cos(θ)
    double x_new = x * cos_theta + y * sin_theta;
    double y_new = -x * sin_theta + y * cos_theta;

    // Вывод результатов
    cout << fixed << setprecision(2);
    cout << "\nИсходная точка: (" << x << ", " << y << ")" << endl;
    cout << "Угол поворота: " << theta_degrees << "°" << endl;
    cout << "Новая точка после поворота: (" << x_new << ", " << y_new << ")" << endl;

    return 0;
}