#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
int DeleteNegativeNumbers() {
    setlocale(LC_ALL, "ru");
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-50, 50);
    
    // Создание и заполнение вектора случайными числами
    std::vector<int> numbers(15);
    std::generate(numbers.begin(), numbers.end(), [&]() { return dis(gen); });
    
    // Вывод исходного вектора
    std::cout << "Исходный вектор: ";
    std::for_each(numbers.begin(), numbers.end(), [](int num) {
        std::cout << num << " ";
    });
    std::cout << std::endl;
    
    // Удаление отрицательных чисел с помощью std::remove_if и лямбды
    auto new_end = std::remove_if(numbers.begin(), numbers.end(), 
        [](int num) {
            return num < 0;  // Лямбда возвращает true для отрицательных чисел
        });
    
    // Удаление "недействительных" элементов из вектора
    numbers.erase(new_end, numbers.end());
    
    // Вывод результата
    std::cout << "Вектор после удаления отрицательных чисел: ";
    std::for_each(numbers.begin(), numbers.end(), [](int num) {
        std::cout << num << " ";
    });
    std::cout << std::endl;
    std::cout << "Размер вектора: " << numbers.size() << std::endl;
    
    return 0;
}