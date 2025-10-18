#include <iostream>
#include <cstring>
#include <cctype>
#include <vector>

using namespace std;
int FindMaxSubstring() {
    setlocale(LC_ALL, "ru");
    string str1, str2;
    cout << "Введите первую строку: ";
    getline(cin, str1);
    cout << "Введите вторую строку: ";
    getline(cin, str2);

    int n = str1.length();
    int m = str2.length();

    // Создаем двумерный массив dp
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    int maxLength = 0;
    int endIndex = 0;

    // Заполняем массив dp
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;

                if (dp[i][j] > maxLength) {
                    maxLength = dp[i][j];
                    endIndex = i;
                }
            } else {
                dp[i][j] = 0;
            }
        }
    }

    // Извлекаем найденную подстроку
    string result = "";
    if (maxLength > 0) {
        result = str1.substr(endIndex - maxLength, maxLength);
    }

    cout << "Наибольшая общая подстрока: \"" << result << "\"" << endl;
    cout << "Длина: " << maxLength << endl;

    return 0;
}