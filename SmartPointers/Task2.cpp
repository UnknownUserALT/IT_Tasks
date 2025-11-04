#include <iostream>
#include <memory>

int Task2() {
    int N;
    
    // Ввод размера массива
    std::cout << "Введите размер массива N: ";
    std::cin >> N;
    
    // Создание динамического массива через std::unique_ptr<int[]>
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(N);
    
    // Заполнение массива числами от 1 до N
    for (int i = 0; i < N; ++i) {
        arr[i] = i + 1;
    }
    
    // Вывод содержимого массива
    std::cout << "Содержимое массива: ";
    for (int i = 0; i < N; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}