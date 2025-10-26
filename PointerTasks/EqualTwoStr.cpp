#include <iostream>

int myStrCmp(const char* s1, const char* s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;

    while (*s1 && *s1 == *s2) { s1++; s2++; }
    return *s1 - *s2;
}

int EqualTwoStr() {
    char s1[256], s2[256];

    std::cout << "Строка 1: ";
    std::cin.getline(s1, 256);

    std::cout << "Строка 2: ";
    std::cin.getline(s2, 256);

    int res = myStrCmp(s1, s2);
    std::cout << "Результат: " << res << " ("
              << (res == 0 ? "равны" : res < 0 ? "s1 < s2" : "s1 > s2")
              << ")" << std::endl;

    return 0;
}