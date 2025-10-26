#include <iostream>

int myStrLen(const char* str) {
    if (str == nullptr) {
        return 0;
    }
    
    int length = 0;
    
    while (str[length] != '\0') {
        length++;
    }
    
    return length;
}

int StrLen() {
    // Тестирование функции
    const char* testString1 = "Hello, World!";
    const char* testString2 = "";
    const char* testString3 = "C++ Programming";
    
    std::cout << "Длина строки \"" << testString1 << "\": " 
              << myStrLen(testString1) << std::endl;
    
    std::cout << "Длина строки \"" << testString2 << "\": " 
              << myStrLen(testString2) << std::endl;
    
    std::cout << "Длина строки \"" << testString3 << "\": " 
              << myStrLen(testString3) << std::endl;
    
    const char* nullString = nullptr;
    std::cout << "Длина нулевого указателя: " 
              << myStrLen(nullString) << std::endl;
    
    return 0;
}