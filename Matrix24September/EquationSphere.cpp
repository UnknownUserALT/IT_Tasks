#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
// Функция для вычисления определителя матрицы 4x4


// Функция для решения системы линейных уравнений методом Гаусса
bool solveGauss(double matrix[4][5], double result[4]) {
    const int n = 4;

    // Прямой ход метода Гаусса
    for (int i = 0; i < n; i++) {
        // Поиск главного элемента
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (abs(matrix[k][i]) > abs(matrix[maxRow][i])) {
                maxRow = k;
            }
        }

        // Перестановка строк
        if (maxRow != i) {
            for (int k = i; k <= n; k++) {
                swap(matrix[i][k], matrix[maxRow][k]);
            }
        }

        // Проверка на вырожденность
        if (abs(matrix[i][i]) < 1e-10) {
            return false;
        }

        // Прямой ход - обнуление элементов под главной диагональю
        for (int k = i + 1; k < n; k++) {
            double factor = matrix[k][i] / matrix[i][i];
            for (int j = i; j <= n; j++) {
                matrix[k][j] -= factor * matrix[i][j];
            }
        }
    }

    // Обратный ход
    for (int i = n - 1; i >= 0; i--) {
        result[i] = matrix[i][n];
        for (int j = i + 1; j < n; j++) {
            result[i] -= matrix[i][j] * result[j];
        }
        result[i] /= matrix[i][i];
    }

    return true;
}

// Функция для нахождения уравнения сферы
void findSphere(double x[4], double y[4], double z[4]) {
    // Создаем матрицу системы уравнений
    // Для каждой точки: x^2 + y^2 + z^2 + Ax + By + Cz + D = 0
    double matrix[4][5];

    for (int i = 0; i < 4; i++) {
        matrix[i][0] = x[i];        // коэффициент при A
        matrix[i][1] = y[i];        // коэффициент при B
        matrix[i][2] = z[i];        // коэффициент при C
        matrix[i][3] = 1;           // коэффициент при D
        matrix[i][4] = -(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]); // правая часть
    }

    double coefficients[4]; // A, B, C, D

    if (!solveGauss(matrix, coefficients)) {
        cout << "Ошибка: невозможно найти уникальную сферу (точки компланарны или совпадают)" << endl;
        return;
    }

    double A = coefficients[0];
    double B = coefficients[1];
    double C = coefficients[2];
    double D = coefficients[3];

    // Находим центр сферы и радиус
    double x0 = -A / 2;
    double y0 = -B / 2;
    double z0 = -C / 2;
    double r = sqrt(x0 * x0 + y0 * y0 + z0 * z0 - D);

    // Вывод результатов
    cout << fixed << setprecision(4);
    cout << "\n=== РЕЗУЛЬТАТЫ ===" << endl;
    cout << "\nУравнение сферы в общем виде:" << endl;
    cout << "x² + y² + z²";
    if (A >= 0) cout << " + " << A << "x";
    else cout << " - " << abs(A) << "x";
    if (B >= 0) cout << " + " << B << "y";
    else cout << " - " << abs(B) << "y";
    if (C >= 0) cout << " + " << C << "z";
    else cout << " - " << abs(C) << "z";
    if (D >= 0) cout << " + " << D;
    else cout << " - " << abs(D);
    cout << " = 0" << endl;

    cout << "\nКоэффициенты:" << endl;
    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "C = " << C << endl;
    cout << "D = " << D << endl;

    cout << "\nЦентр сферы: (" << x0 << ", " << y0 << ", " << z0 << ")" << endl;
    cout << "Радиус: " << r << endl;

    cout << "\nУравнение в каноническом виде:" << endl;
    cout << "(x - " << x0 << ")² + (y - " << y0 << ")² + (z - " << z0 << ")² = " << r * r << endl;

    // Проверка
    cout << "\n=== ПРОВЕРКА ===" << endl;
    for (int i = 0; i < 4; i++) {
        double dist = sqrt(pow(x[i] - x0, 2) + pow(y[i] - y0, 2) + pow(z[i] - z0, 2));
        cout << "Точка " << i + 1 << " (" << x[i] << ", " << y[i] << ", " << z[i] << "): ";
        cout << "расстояние до центра = " << dist;
        if (abs(dist - r) < 1e-6) {
            cout << " ✓" << endl;
        }
        else {
            cout << " ✗" << endl;
        }
    }
}
int equationsp() {
    setlocale(LC_ALL, "ru");

    cout << "=== НАХОЖДЕНИЕ УРАВНЕНИЯ СФЕРЫ ПО ЧЕТЫРЕМ ТОЧКАМ ===" << endl;

    double x[4], y[4], z[4];

    cout << "\nВведите координаты четырех точек:" << endl;

    for (int i = 0; i < 4; i++) {
        cout << "Точка " << (char)('A' + i) << " (x y z): ";
        cin >> x[i] >> y[i] >> z[i];
    }

    findSphere(x, y, z);

    return 0;
}