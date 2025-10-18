#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;
int CompressionWords() {
    setlocale(LC_ALL, "ru");

    char str[100];
    cout << "Введите строку: ";
    cin.getline(str, 100);

    int length = strlen(str);
    char result[200] = ""; // Результирующая строка
    int resultIndex = 0;

    int i = 0;
    while (i < length) {
        char currentChar = str[i];
        int count = 0;

        // Подсчитываем количество одинаковых символов подряд
        while (i < length && str[i] == currentChar) {
            count++;
            i++;
        }

        // Добавляем символ и количество в результат
        result[resultIndex++] = currentChar;

        // Преобразуем число в символы и добавляем в результат
        if (count > 0) {
            char countStr[10];
            int j = 0;
            while (count > 0) {
                countStr[j++] = '0' + (count % 10);
                count /= 10;
            }
            // Записываем цифры в правильном порядке
            for (int k = j - 1; k >= 0; k--) {
                result[resultIndex++] = countStr[k];
            }
        }
    }

    result[resultIndex] = '\0'; // Завершаем строку

    cout << "Сжатая строка: " << result << endl;

    return 0;
}