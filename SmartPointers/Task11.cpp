#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>

int Task11() {
    int N;
    std::cout << "Введите количество измерений N: ";
    std::cin >> N;
    
    std::vector<double> signals(N);
    std::cout << "Введите " << N << " значений температур: ";
    for (int i = 0; i < N; ++i) {
        std::cin >> signals[i];
    }
    
    // Вывод исходных данных
    std::cout << "Исходные данные: ";
    for (const auto& val : signals) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // Создание динамического массива и копирование значений
    std::unique_ptr<double[]> signalsArray = std::make_unique<double[]>(N);
    for (int i = 0; i < N; ++i) {
        signalsArray[i] = signals[i];
    }
    
    // Лямбда-функция для вычисления среднего значения
    auto mean = [](std::unique_ptr<double[]>& arr, int size) -> double {
        double sum = 0.0;
        for (int i = 0; i < size; ++i) {
            sum += arr[i];
        }
        return sum / size;
    };
    
    // Лямбда-функция для нормализации
    auto normalize = [](std::unique_ptr<double[]>& arr, int size, double maxVal) {
        for (int i = 0; i < size; ++i) {
            arr[i] /= maxVal;
        }
    };
    
    // Лямбда-функция для копирования обратно в вектор
    auto copyBack = [](std::unique_ptr<double[]>& arr, std::vector<double>& vec, int size) {
        for (int i = 0; i < size; ++i) {
            vec[i] = arr[i];
        }
    };
    
    // Вычисление среднего значения
    double average = mean(signalsArray, N);
    std::cout << "Среднее значение: " << std::fixed << std::setprecision(2) << average << std::endl;
    
    // Нахождение максимального значения для нормализации
    double maxSignal = *std::max_element(signals.begin(), signals.end());
    
    // Нормализация массива
    normalize(signalsArray, N, maxSignal);
    
    // Копирование обратно в вектор
    copyBack(signalsArray, signals, N);
    
    // Вывод нормализованных сигналов
    std::cout << "Нормализованные сигналы: ";
    for (const auto& val : signals) {
        std::cout << std::fixed << std::setprecision(3) << val << " ";
    }
    std::cout << std::endl;
    
    return 0;
}