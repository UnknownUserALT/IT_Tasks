#include <iostream>

bool isPalindrome(const char* str) {
    if (str == nullptr) {
        return false;
    }
    
    if (*str == '\0') {
        return true;
    }
    
    const char* start = str;
    const char* end = str;
    
    while (*end != '\0') {
        end++;
    }
    end--;
    
    while (start < end) {
        if (*start != *end) {
            return false;
        }
        start++;
        end--;
    }
    
    return true;
}

int CheckPolyndrom() {
    const int BUFFER_SIZE = 256;
    char inputString[BUFFER_SIZE];
    
    std::cout << "Введите строку для проверки на палиндром: ";
    std::cin.getline(inputString, BUFFER_SIZE);
    
    bool result = isPalindrome(inputString);
    
    std::cout << "Строка \"" << inputString << "\" ";
    if (result) {
        std::cout << "является палиндромом" << std::endl;
    } else {
        std::cout << "НЕ является палиндромом" << std::endl;
    }
    
    return 0;
}