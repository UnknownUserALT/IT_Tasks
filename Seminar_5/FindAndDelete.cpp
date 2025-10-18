#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
int FindAndDelete() {
    setlocale(LC_ALL, "ru");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 15); // Маленький диапазон для повторений

    // Создание и заполнение вектора из 20 чисел
    std::vector<int> numbers(20);
    std::generate(numbers.begin(), numbers.end(), [&]() { return dis(gen); });

    // Вывод исходного вектора
    std::cout << "Исходный вектор (20 чисел): ";
    for (int num : numbers) std::cout << num << " ";
    std::cout << std::endl << std::endl;

    // 1. Проверка наличия числа 7 с помощью find
    auto found = std::find(numbers.begin(), numbers.end(), 7);
    if (found != numbers.end()) {
        std::cout << "Число 7 найдено на позиции: " << std::distance(numbers.begin(), found) << std::endl;
    } else {
        std::cout << "Число 7 не найдено в векторе" << std::endl;
    }

    // 2. Удаление всех повторяющихся чисел (sort + unique)
    // Сначала сортируем вектор
    std::sort(numbers.begin(), numbers.end());

    std::cout << "После сортировки: ";
    for (int num : numbers) std::cout << num << " ";
    std::cout << std::endl;

    // Затем используем unique для перемещения дубликатов в конец
    auto last = std::unique(numbers.begin(), numbers.end());

    // Удаляем "недействительные" элементы
    numbers.erase(last, numbers.end());

    // 3. Вывод оставшихся элементов
    std::cout << "После удаления дубликатов: ";
    for (int num : numbers) std::cout << num << " ";
    std::cout << std::endl;
    std::cout << "Количество уникальных элементов: " << numbers.size() << std::endl;

    return 0;
}