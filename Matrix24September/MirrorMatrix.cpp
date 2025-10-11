#include <iostream>
using namespace std;

int mirrorm() {
    setlocale(LC_ALL, "ru");

    int N;
    cout << "Введите размер матрицы N: ";
    cin >> N;

    int A[100][100];

    // Читаем исходную матрицу
    cout << "Введите матрицу " << N << "x" << N << " (построчно):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> A[i][j];
        }
    }

    // Выводим исходную матрицу
    cout << "\nИсходная матрица:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    // Создаем 4 матрицы для преобразований
    int m1[100][100], m2[100][100], m3[100][100], m4[100][100];

    // 1. Отражение относительно главной диагонали (транспонирование)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m1[i][j] = A[j][i];
        }
    }

    // 2. Отражение относительно побочной диагонали
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m2[i][j] = A[N - 1 - j][N - 1 - i];
        }
    }

    // 3. Зеркальное отражение относительно вертикальной оси
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m3[i][j] = A[i][N - 1 - j];
        }
    }

    // 4. Зеркальное отражение относительно горизонтальной оси
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m4[i][j] = A[N - 1 - i][j];
        }
    }

    // Выводим все 4 матрицы
    cout << "\n=== РЕЗУЛЬТАТЫ ПРЕОБРАЗОВАНИЙ ===" << endl;

    cout << "\n1. Отражение относительно главной диагонали:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << m1[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\n2. Отражение относительно побочной диагонали:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << m2[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\n3. Зеркальное отражение относительно вертикальной оси:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << m3[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\n4. Зеркальное отражение относительно горизонтальной оси:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << m4[i][j] << " ";
        }
        cout << endl;
    }

    // Проверяем совпадения матриц
    cout << "\n=== СОВПАДАЮЩИЕ МАТРИЦЫ ===" << endl;
    bool found = false;

    // Проверяем все пары (i,j) где i < j
    for (int i = 1; i <= 4; i++) {
        for (int j = i + 1; j <= 4; j++) {
            bool equal = true;

            // Сравниваем соответствующие матрицы
            int (*mat1)[100], (*mat2)[100];

            if (i == 1) mat1 = m1;
            else if (i == 2) mat1 = m2;
            else if (i == 3) mat1 = m3;
            else mat1 = m4;

            if (j == 1) mat2 = m1;
            else if (j == 2) mat2 = m2;
            else if (j == 3) mat2 = m3;
            else mat2 = m4;

            // Поэлементное сравнение
            for (int x = 0; x < N && equal; x++) {
                for (int y = 0; y < N && equal; y++) {
                    if (mat1[x][y] != mat2[x][y]) {
                        equal = false;
                    }
                }
            }

            if (equal) {
                cout << "Матрицы " << i << " и " << j << " совпадают" << endl;
                found = true;
            }
        }
    }

    if (!found) {
        cout << "NONE (совпадающих матриц нет)" << endl;
    }

    return 0;
}