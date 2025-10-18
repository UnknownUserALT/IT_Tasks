#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;
int CesarAlgorithm() {
    setlocale(LC_ALL, "ru");

    char text[100];
    int shift;

    cout << "Введите текст: ";
    cin.getline(text, 100);

    cout << "Введите сдвиг: ";
    cin >> shift;

    // Обрабатываем каждый символ строки
    for (int i = 0; text[i] != '\0'; i++) {
        if (isupper(text[i])) {
            // Для заглавных букв
            text[i] = (text[i] - 'A' + shift) % 26 + 'A';
            if (text[i] < 'A') {
                text[i] += 26; // Коррекция для отрицательного сдвига
            }
        } else if (islower(text[i])) {
            // Для строчных букв
            text[i] = (text[i] - 'a' + shift) % 26 + 'a';
            if (text[i] < 'a') {
                text[i] += 26; // Коррекция для отрицательного сдвига
            }
        }
        // Остальные символы (пробелы, знаки препинания) не изменяются
    }

    cout << "Зашифрованный текст: " << text << endl;

    return 0;
}