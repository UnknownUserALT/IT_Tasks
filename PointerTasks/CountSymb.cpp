#include <iostream>

// Компактная версия
int countChar(const char* str, char ch) {
    int count = 0;
    if (!str) return 0;
    
    for (const char* p = str; *p; p++) {
        if (*p == ch) count++;
    }
    
    return count;
}

int CountSymb() {
    char text[256];
    char symbol;
    
    std::cout << "Введите текст: ";
    std::cin.getline(text, 256);
    
    std::cout << "Введите символ для подсчёта: ";
    std::cin >> symbol;
    
    std::cout << "Результат: " << countChar(text, symbol) << " вхождений" << std::endl;
    
    return 0;
}