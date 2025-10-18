#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
int StatsByData() {
    setlocale(LC_ALL, "ru");
    
       // Генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    // Создание и заполнение вектора из 20 чисел
    std::vector<int> numbers(20);
    std::generate(numbers.begin(), numbers.end(), [&]() { return dis(gen); });
    
    // Вывод исходного вектора
    std::cout << "Исходный вектор: ";
    for (int num : numbers) std::cout << num << " ";
    std::cout << std::endl << std::endl;
    
    // 1. Нахождение минимального и максимального значения с помощью minmax_element
    auto minmax = std::minmax_element(numbers.begin(), numbers.end());
    std::cout << "Минимальное значение: " << *minmax.first << std::endl;
    std::cout << "Максимальное значение: " << *minmax.second << std::endl;
    
    // 2. Вычисление среднего значения с помощью accumulate
    double average = std::accumulate(numbers.begin(), numbers.end(), 0.0) / numbers.size();
    std::cout << "Среднее значение: " << average << std::endl;
    
    // 3. Нахождение медианы
    // Создаем копию для сортировки (чтобы не изменять исходный вектор)
    std::vector<int> sorted_numbers = numbers;
    std::sort(sorted_numbers.begin(), sorted_numbers.end());
    
    double median;
    if (sorted_numbers.size() % 2 == 0) {
        // Четное количество элементов - медиана это среднее двух центральных
        int mid1 = sorted_numbers[sorted_numbers.size() / 2 - 1];
        int mid2 = sorted_numbers[sorted_numbers.size() / 2];
        median = (mid1 + mid2) / 2.0;
    } else {
        // Нечетное количество элементов - медиана это центральный элемент
        median = sorted_numbers[sorted_numbers.size() / 2];
    }
    
    std::cout << "Медиана: " << median << std::endl;
    
    // Дополнительно: вывод отсортированного вектора
    std::cout << std::endl << "Отсортированный вектор: ";
    for (int num : sorted_numbers) std::cout << num << " ";
    std::cout << std::endl;
    
    return 0;
}