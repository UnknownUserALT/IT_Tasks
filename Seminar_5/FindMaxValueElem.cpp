#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
int FindMaxValueElem() {
    setlocale(LC_ALL, "ru");
    // Создание вектора из 20 чисел
    std::vector<int> numbers = {15, 8, 23, 4, 42, 19, 7, 31, 11, 28,
                                3, 37, 14, 25, 9, 33, 6, 41, 17, 22};

    // Вывод исходного вектора
    std::cout << "Исходный вектор: ";
    std::for_each(numbers.begin(), numbers.end(), [](int num) {
        std::cout << num << " ";
    });
    std::cout << std::endl;

    // Поиск максимального элемента с помощью std::max_element и лямбды
    auto max_element = std::max_element(numbers.begin(), numbers.end(),
        [](int a, int b) {
            return a < b;  // Лямбда для сравнения элементов
        });

    // Вывод результата
    std::cout << "Максимальный элемент: " << *max_element << std::endl;
    std::cout << "Позиция в векторе: " << std::distance(numbers.begin(), max_element) << std::endl;

    return 0;
}