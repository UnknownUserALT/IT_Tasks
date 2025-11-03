#include <iostream>
#include <iomanip>

using namespace std;

// Функция для выделения памяти под пластину
double **allocatePlate(int n, int m) {
    double **plate = new double*[n];
    for (int i = 0; i < n; i++) {
        plate[i] = new double[m];
    }
    return plate;
}

// Функция для освобождения памяти
void freePlate(double **plate, int n) {
    if (plate == nullptr) return;
    
    for (int i = 0; i < n; i++) {
        delete[] plate[i];
    }
    delete[] plate;
}

// Функция для инициализации пластины
void initializePlate(double **plate, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            plate[i][j] = 20.0;
        }
    }
    
    int center_i = n / 2;
    int center_j = m / 2;
    plate[center_i][center_j] = 100.0;
}

// Функция для обновления температур
void updateTemperature(double **plate, int n, int m) {
    double **newPlate = allocatePlate(n, m);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            newPlate[i][j] = plate[i][j];
        }
    }
    
    int center_i = n / 2;
    int center_j = m / 2;
    
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            if (i == center_i && j == center_j) {
                continue;
            }
            
            double sum = *(*(plate + i - 1) + j) +  // верхний сосед
                         *(*(plate + i + 1) + j) +  // нижний сосед
                         *(*(plate + i) + j - 1) +  // левый сосед
                         *(*(plate + i) + j + 1);   // правый сосед
            
            *(*(newPlate + i) + j) = sum / 4.0;
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            *(*(plate + i) + j) = *(*(newPlate + i) + j);
        }
    }
    
    // Восстанавливаем температуру нагревателя
    *(*(plate + center_i) + center_j) = 100.0;
    
    // Освобождаем временную память
    freePlate(newPlate, n);
}

void printPlate(double **plate, int n, int m, const string& title = "Температурное распределение:") {
    cout << title << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << fixed << setprecision(1) << setw(6) << *(*(plate + i) + j);
        }
        cout << endl;
    }
    cout << endl;
}

void simulateHeatTransfer(int n, int m, int iterations) {
    cout << "=== МОДЕЛИРОВАНИЕ ТЕПЛОПЕРЕДАЧИ ===" << endl;
    cout << "Размер пластины: " << n << "x" << m << endl;
    cout << "Количество итераций: " << iterations << endl;
    cout << "Начальная температура: 20.0°C" << endl;
    cout << "Температура нагревателя: 100.0°C" << endl << endl;
    
    double **plate = allocatePlate(n, m);
    initializePlate(plate, n, m);
    
    cout << "Начальное состояние:" << endl;
    printPlate(plate, n, m);
    
    for (int iter = 1; iter <= iterations; iter++) {
        updateTemperature(plate, n, m);
        
        if (iter <= 3 || iter == iterations) {
            cout << "После " << iter << " итерации:" << endl;
            printPlate(plate, n, m);
        }
    }
    
    // Освобождаем память
    freePlate(plate, n);
}

int DynamicManageMeshStruct() {
    const int N = 7;  // строки
    const int M = 7;  // столбцы
    const int K = 10; // итераций
    
    simulateHeatTransfer(N, M, K);
    
    return 0;
}