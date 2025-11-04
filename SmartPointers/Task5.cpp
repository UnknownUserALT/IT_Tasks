#include <iostream>
#include <memory>
#include <iomanip>

int Task5() {
    int N, M;
    
    // Ввод размеров таблицы
    std::cout << "Введите количество строк N: ";
    std::cin >> N;
    std::cout << "Введите количество столбцов M: ";
    std::cin >> M;
    
    if (N <= 0 || M <= 0) {
        std::cout << "Ошибка: размеры должны быть положительными!" << std::endl;
        return 1;
    }
    
    // Создание одномерного массива для хранения двумерной таблицы
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(N * M);
    
    // Заполнение таблицы произведениями индексов
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            arr[i * M + j] = i * j;
        }
    }
    
    // Вывод результата в виде таблицы
    std::cout << "\nТаблица произведений индексов:" << std::endl;
    
    // Вывод заголовка столбцов
    std::cout << std::setw(4) << " ";
    for (int j = 0; j < M; ++j) {
        std::cout << std::setw(4) << j;
    }
    std::cout << std::endl;
    
    // Вывод разделительной линии
    std::cout << std::setw(4) << " ";
    for (int j = 0; j < M; ++j) {
        std::cout << "----";
    }
    std::cout << std::endl;
    
    // Вывод данных таблицы
    for (int i = 0; i < N; ++i) {
        std::cout << std::setw(2) << i << " |";
        for (int j = 0; j < M; ++j) {
            std::cout << std::setw(4) << arr[i * M + j];
        }
        std::cout << std::endl;
    }
    
    return 0;
}