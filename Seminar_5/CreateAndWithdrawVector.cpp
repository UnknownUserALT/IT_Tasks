#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
int CreateAndWithdrawVector() {
    setlocale(LC_ALL, "ru");
    
    // Создание вектора целых чисел из 10 элементов
    std::vector<int> numbers(10);
    
    // Заполнение вектора числами от 1 до 10
    for (int i = 0; i < 10; ++i) {
        numbers[i] = i + 1;
    }
    
    // Вывод вектора с помощью цикла for_each и лямбда-функции
    std::cout << "Вектор чисел: ";
    std::for_each(numbers.begin(), numbers.end(), [](int num) {
        std::cout << num << " ";
    });
    std::cout << std::endl;
    
    return 0;
}