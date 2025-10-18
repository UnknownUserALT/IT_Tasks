#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;
int DeleteAllNumberFromString() {
    setlocale(LC_ALL, "ru");

    char str[100];
    cout << "Введите строку: ";
    cin.getline(str, 100);

    int writeIndex = 0;

    for (int readIndex = 0; str[readIndex] != '\0'; readIndex++) {
        // Если символ не цифра - копируем его
        if (!isdigit(str[readIndex])) {
            str[writeIndex] = str[readIndex];
            writeIndex++;
        }
    }

    // Завершаем строку
    str[writeIndex] = '\0';

    cout << "Результат: " << str << endl;

    return 0;
}