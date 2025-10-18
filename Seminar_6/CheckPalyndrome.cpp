#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;
int CheckPalyndrom() {
    setlocale(LC_ALL, "ru");

    char str[100];
    cout << "Введите строку: ";
    cin.getline(str, 100);

    int left = 0;
    int right = strlen(str) - 1;
    bool isPalindrome = true;

    while (left < right) {
        // Пропускаем пробелы слева
        while (left < right && isspace(str[left])) {
            left++;
        }
        // Пропускаем пробелы справа
        while (left < right && isspace(str[right])) {
            right--;
        }

        // Сравниваем символы (игнорируем регистр)
        if (tolower(str[left]) != tolower(str[right])) {
            isPalindrome = false;
            break;
        }

        left++;
        right--;
    }

    if (isPalindrome) {
        cout << "Палиндром" << endl;
    } else {
        cout << "Не палиндром" << endl;
    }

    return 0;
}