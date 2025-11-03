#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

// Функция для генерации случайного числа от -1.0 до 1.0
double randomWeight() {
    return (rand() / (double)RAND_MAX) * 2.0 - 1.0;
}

// Функция для выделения памяти под матрицу весов
double **allocateWeights(int n, int m) {
    double **weights = new double*[n];
    for (int i = 0; i < n; i++) {
        weights[i] = new double[m];
    }
    return weights;
}

// Функция для освобождения памяти
void freeWeights(double **weights, int n) {
    if (weights == nullptr) return;
    
    for (int i = 0; i < n; i++) {
        delete[] weights[i];
    }
    delete[] weights;
}

// Функция для инициализации весов случайными значениями
void initializeWeights(double **weights, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            weights[i][j] = randomWeight();
        }
    }
}

// Функция для нормализации весов (сумма в каждой строке <= 1)
void normalizeWeights(double **weights, int n, int m) {
    for (int i = 0; i < n; i++) {
        // Вычисляем сумму абсолютных значений весов в строке
        double sum = 0.0;
        for (int j = 0; j < m; j++) {
            sum += fabs(weights[i][j]);
        }
        
        // Если сумма больше 1, нормализуем веса
        if (sum > 1.0) {
            for (int j = 0; j < m; j++) {
                weights[i][j] /= sum;
            }
        }
    }
}

// Функция прямого распространения (forward pass)
double *forward(double **weights, double *inputs, int n, int m) {
    double *outputs = new double[n];
    
    for (int i = 0; i < n; i++) {
        outputs[i] = 0.0;
        for (int j = 0; j < m; j++) {
            // Вычисляем: y_i = Σ(w_ij * x_j)
            outputs[i] += weights[i][j] * inputs[j];
        }
    }
    
    return outputs;
}

// Функция для вывода матрицы весов
void printWeights(double **weights, int n, int m, const string& title = "Матрица весов:") {
    cout << title << endl;
    for (int i = 0; i < n; i++) {
        cout << "Нейрон " << i << ": ";
        for (int j = 0; j < m; j++) {
            cout << fixed << setprecision(3) << setw(8) << weights[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

// Функция для вывода вектора
void printVector(double *vector, int size, const string& title = "Вектор:") {
    cout << title << endl;
    for (int i = 0; i < size; i++) {
        cout << fixed << setprecision(3) << setw(8) << vector[i];
    }
    cout << endl << endl;
}

// Функция для проверки суммы весов в каждой строке
void checkWeightSums(double **weights, int n, int m) {
    cout << "Проверка сумм весов по строкам:" << endl;
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < m; j++) {
            sum += fabs(weights[i][j]);
        }
        cout << "Нейрон " << i << ": сумма = " << fixed << setprecision(3) << sum;
        if (sum > 1.0) {
            cout << " (требуется нормализация)";
        }
        cout << endl;
    }
    cout << endl;
}

int DynamicStructNeuralLayer() {
    srand(time(0));
    
    // Параметры нейронного слоя
    const int N = 4;  // количество нейронов
    const int M = 5;  // количество входов
    
    cout << "=== МОДЕЛЬ НЕЙРОННОГО СЛОЯ ===" << endl;
    cout << "Количество нейронов: " << N << endl;
    cout << "Количество входов: " << M << endl << endl;
    
    // 1. Создание и инициализация матрицы весов
    double **weights = allocateWeights(N, M);
    initializeWeights(weights, N, M);
    
    cout << "ДО нормализации:" << endl;
    printWeights(weights, N, M);
    checkWeightSums(weights, N, M);
    
    // 2. Нормализация весов
    normalizeWeights(weights, N, M);
    
    cout << "ПОСЛЕ нормализации:" << endl;
    printWeights(weights, N, M);
    checkWeightSums(weights, N, M);
    
    // 3. Создание входного вектора
    double *inputs = new double[M];
    cout << "Входные сигналы:" << endl;
    for (int j = 0; j < M; j++) {
        inputs[j] = (j + 1) * 0.1;  // примерные значения: 0.1, 0.2, 0.3, ...
        cout << "x" << j << " = " << fixed << setprecision(1) << inputs[j] << endl;
    }
    cout << endl;
    
    // 4. Прямое распространение
    double *outputs = forward(weights, inputs, N, M);
    
    // 5. Вывод результатов
    cout << "=== РЕЗУЛЬТАТЫ ВЫЧИСЛЕНИЙ ===" << endl;
    printVector(inputs, M, "Входной вектор X:");
    printVector(outputs, N, "Выходной вектор Y:");
    
    // Детальный вывод вычислений
    cout << "Детальные вычисления:" << endl;
    cout << "y_i = Σ(w_ij * x_j)" << endl;
    for (int i = 0; i < N; i++) {
        cout << "y" << i << " = ";
        double sum = 0.0;
        for (int j = 0; j < M; j++) {
            double product = weights[i][j] * inputs[j];
            cout << "(" << fixed << setprecision(3) << weights[i][j] 
                 << " * " << inputs[j] << ")";
            sum += product;
            if (j < M - 1) cout << " + ";
        }
        cout << " = " << fixed << setprecision(3) << sum << endl;
    }
    cout << endl;
    
    // 6. Освобождение памяти
    freeWeights(weights, N);
    delete[] inputs;
    delete[] outputs;
    
    cout << "Память успешно освобождена!" << endl;
    
    return 0;
}