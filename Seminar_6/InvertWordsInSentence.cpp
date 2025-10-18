#include <iostream>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;
int InvertWordsInSentence() {
    setlocale(LC_ALL, "ru");
    string text;
    cout << "Введите текст: ";
    getline(cin, text);

    string result = "";
    string word = "";

    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] == ' ') {
            // Переворачиваем накопленное слово
            reverse(word.begin(), word.end());
            result += word + " ";
            word = "";
        } else {
            word += text[i];
        }
    }

    // Не забываем последнее слово
    if (!word.empty()) {
        reverse(word.begin(), word.end());
        result += word;
    }

    cout << "Результат: \"" << result << "\"" << endl;

    return 0;

    return 0;
}