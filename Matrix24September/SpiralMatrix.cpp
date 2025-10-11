#include <iostream>
#include <iomanip>

using namespace std;
int spiralmatrix() {
    setlocale(LC_ALL, "ru");
    int n;

    cout << "=== СПИРАЛЬНОЕ ЗАПОЛНЕНИЕ МАТРИЦЫ ===" << endl;
    cout << "\nВведите размер матрицы N (1 ≤ N ≤ 100): ";
    cin >> n;

    // Проверка корректности ввода
    if (n < 1 || n > 100) {
        cout << "Ошибка: N должно быть от 1 до 100!" << endl;
        return 1;
    }

    // Динамическое выделение памяти для матрицы
    int** matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
    }

    // ЗАПОЛНЕНИЕ МАТРИЦЫ ПО СПИРАЛИ
    int num = 1;  // Текущее число для заполнения
    int top = 0, bottom = n - 1;
    int left = 0, right = n - 1;

    while (top <= bottom && left <= right) {
        // Движение вправо по верхней строке
        for (int i = left; i <= right; i++) {
            matrix[top][i] = num++;
        }
        top++;

        // Движение вниз по правому столбцу
        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = num++;
        }
        right--;

        // Движение влево по нижней строке (если есть)
        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = num++;
            }
            bottom--;
        }

        // Движение вверх по левому столбцу (если есть)
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = num++;
            }
            left++;
        }
    }

    // ВЫВОД МАТРИЦЫ
    // Определяем максимальную ширину числа для выравнивания
    int maxNum = n * n;
    int width = 0;
    while (maxNum > 0) {
        width++;
        maxNum /= 10;
    }

    cout << "\nМатрица " << n << "×" << n << ":\n" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(width + 1) << matrix[i][j];
        }
        cout << endl;
    }


    return 0;
}