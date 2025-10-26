#include <iostream>

void removeSpaces(char* str) {
    if (!str) return;
    
    char* w = str;
    for (char* r = str; *r; r++) {
        if (*r != ' ') *w++ = *r;
    }
    *w = '\0';
}

int DeleteSpacesFromStr() {
    char text[256];
    
    std::cout << "Введите строку: ";
    std::cin.getline(text, 256);
    
    std::cout << "До: \"" << text << "\"" << std::endl;
    removeSpaces(text);
    std::cout << "После: \"" << text << "\"" << std::endl;
    
    return 0;
}