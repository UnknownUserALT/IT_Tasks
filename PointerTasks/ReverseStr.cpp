#include <iostream>

void reverse(char* str) {
    if (str == nullptr || *str == '\0') {
        return;
    }
    
    char* start = str;
    char* end = str;
    
    while (*end != '\0') {
        end++;
    }
    end--;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        
        start++;
        end--;
    }
}

int ReverseStr() {
    const int BUFFER_SIZE = 256;
    char inputString[BUFFER_SIZE];
    
    std::cout << "Введите строку: ";
    std::cin.getline(inputString, BUFFER_SIZE);
    
    std::cout << "Исходная строка: \"" << inputString << "\"" << std::endl;
    
    reverse(inputString);
    
    std::cout << "Перевёрнутая строка: \"" << inputString << "\"" << std::endl;
    
    return 0;
}