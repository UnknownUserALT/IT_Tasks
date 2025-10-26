#include <iostream>

void myStrCat(char* dest, const char* src) {
    if (dest == nullptr || src == nullptr) {
        return;
    }
    
    int i = 0;
    while (dest[i] != '\0') {
        i++;
    }
    
    int j = 0;
    while (src[j] != '\0') {
        dest[i] = src[j];
        i++;
        j++;
    }
    
    dest[i] = '\0';
}

int ConcatStr() {
    char dest1[50] = "Hello, ";
    const char* src1 = "World!";
    
    std::cout << "До конкатенации: \"" << dest1 << "\"" << std::endl;
    myStrCat(dest1, src1);
    std::cout << "После конкатенации: \"" << dest1 << "\"" << std::endl;
    std::cout << std::endl;
    
    char dest2[50] = "C++ ";
    const char* src2 = "";
    
    std::cout << "До конкатенации: \"" << dest2 << "\"" << std::endl;
    myStrCat(dest2, src2);
    std::cout << "После конкатенации: \"" << dest2 << "\"" << std::endl;
    std::cout << std::endl;
    
    char dest3[50] = "";
    const char* src3 = "Programming";
    
    std::cout << "До конкатенации: \"" << dest3 << "\"" << std::endl;
    myStrCat(dest3, src3);
    std::cout << "После конкатенации: \"" << dest3 << "\"" << std::endl;
    std::cout << std::endl;
    
    char dest4[100] = "Start";
    myStrCat(dest4, " -> ");
    myStrCat(dest4, "Middle");
    myStrCat(dest4, " -> ");
    myStrCat(dest4, "End");
    
    std::cout << "Результат цепочки конкатенаций: \"" << dest4 << "\"" << std::endl;
    
    return 0;
}