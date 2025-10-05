#include<iostream>
#include <cmath>

using namespace std;

int DiagPr()
{
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3, x4, y4;

    cout << "Введите координаты 4 вершин прямоугольника:" << endl;
    cout << "Вершина 1 (x y): ";
    cin >> x1 >> y1;
    cout << "Вершина 2 (x y): ";
    cin >> x2 >> y2;
    cout << "Вершина 3 (x y): ";
    cin >> x3 >> y3;
    cout << "Вершина 4 (x y): ";
    cin >> x4 >> y4;

    // Диагональ между вершинами 1 и 3 (противоположные)
    double dx = x3 - x1;
    double dy = y3 - y1;
    double diagonal = sqrt(dx * dx + dy * dy);

    cout << "Длина диагонали: " << diagonal << endl;

    return 0;
}