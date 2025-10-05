#include <iostream>
using namespace std;

int PrToch() {
    setlocale(LC_ALL, "ru");

    // Координаты вершин многоугольника (квадрат)
    int x[] = { 0, 10, 10, 0 };
    int y[] = { 0, 0, 10, 10 };
    int n = 4; // количество вершин

    // Проверяемая точка
    int px, py;
    cout << "Введите координаты точки (x y): ";
    cin >> px >> py;

    bool inside = false;

    // Проходим по всем сторонам многоугольника
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n; // следующая вершина

        // Проверяем пересечение луча со стороной
        if ((y[i] > py) != (y[j] > py)) { // луч пересекает сторону по Y
            // Вычисляем X-координату пересечения
            double intersectX = (x[j] - x[i]) * (py - y[i]) / (double)(y[j] - y[i]) + x[i];

            if (px < intersectX) {
                inside = !inside; // меняем состояние
            }
        }
    }

    if (inside) {
        cout << "Точка ВНУТРИ многоугольника" << endl;
    }
    else {
        cout << "Точка СНАРУЖИ многоугольника" << endl;
    }

    return 0;
}