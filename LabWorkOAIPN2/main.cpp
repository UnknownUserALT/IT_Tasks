#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

// Константы для ограничения размеров массивов
const int MAX_INPUT_SIZE = 1000;    // Максимальная длина входной строки
const int MAX_WORDS_COUNT = 50;     // Максимальное количество слов
const int MAX_WORD_LEN = 100;       // Максимальная длина одного слова

// Функция для разделения строки на слова
// str - входная строка для разбиения
// words - двумерный массив для хранения слов
// wordCount - переменная для возврата количества найденных слов
void splitString(const char* str, char words[][MAX_WORD_LEN], int& wordCount) {
    wordCount = 0;  // Инициализируем счетчик слов
    int len = strlen(str);  // Получаем длину входной строки
    int wordIndex = 0;      // Индекс текущего символа в слове
    bool inWord = false;    // Флаг, указывающий, находимся ли мы внутри слова

    // Проходим по всем символам строки включая завершающий нуль-символ
    for (int i = 0; i <= len; i++) {
        // Если текущий символ не пробел и не конец строки - это часть слова
        if (str[i] != ' ' && str[i] != '\0') {
            // Если мы не в слове, значит начинается новое слово
            if (!inWord) {
                inWord = true;      // Устанавливаем флаг "в слове"
                wordIndex = 0;      // Сбрасываем индекс символа
            }
            // Записываем символ в слово, проверяя границы массива
            if (wordIndex < MAX_WORD_LEN - 1) {
                words[wordCount][wordIndex++] = str[i];
            }
        } else {
            // Если мы в слове и встретили пробел или конец строки
            if (inWord) {
                words[wordCount][wordIndex] = '\0';  // Добавляем нуль-терминатор
                wordCount++;        // Увеличиваем счетчик слов
                inWord = false;     // Сбрасываем флаг "в слове"
            }
        }
    }
}

// 1) Количество слов, начинающихся с буквы 'b' (регистронезависимо)
// words - массив слов
// wordCount - количество слов в массиве
// возвращает количество слов, начинающихся на 'b' или 'B'
int countWordsStartingWithB(char words[][MAX_WORD_LEN], int wordCount) {
    int count = 0;
    // Проходим по всем словам
    for (int i = 0; i < wordCount; i++) {
        // Проверяем первую букву слова
        if (words[i][0] == 'b' || words[i][0] == 'B') {
            count++;  // Увеличиваем счетчик
        }
    }
    return count;
}

// 2) Длина самого длинного слова в предложении
// words - массив слов
// wordCount - количество слов в массиве
// возвращает длину самого длинного слова
int findLongestWordLength(char words[][MAX_WORD_LEN], int wordCount) {
    int maxLength = 0;
    // Проходим по всем словам
    for (int i = 0; i < wordCount; i++) {
        int currentLength = strlen(words[i]);  // Получаем длину текущего слова
        // Сравниваем с максимальной найденной длиной
        if (currentLength > maxLength) {
            maxLength = currentLength;  // Обновляем максимум
        }
    }
    return maxLength;
}

// 3) Количество букв 'c' в третьем слове строки (регистронезависимо)
// words - массив слов
// wordCount - количество слов в массиве
// возвращает количество букв 'c' или 'C' в третьем слове
int countCInThirdWord(char words[][MAX_WORD_LEN], int wordCount) {
    // Проверяем, что в строке есть хотя бы 3 слова
    if (wordCount < 3) {
        return 0;  // Если слов меньше 3, возвращаем 0
    }

    int count = 0;
    char* thirdWord = words[2];  // Третье слово имеет индекс 2
    int len = strlen(thirdWord); // Получаем длину третьего слова

    // Проходим по всем символам третьего слова
    for (int i = 0; i < len; i++) {
        // Считаем буквы 'c' и 'C'
        if (thirdWord[i] == 'c' || thirdWord[i] == 'C') {
            count++;  // Увеличиваем счетчик
        }
    }

    return count;
}

// 4) Преобразование всех строчных букв в строке в верхний регистр
// str - строка для преобразования (изменяется на месте)
void toUpperCase(char* str) {
    // Проходим по всем символам строки до нуль-терминатора
    for (int i = 0; str[i] != '\0'; i++) {
        // Преобразуем каждый символ в верхний регистр
        str[i] = toupper(str[i]);
    }
}

// 5) Количество слов, у которых совпадает второй и предпоследний символ
// words - массив слов
// wordCount - количество слов в массиве
// возвращает количество слов, удовлетворяющих условию
int countWordsWithMatchingSecondAndPenultimate(char words[][MAX_WORD_LEN], int wordCount) {
    int count = 0;

    // Проходим по всем словам
    for (int i = 0; i < wordCount; i++) {
        int len = strlen(words[i]);  // Получаем длину текущего слова
        // Для проверки нужно как минимум 4 символа в слове:
        // - второй символ имеет индекс 1
        // - предпоследний символ имеет индекс (длина-2)
        if (len >= 4) {
            // Сравниваем второй и предпоследний символы
            if (words[i][1] == words[i][len - 2]) {
                count++;  // Увеличиваем счетчик при совпадении
            }
        }
    }

    return count;
}

// 6) Найти самую длинную общую подстроку между двумя словами в предложении
// words - массив слов
// wordCount - количество слов в массиве
// result - буфер для записи результата (самой длинной общей подстроки)
void findLongestCommonSubstring(char words[][MAX_WORD_LEN], int wordCount, char* result) {
    result[0] = '\0'; // Инициализируем результат пустой строкой

    // Проверяем, что в предложении есть хотя бы 2 слова
    if (wordCount < 2) {
        return;  // Недостаточно слов для поиска общих подстрок
    }

    int maxCommonLength = 0;  // Длина самой длинной найденной подстроки

    // Матрица для динамического программирования
    // dp[i][j] хранит длину общей подстроки, заканчивающейся в позициях i-1 и j-1
    int dp[MAX_WORD_LEN + 1][MAX_WORD_LEN + 1];

    // Проверяем все возможные пары слов в предложении
    for (int i = 0; i < wordCount; i++) {
        for (int j = i + 1; j < wordCount; j++) {
            char* word1 = words[i];  // Первое слово пары
            char* word2 = words[j];  // Второе слово пары

            int n = strlen(word1);  // Длина первого слова
            int m = strlen(word2);  // Длина второго слова

            int maxLength = 0;  // Максимальная длина общей подстроки для текущей пары
            int endPos = 0;     // Позиция конца подстроки в первом слове

            // Заполняем матрицу динамического программирования
            for (int i1 = 0; i1 <= n; i1++) {
                for (int j1 = 0; j1 <= m; j1++) {
                    // Инициализация граничных условий
                    if (i1 == 0 || j1 == 0) {
                        dp[i1][j1] = 0;  // Нулевая строка и столбец заполняются нулями
                    } else if (word1[i1 - 1] == word2[j1 - 1]) {
                        // Если символы совпадают, увеличиваем длину общей подстроки
                        dp[i1][j1] = dp[i1 - 1][j1 - 1] + 1;
                        // Проверяем, не найден ли новый максимум
                        if (dp[i1][j1] > maxLength) {
                            maxLength = dp[i1][j1];  // Обновляем максимальную длину
                            endPos = i1 - 1;        // Запоминаем позицию конца подстроки
                        }
                    } else {
                        // Если символы не совпадают, общая подстрока прерывается
                        dp[i1][j1] = 0;
                    }
                }
            }

            // Если нашли общую подстроку длиннее предыдущих
            if (maxLength > maxCommonLength && maxLength > 0) {
                maxCommonLength = maxLength;  // Обновляем глобальный максимум
                // Вычисляем начальную позицию подстроки в первом слове
                int startPos = endPos - maxLength + 1;
                // Копируем найденную подстроку в результат
                strncpy(result, word1 + startPos, maxLength);
                result[maxLength] = '\0';  // Добавляем нуль-терминатор
            }
        }
    }
}

int main() {
    // Объявляем массивы для хранения данных
    char input[MAX_INPUT_SIZE];              // Буфер для входной строки
    char words[MAX_WORDS_COUNT][MAX_WORD_LEN]; // Массив для хранения слов
    int wordCount = 0;                       // Счетчик слов

    // Получаем входную строку от пользователя
    cout << "Введите строку из 10 слов на латинице: ";
    cin.getline(input, MAX_INPUT_SIZE);

    // Разделяем введенную строку на отдельные слова
    splitString(input, words, wordCount);

    // Проверяем, что пользователь ввел достаточно слов
    if (wordCount < 10) {
        cout << "Ошибка: в строке должно быть не менее 10 слов!" << endl;
        cout << "Вы ввели только " << wordCount << " слов." << endl;
        return 1;  // Завершаем программу с кодом ошибки
    }

    // Выводим заголовок для результатов
    cout << "\n=== РЕЗУЛЬТАТЫ АНАЛИЗА СТРОКИ ===" << endl;

    // 1) Подсчет слов, начинающихся с буквы 'b'
    int bWords = countWordsStartingWithB(words, wordCount);
    cout << "1) Слов, начинающихся с буквы 'b': " << bWords << endl;

    // 2) Поиск длины самого длинного слова
    int maxLength = findLongestWordLength(words, wordCount);
    cout << "2) Длина самого длинного слова: " << maxLength << endl;

    // 3) Подсчет букв 'c' в третьем слове
    int cCount = countCInThirdWord(words, wordCount);
    cout << "3) Количество букв 'c' в третьем слове: " << cCount << endl;

    // 4) Преобразование всей строки в верхний регистр
    char upperCaseStr[MAX_INPUT_SIZE];      // Буфер для строки в верхнем регистре
    strcpy(upperCaseStr, input);           // Копируем исходную строку
    toUpperCase(upperCaseStr);             // Преобразуем в верхний регистр
    cout << "4) Строка в верхнем регистре: " << upperCaseStr << endl;

    // 5) Подсчет слов с совпадающим вторым и предпоследним символом
    int matchingChars = countWordsWithMatchingSecondAndPenultimate(words, wordCount);
    cout << "5) Слов с совпадающим вторым и предпоследним символом: " << matchingChars << endl;

    // 6) Поиск самой длинной общей подстроки между любыми двумя словами
    char longestCommon[MAX_WORD_LEN];  // Буфер для самой длинной общей подстроки
    findLongestCommonSubstring(words, wordCount, longestCommon);
    // Проверяем, была ли найдена общая подстрока
    if (strlen(longestCommon) == 0) {
        cout << "6) Общих подстрок не найдено" << endl;
    } else {
        cout << "6) Самая длинная общая подстрока: " << longestCommon << endl;
    }

    // Дополнительно: выводим все слова для наглядности
    cout << "\n--- Все слова в строке ---" << endl;
    for (int i = 0; i < wordCount; i++) {
        // Выводим номер слова, само слово и его длину
        cout << (i + 1) << ") " << words[i] << " (длина: " << strlen(words[i]) << ")" << endl;
    }

    return 0;  // Успешное завершение программы
}