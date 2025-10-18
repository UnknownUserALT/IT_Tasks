#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;
int CountUniqSymbols() {
    setlocale(LC_ALL, "ru");

    char str[100];
    cout << "Введите строку: ";
    cin.getline(str, 100);

    int length = strlen(str);
    int uniqueCount = 0;

    cout << "Уникальные символы: ";

    for (int i = 0; i < length; i++) {
        bool isUnique = true;

        // Проверяем, встречался ли текущий символ ранее
        for (int j = 0; j < i; j++) {
            if (str[i] == str[j]) {
                isUnique = false;
                break;
            }
        }

        // Если символ уникальный, увеличиваем счетчик и выводим его
        if (isUnique) {
            uniqueCount++;
            cout << str[i];
            if (uniqueCount > 0 && i < length - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl << "Количество уникальных символов: " << uniqueCount << endl;

    return 0;
}