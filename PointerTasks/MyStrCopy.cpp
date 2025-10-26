#include <iostream>

void myStrCopy(char* dest, const char* src) {
    if (dest == nullptr || src == nullptr) {
        return;
    }
    
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    
    dest[i] = '\0';
}

int StrCopy() {
    char dest1[50];
    const char* src1 = "Hello, World!";
    
    myStrCopy(dest1, src1);
    std::cout << "Исходная строка: \"" << src1 << "\"" << std::endl;
    std::cout << "Скопированная строка: \"" << dest1 << "\"" << std::endl;
    std::cout << std::endl;
    
    char dest2[50];
    const char* src2 = "";
    
    myStrCopy(dest2, src2);
    std::cout << "Исходная строка: \"" << src2 << "\"" << std::endl;
    std::cout << "Скопированная строка: \"" << dest2 << "\"" << std::endl;
    std::cout << std::endl;
    
    char dest3[50];
    const char* src3 = "C++ Programming Language";
    
    myStrCopy(dest3, src3);
    std::cout << "Исходная строка: \"" << src3 << "\"" << std::endl;
    std::cout << "Скопированная строка: \"" << dest3 << "\"" << std::endl;
    std::cout << std::endl;
    
    return 0;
}