#include <iostream>

void replaceSpaces(char* str) {
    if (!str) return;
    while (*str) {
        if (*str == ' ') *str = '_';
        str++;
    }
}

int ReplaceSpaceWith() {
    char text[256];
    
    std::cout << "Введите строку: ";
    std::cin.getline(text, 256);
    
    std::cout << "До: " << text << std::endl;
    replaceSpaces(text);
    std::cout << "После: " << text << std::endl;
    
    return 0;
}