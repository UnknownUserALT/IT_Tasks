#include <iostream>
#include <cmath>
using namespace std;
int PerMnogoug() {
    setlocale(LC_ALL, "ru");
    // Первый многоугольник
    int n1;
    double x1[10]; // координаты x
    double y1[10]; // координаты y

    cout << "Введите количество вершин первого многоугольника (3-10): ";
    cin >> n1;

    cout << "Введите координаты вершин первого многоугольника:" << endl;
    for (int i = 0; i < n1; i++) {
        cout << "Вершина " << i + 1 << " (x y): ";
        cin >> x1[i] >> y1[i];
    }

    // Второй многоугольник
    int n2;
    double x2[10];
    double y2[10];

    cout << "\nВведите количество вершин второго многоугольника (3-10): ";
    cin >> n2;

    cout << "Введите координаты вершин второго многоугольника:" << endl;
    for (int i = 0; i < n2; i++) {
        cout << "Вершина " << i + 1 << " (x y): ";
        cin >> x2[i] >> y2[i];
    }

    // Флаг пересечения
    bool peresekayutsya = false;

    // Проходим по всем сторонам первого многоугольника
    for (int i = 0; i < n1; i++) {
        // Координаты первого отрезка AB (сторона первого многоугольника)
        double ax = x1[i];
        double ay = y1[i];
        double bx = x1[(i + 1) % n1]; // следующая вершина (замыкаем на первую)
        double by = y1[(i + 1) % n1];

        // Проходим по всем сторонам второго многоугольника
        for (int j = 0; j < n2; j++) {
            // Координаты второго отрезка CD
            double cx = x2[j];
            double cy = y2[j];
            double dx = x2[(j + 1) % n2];
            double dy = y2[(j + 1) % n2];

            // Вычисляем ориентации для проверки пересечения отрезков AB и CD
            // Ориентация показывает, где находится точка относительно отрезка

            // Ориентация тройки (A, B, C)
            double o1 = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);

            // Ориентация тройки (A, B, D)
            double o2 = (bx - ax) * (dy - ay) - (by - ay) * (dx - ax);

            // Ориентация тройки (C, D, A)
            double o3 = (dx - cx) * (ay - cy) - (dy - cy) * (ax - cx);

            // Ориентация тройки (C, D, B)
            double o4 = (dx - cx) * (by - cy) - (dy - cy) * (bx - cx);

            // Общий случай: отрезки пересекаются, если точки C и D
            // находятся по разные стороны от AB, и A и B по разные стороны от CD
            if (o1 * o2 < 0 && o3 * o4 < 0) {
                peresekayutsya = true;
                break;
            }

            // Специальные случаи: точки лежат на одной прямой
            // Проверяем, лежит ли точка на отрезке
            if (o1 == 0) {
                // C лежит на AB, проверяем, внутри ли отрезка
                if (cx >= min(ax, bx) && cx <= max(ax, bx) &&
                    cy >= min(ay, by) && cy <= max(ay, by)) {
                    peresekayutsya = true;
                    break;
                }
            }
            if (o2 == 0) {
                // D лежит на AB
                if (dx >= min(ax, bx) && dx <= max(ax, bx) &&
                    dy >= min(ay, by) && dy <= max(ay, by)) {
                    peresekayutsya = true;
                    break;
                }
            }
            if (o3 == 0) {
                // A лежит на CD
                if (ax >= min(cx, dx) && ax <= max(cx, dx) &&
                    ay >= min(cy, dy) && ay <= max(cy, dy)) {
                    peresekayutsya = true;
                    break;
                }
            }
            if (o4 == 0) {
                // B лежит на CD
                if (bx >= min(cx, dx) && bx <= max(cx, dx) &&
                    by >= min(cy, dy) && by <= max(cy, dy)) {
                    peresekayutsya = true;
                    break;
                }
            }
        }
        if (peresekayutsya) {
            break;
        }
    }

    // Выводим результат
    cout << "\n";
    if (peresekayutsya) {
        cout << "Многоугольники ПЕРЕСЕКАЮТСЯ!" << endl;
    }
    else {
        cout << "Многоугольники НЕ пересекаются." << endl;
    }

    return 0;
}