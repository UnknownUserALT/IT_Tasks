#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
int areatriangle() {
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3;

    // Ввод координат трех точек
    cout << "Введите координаты точки A (x1 y1): ";
    cin >> x1 >> y1;

    cout << "Введите координаты точки B (x2 y2): ";
    cin >> x2 >> y2;

    cout << "Введите координаты точки C (x3 y3): ";
    cin >> x3 >> y3;

    // Создаем матрицу 3x3 и вычисляем определитель
    // Матрица имеет вид:
    // | x1  y1  1 |
    // | x2  y2  1 |
    // | x3  y3  1 |

    // Вычисляем определитель по формуле:
    // det = x1*(y2 - y3) - y1*(x2 - x3) + 1*(x2*y3 - x3*y2)
    double determinant = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);

    // Площадь равна половине модуля определителя
    double area = 0.5 * abs(determinant);

    // Вывод результата
    cout << fixed << setprecision(2);
    cout << "Площадь треугольника: " << area << endl;

    return 0;
}