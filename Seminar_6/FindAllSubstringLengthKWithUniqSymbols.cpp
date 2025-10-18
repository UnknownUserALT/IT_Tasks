#include <iostream>
#include <cstring>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;
int FindAllSubstringLengthKWithUniqSymbols() {
    setlocale(LC_ALL, "ru");
    char str[100];
    int k;

    cout << "Введите строку: ";
    cin.getline(str, 100);

    cout << "Введите k: ";
    cin >> k;

    int len = strlen(str);
    vector<string> uniqueSubstrings;

    cout << "Подстроки длины " << k << " с уникальными символами:" << endl;

    for (int i = 0; i <= len - k; i++) {
        bool used[256] = {false};
        bool isUnique = true;

        // Проверяем уникальность символов в подстроке
        for (int j = i; j < i + k; j++) {
            if (used[(unsigned char)str[j]]) {
                isUnique = false;
                break;
            }
            used[(unsigned char)str[j]] = true;
        }

        // Если все символы уникальны, добавляем подстроку в вектор
        if (isUnique) {
            string substring;
            for (int j = i; j < i + k; j++) {
                substring += str[j];
            }

            // Проверяем, нет ли уже такой подстроки в векторе
            if (find(uniqueSubstrings.begin(), uniqueSubstrings.end(), substring) == uniqueSubstrings.end()) {
                uniqueSubstrings.push_back(substring);
                cout << substring << endl;
            }
        }
    }

    return 0;
}