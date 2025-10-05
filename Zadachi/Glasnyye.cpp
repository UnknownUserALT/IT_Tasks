#include <iostream>
using namespace std;

int glasn() {
    string input;
    int vowelCount = 0;

    // Ввод строки
    cout << "Введите строку: ";
    cin >> input;

    // Подсчет гласных букв
    for (char c : input) {
        // Приводим символ к нижнему регистру для удобства проверки
        char lowerC = tolower(c);

        // Проверяем, является ли символ гласной буквой
        if (lowerC == 'a' || lowerC == 'e' || lowerC == 'i' ||
            lowerC == 'o' || lowerC == 'u' || lowerC == 'y') {
            vowelCount++;
        }
    }

    // Вывод результата
    cout << "Количество гласных букв в строке: " << vowelCount << endl;

    return 0;
}