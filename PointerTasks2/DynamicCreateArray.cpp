#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;
void freeMemory(int **arr, int n) {
    for (int i = 0; i < n; i++) {
        delete[] arr[i];
        arr[i] = nullptr;
    }

    delete[] arr;
}

int **transpose(int **matrix, int n, int m) {
    int **result = new int*[m];
    for (int i = 0; i < m; i++) {
        result[i] = new int[n];
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = matrix[j][i];
        }
    }

    return result;
}
int rowSum(int *row, int m) {
    int sum = 0;
    for (int j = 0; j < m; j++) {
        sum += row[j];
    }
    return sum;
}
void sortRowsBySum(int **arr, int n, int m) {
    // Создаем массив для хранения сумм строк
    int *sums = new int[n];
    for (int i = 0; i < n; i++) {
        sums[i] = rowSum(arr[i], m);
    }

    // Пузырьковая сортировка
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (sums[j] > sums[j + 1]) {
                // Меняем местами указатели на строки
                int *tempPtr = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tempPtr;

                // Меняем местами суммы
                int tempSum = sums[j];
                sums[j] = sums[j + 1];
                sums[j + 1] = tempSum;
            }
        }
    }

    delete[] sums;
}

void printArray(int **arr, int n, int m, const string& name = "Массив") {
    cout << name << " (" << n << "x" << m << "):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}
void printRowSums(int **arr, int n, int m) {
    cout << "Суммы строк:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Строка " << i << ": сумма = " << rowSum(arr[i], m) << endl;
    }
    cout << endl;
}


int DynamicCreateArray() {
    int n, m;

    cout << "Введите n и m: ";
    cin >> n >> m;

    if (n <= 0 || m <= 0) {
        cout << "Ошибка: размеры должны быть положительными!" << endl;
        return 1;
    }

    int** arr = new int*[n];
    for (int i = 0; i < n; i++) {
        arr[i] = new int[m];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr[i][j] = rand() % 100;
        }
    }

    // Выводим исходную матрицу
    cout << "Исходная матрица (" << n << "x" << m << "):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }

    // Транспонируем матрицу
    int **transposed = transpose(arr, n, m);

    // Выводим транспонированную матрицу
    cout << "\nТранспонированная матрица (" << m << "x" << n << "):" << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << transposed[i][j] << "\t";
        }
        cout << endl;
    }

    // Вывод исходного массива
    cout << "\n=== ДО сортировки ===" << endl;
    printArray(arr, n, m, "Исходный массив");
    printRowSums(arr, n, m);

    // Сортировка строк по сумме элементов
    sortRowsBySum(arr, n, m);

    // Вывод отсортированного массива
    cout << "=== ПОСЛЕ сортировки ===" << endl;
    printArray(arr, n, m, "Отсортированный массив");
    printRowSums(arr, n, m);


    // Освобождение памяти
    freeMemory(arr, n);
    freeMemory(transposed, m);

    return 0;
}

