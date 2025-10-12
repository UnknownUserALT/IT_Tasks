#include <iostream>
#include <iomanip>
using namespace std;

int systemeofequations8() {
    setlocale(LC_ALL, "ru");
    // Система уравнений:
    // 3x - y + 2z = 5
    // 2x + 4y - z = 6
    // x - 3y + z = 4

    // Матрица коэффициентов и свободных членов
    double a[3][4] = {
        {3, -1,  2, 5},
        {2,  4, -1, 6},
        {1, -3,  1, 4}
    };

    // Прямой ход метода Гаусса
    for (int i = 0; i < 3; i++) {
        // Поиск максимального элемента в столбце
        int maxRow = i;
        for (int k = i + 1; k < 3; k++) {
            if (abs(a[k][i]) > abs(a[maxRow][i])) {
                maxRow = k;
            }
        }

        // Перестановка строк
        for (int k = i; k < 4; k++) {
            double tmp = a[maxRow][k];
            a[maxRow][k] = a[i][k];
            a[i][k] = tmp;
        }

        // Приведение к треугольному виду
        for (int k = i + 1; k < 3; k++) {
            double coef = a[k][i] / a[i][i];
            for (int j = i; j < 4; j++) {
                a[k][j] -= coef * a[i][j];
            }
        }
    }

    // Обратный ход
    double x[3];
    for (int i = 2; i >= 0; i--) {
        x[i] = a[i][3];
        for (int j = i + 1; j < 3; j++) {
            x[i] -= a[i][j] * x[j];
        }
        x[i] /= a[i][i];
    }

    cout << "Решение системы уравнений:" << endl;
    cout << "3x - y + 2z = 5" << endl;
    cout << "2x + 4y - z = 6" << endl;
    cout << "x - 3y + z = 4" << endl;
    cout << endl;

    cout << fixed << setprecision(2);
    cout << "x = " << x[0] << endl;
    cout << "y = " << x[1] << endl;
    cout << "z = " << x[2] << endl;

    // Проверка решения
    cout << endl << "Проверка:" << endl;
    cout << "3*" << x[0] << " - " << x[1] << " + 2*" << x[2] << " = "
        << (3 * x[0] - x[1] + 2 * x[2]) << endl;
    cout << "2*" << x[0] << " + 4*" << x[1] << " - " << x[2] << " = "
        << (2 * x[0] + 4 * x[1] - x[2]) << endl;
    cout << x[0] << " - 3*" << x[1] << " + " << x[2] << " = "
        << (x[0] - 3 * x[1] + x[2]) << endl;

    return 0;
}