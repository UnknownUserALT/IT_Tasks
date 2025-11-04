#include <iostream>
#include <memory>

int Task1() {
    // Создание умного указателя с инициализацией значения 10
    std::unique_ptr<int> ptr = std::make_unique<int>(10);
    
    // Вывод начального значения
    std::cout << "Начальное значение: " << *ptr << std::endl;
    
    // Изменение значения на 25 и вывод
    *ptr = 25;
    std::cout << "Измененное значение: " << *ptr << std::endl;
    
    return 0;
}