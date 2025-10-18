#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;
int CheckAllSymbolStringSameAnother() {
    setlocale(LC_ALL, "ru");

    char A[100], B[100];

    cout << "Введите строку A: ";
    cin.getline(A, 100);

    cout << "Введите строку B: ";
    cin.getline(B, 100);

    bool allFound = true;

    // Проверяем каждый символ строки B
    for (int i = 0; B[i] != '\0'; i++) {
        bool found = false;

        // Ищем текущий символ B в строке A
        for (int j = 0; A[j] != '\0'; j++) {
            if (B[i] == A[j]) {
                found = true;
                break;
            }
        }

        // Если символ не найден, устанавливаем флаг и прерываем цикл
        if (!found) {
            allFound = false;
            break;
        }
    }

    if (allFound) {
        cout << "Да" << endl;
    } else {
        cout << "Нет" << endl;
    }

    return 0;
}