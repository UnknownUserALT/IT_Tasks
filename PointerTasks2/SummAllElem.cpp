#include <iostream>

using namespace std;

int sumElementsFunc(int **arr, int n, int m) {
    int total = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            total += *(*(arr + i) + j);
        }
    }
    
    return total;
}

int sumElements() {
    int n = 3, m = 5;
    
    // Создание массива
    int **matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[m];
    }
    
    // Заполнение через указатели
    int value = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            *(*(matrix + i) + j) = value++;
        }
    }
    
    // Вывод через указатели
    cout << "Массив:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << *(*(matrix + i) + j) << " ";
        }
        cout << endl;
    }
    
    // Вычисление суммы
    int result = sumElementsFunc(matrix, n, m);
    cout << "\nСумма элементов: " << result << endl;
    
    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    
    return 0;
}