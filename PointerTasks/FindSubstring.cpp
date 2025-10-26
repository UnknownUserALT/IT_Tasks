#include <iostream>

const char* findSubstring(const char* text, const char* word) {
    if (text == nullptr || word == nullptr) {
        return nullptr;
    }
    
    if (*word == '\0') {
        return text;
    }
    
    const char* textPtr = text;
    while (*textPtr != '\0') {
        const char* textCurrent = textPtr;
        const char* wordCurrent = word;
        
        while (*wordCurrent != '\0' && *textCurrent != '\0' &&
               *textCurrent == *wordCurrent) {
            textCurrent++;
            wordCurrent++;
        }
        
        if (*wordCurrent == '\0') {
            return textPtr;
        }
        
        textPtr++;
    }
    
    return nullptr;
}

int FindSubString() {
    const int BUFFER_SIZE = 256;
    char text[BUFFER_SIZE];
    char word[BUFFER_SIZE];
    
    std::cout << "Введите текст: ";
    std::cin.getline(text, BUFFER_SIZE);
    
    std::cout << "Введите подстроку для поиска: ";
    std::cin.getline(word, BUFFER_SIZE);
    
    const char* result = findSubstring(text, word);
    
    if (result != nullptr) {
        std::cout << "Подстрока \"" << word << "\" найдена в позиции: " 
                  << (result - text) << std::endl;
    } else {
        std::cout << "Подстрока \"" << word << "\" не найдена в тексте" << std::endl;
    }
    
    return 0;
}