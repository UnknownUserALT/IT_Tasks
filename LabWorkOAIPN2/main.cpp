#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Функция для разделения строки на слова
vector<string> splitString(const string& str) {
    vector<string> words;
    stringstream ss(str);
    string word;

    // Разбиваем строку на слова с помощью stringstream
    while (ss >> word) {
        words.push_back(word);
    }

    return words;
}

// 1) Количество слов, начинающихся с буквы 'b'
int countWordsStartingWithB(const vector<string>& words) {
    int count = 0;
    for (const auto& word : words) {
        // Проверяем, что слово не пустое и начинается с 'b' (строчной или заглавной)
        if (!word.empty() && (word[0] == 'b' || word[0] == 'B')) {
            count++;
        }
    }
    return count;
}

// 2) Длина самого длинного слова
int findLongestWordLength(const vector<string>& words) {
    int maxLength = 0;
    for (const auto& word : words) {
        // Сравниваем длину текущего слова с максимальной найденной длиной
        if (word.length() > maxLength) {
            maxLength = word.length();
        }
    }
    return maxLength;
}

// 3) Количество букв 'c' в третьем слове строки
int countCInThirdWord(const vector<string>& words) {
    // Проверяем, что в строке есть хотя бы 3 слова
    if (words.size() < 3) {
        return 0;
    }

    int count = 0;
    const string& thirdWord = words[2]; // Третье слово имеет индекс 2

    // Проходим по всем символам третьего слова
    for (char c : thirdWord) {
        // Считаем буквы 'c' (строчные и заглавные)
        if (c == 'c' || c == 'C') {
            count++;
        }
    }

    return count;
}

// 4) Замена всех строчных букв на буквы верхнего регистра
string toUpperCase(const string& str) {
    string result = str;
    // Проходим по всем символам строки
    for (char& c : result) {
        // Преобразуем каждый символ в верхний регистр
        c = toupper(c);
    }
    return result;
}

// 5) Количество слов, у которых совпадает второй и предпоследний символ
int countWordsWithMatchingSecondAndPenultimate(const vector<string>& words) {
    int count = 0;

    for (const auto& word : words) {
        // Для проверки нужно как минимум 4 символа в слове
        // (второй символ - индекс 1, предпоследний - индекс (длина-2))
        if (word.length() >= 4) {
            // Сравниваем второй и предпоследний символы
            if (word[1] == word[word.length() - 2]) {
                count++;
            }
        }
    }

    return count;
}

// 6) Найти самую длинную общую подстроку между двумя словами в предложении
string findLongestCommonSubstring(const vector<string>& words) {
    // Проверяем, что в предложении есть хотя бы 2 слова
    if (words.size() < 2) {
        return "";
    }

    string longestCommon = ""; // Здесь будем хранить самую длинную общую подстроку

    // Проверяем все возможные пары слов в предложении
    for (int i = 0; i < words.size(); i++) {
        for (int j = i + 1; j < words.size(); j++) {
            const string& word1 = words[i];
            const string& word2 = words[j];

            // Алгоритм поиска самой длинной общей подстроки с использованием динамического программирования

            int n = word1.length();
            int m = word2.length();

            // Создаем матрицу для динамического программирования
            // dp[i][j] будет хранить длину общей подстроки, заканчивающейся в позициях i-1 и j-1
            vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
            int maxLength = 0;   // Максимальная длина общей подстроки для текущей пары
            int endIndex = 0;    // Конечный индекс в word1 для самой длинной подстроки

            // Заполняем матрицу dp
            for (int i1 = 1; i1 <= n; i1++) {
                for (int j1 = 1; j1 <= m; j1++) {
                    // Если символы совпадают
                    if (word1[i1 - 1] == word2[j1 - 1]) {
                        // Увеличиваем длину общей подстроки
                        dp[i1][j1] = dp[i1 - 1][j1 - 1] + 1;

                        // Обновляем максимальную длину и конечный индекс
                        if (dp[i1][j1] > maxLength) {
                            maxLength = dp[i1][j1];
                            endIndex = i1 - 1;
                        }
                    } else {
                        // Если символы не совпадают, общая подстрока прерывается
                        dp[i1][j1] = 0;
                    }
                }
            }

            // Если нашли общую подстроку для текущей пары слов
            if (maxLength > 0) {
                // Извлекаем подстроку из первого слова
                // endIndex - maxLength + 1 - начальный индекс, maxLength - длина
                string commonSub = word1.substr(endIndex - maxLength + 1, maxLength);

                // Сравниваем с текущей самой длинной общей подстрокой
                if (commonSub.length() > longestCommon.length()) {
                    longestCommon = commonSub;
                }
            }
        }
    }

    return longestCommon;
}

int main() {
    string input;

    cout << "Введите строку из 10 слов на латинице: ";
    getline(cin, input);

    // Разделяем введенную строку на отдельные слова
    vector<string> words = splitString(input);

    // Проверяем, что пользователь ввел достаточно слов
    if (words.size() < 10) {
        cout << "Ошибка: в строке должно быть не менее 10 слов!" << endl;
        cout << "Вы ввели только " << words.size() << " слов." << endl;
        return 1;
    }

    cout << "\n=== РЕЗУЛЬТАТЫ АНАЛИЗА СТРОКИ ===" << endl;

    // 1) Подсчет слов, начинающихся с буквы 'b'
    int bWords = countWordsStartingWithB(words);
    cout << "1) Слов, начинающихся с буквы 'b': " << bWords << endl;

    // 2) Поиск длины самого длинного слова
    int maxLength = findLongestWordLength(words);
    cout << "2) Длина самого длинного слова: " << maxLength << endl;

    // 3) Подсчет букв 'c' в третьем слове
    int cCount = countCInThirdWord(words);
    cout << "3) Количество букв 'c' в третьем слове: " << cCount << endl;

    // 4) Преобразование всей строки в верхний регистр
    string upperCaseStr = toUpperCase(input);
    cout << "4) Строка в верхнем регистре: " << upperCaseStr << endl;

    // 5) Подсчет слов с совпадающим вторым и предпоследним символом
    int matchingChars = countWordsWithMatchingSecondAndPenultimate(words);
    cout << "5) Слов с совпадающим вторым и предпоследним символом: " << matchingChars << endl;

    // 6) Поиск самой длинной общей подстроки между любыми двумя словами
    string longestCommon = findLongestCommonSubstring(words);
    if (longestCommon.empty()) {
        cout << "6) Общих подстрок не найдено" << endl;
    } else {
        cout << "6) Самая длинная общая подстрока: " << longestCommon << endl;
    }

    // Дополнительно: выводим все слова для наглядности
    cout << "\n--- Все слова в строке ---" << endl;
    for (int i = 0; i < words.size(); i++) {
        cout << (i + 1) << ") " << words[i] << " (длина: " << words[i].length() << ")" << endl;
    }

    return 0;
}