#include<iostream>
#include <cmath>

using namespace std;

int TriToch()
{
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3;

    // Ввод координат трех точек
    cout << "Введите координаты трех точек:" << endl;
    cout << "Точка 1 (x y): ";
    cin >> x1 >> y1;
    cout << "Точка 2 (x y): ";
    cin >> x2 >> y2;
    cout << "Точка 3 (x y): ";
    cin >> x3 >> y3;

    // Вычисляем векторное произведение
    double cross_product = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);

    // Проверяем, равно ли векторное произведение нулю (с учетом погрешности)
    if (abs(cross_product) < 1e-9) {
        cout << "Точки лежат на одной прямой" << endl;
    }
    else {
        cout << "Точки НЕ лежат на одной прямой" << endl;
    }

    return 0;
}