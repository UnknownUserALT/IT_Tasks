#include <iostream>
using namespace std;
int saddlepoints() {
    setlocale(LC_ALL, "ru");
    int N, M;
    cout << "Введите количество строк N и столбцов M: ";
    cin >> N >> M;

    int matrix[100][100];

    cout << "Введите матрицу " << N << "x" << M << " (построчно):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> matrix[i][j];
        }
    }

    // Выводим исходную матрицу
    cout << "\nИсходная матрица:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // Создаем массивы для хранения min/max по строкам и столбцам
    int rowMin[100], rowMax[100];
    int colMin[100], colMax[100];

    // Инициализируем массивы
    for (int i = 0; i < N; i++) {
        rowMin[i] = matrix[i][0];
        rowMax[i] = matrix[i][0];
    }

    for (int j = 0; j < M; j++) {
        colMin[j] = matrix[0][j];
        colMax[j] = matrix[0][j];
    }

    // Вычисляем min и max для каждой строки
    cout << "\nВычисляем минимумы и максимумы:" << endl;
    cout << "По строкам:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] < rowMin[i]) {
                rowMin[i] = matrix[i][j];
            }
            if (matrix[i][j] > rowMax[i]) {
                rowMax[i] = matrix[i][j];
            }
        }
        cout << "Строка " << i + 1 << ": min = " << rowMin[i] << ", max = " << rowMax[i] << endl;
    }

    // Вычисляем min и max для каждого столбца
    cout << "\nПо столбцам:" << endl;
    for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {
            if (matrix[i][j] < colMin[j]) {
                colMin[j] = matrix[i][j];
            }
            if (matrix[i][j] > colMax[j]) {
                colMax[j] = matrix[i][j];
            }
        }
        cout << "Столбец " << j + 1 << ": min = " << colMin[j] << ", max = " << colMax[j] << endl;
    }

    // Ищем седловые точки
    bool foundAny = false;

    cout << "\n=== ПОИСК СЕДЛОВЫХ ТОЧЕК ===" << endl;

    // Поиск точек типа A: минимальны в строке и максимальны в столбце
    cout << "\nТип A (min в строке, max в столбце):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] == rowMin[i] && matrix[i][j] == colMax[j]) {
                cout << "A " << matrix[i][j] << " " << i + 1 << " " << j + 1 << endl;
                foundAny = true;
            }
        }
    }

    // Поиск точек типа B: максимальны в строке и минимальны в столбце
    cout << "\nТип B (max в строке, min в столбце):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] == rowMax[i] && matrix[i][j] == colMin[j]) {
                cout << "B " << matrix[i][j] << " " << i + 1 << " " << j + 1 << endl;
                foundAny = true;
            }
        }
    }

    if (!foundAny) {
        cout << "NONE" << endl;
    }

    return 0;
}