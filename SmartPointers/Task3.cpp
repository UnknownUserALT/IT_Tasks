#include <iostream>
#include <memory>

// Функция для заполнения массива квадратами индексов
void fillArray(std::unique_ptr<int[]>& arr, int n) {
    for (int i = 0; i < n; ++i) {
        arr[i] = i * i;
    }
}

int Task3() {
    int n;

    // Ввод размера массива
    std::cout << "Введите размер массива: ";
    std::cin >> n;

    if (n <= 0) {
        std::cout << "Ошибка: размер массива должен быть положительным!" << std::endl;
        return 1;
    }

    // Создание динамического массива
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(n);

    // Передача unique_ptr по ссылке в функцию для заполнения
    fillArray(arr, n);

    // Вывод значений массива
    std::cout << "Массив квадратов: ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}