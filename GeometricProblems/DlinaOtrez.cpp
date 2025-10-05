#include <iostream>
#include <cmath>

using namespace std;

int DlinOtr() {
    setlocale(LC_ALL, "ru");

    int points[2][2]; // Две точки, каждая с двумя координатами

    cout << "Введите координаты первой точки (x y): ";
    cin >> points[0][0] >> points[0][1];

    cout << "Введите координаты второй точки (x y): ";
    cin >> points[1][0] >> points[1][1];

    // Вычисляем разности координат
    int dx = points[1][0] - points[0][0];
    int dy = points[1][1] - points[0][1];

    // Вычисляем длину по теореме Пифагора
    double length = sqrt(dx * dx + dy * dy);

    cout << "Координаты точек: (" << points[0][0] << "," << points[0][1]
        << ") и (" << points[1][0] << "," << points[1][1] << ")" << endl;
    cout << "Длина отрезка: " << length << endl;

    return 0;
}