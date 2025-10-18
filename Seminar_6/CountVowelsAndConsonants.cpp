#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;
int CountVowelsAndConsonants() {
    setlocale(LC_ALL, "ru");

    char str[100];
    cout << "Введите строку: ";
    cin.getline(str, 100);

    int vowels = 0;
    int consonants = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower(str[i]);

        // Проверяем, является ли символ буквой
        if (isalpha(c)) {
            // Проверяем, является ли буква гласной
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y') {
                vowels++;
            } else {
                consonants++;
            }
        }
    }

    cout << "Гласных: " << vowels << endl;
    cout << "Согласных: " << consonants << endl;

    return 0;
}