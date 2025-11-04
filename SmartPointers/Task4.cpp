#include <iostream>
#include <memory>

int Task4() {
    // Создание умного указателя и инициализация значением 42
    std::unique_ptr<int> ptr1 = std::make_unique<int>(42);
    
    // Вывод исходного состояния
    std::cout << "Исходное состояние:" << std::endl;
    std::cout << "ptr1: " << (ptr1 ? "указывает на значение" : "nullptr") << std::endl;
    if (ptr1) {
        std::cout << "Значение ptr1: " << *ptr1 << std::endl;
    }
    
    // Передача владения из ptr1 в ptr2 через std::move
    std::unique_ptr<int> ptr2 = std::move(ptr1);
    
    // Проверка состояния после передачи владения
    std::cout << "\nПосле передачи владения через std::move:" << std::endl;
    
    // Проверка ptr1 (должен быть nullptr)
    std::cout << "ptr1: " << (ptr1 ? "указывает на значение" : "nullptr") << std::endl;
    
    // Проверка ptr2 (должен хранить значение 42)
    std::cout << "ptr2: " << (ptr2 ? "указывает на значение" : "nullptr") << std::endl;
    if (ptr2) {
        std::cout << "Значение ptr2: " << *ptr2 << std::endl;
    }
    
    return 0;
}