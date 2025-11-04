#include <iostream>
#include <memory>
#include <cstring>
#include <cctype>
#include <string>

// Функция для проверки, является ли символ гласной буквой (только для английского)
bool isVowel(char c) {
    c = std::tolower(static_cast<unsigned char>(c));
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y');
}

// Функция для проверки, является ли символ согласной буквой (только для английского)
bool isConsonant(char c) {
    c = std::tolower(static_cast<unsigned char>(c));
    return std::isalpha(static_cast<unsigned char>(c)) && !isVowel(c);
}

int Task7() {
    // Используем только английские символы чтобы избежать ошибки
    const char* inputString = "Hello World! Programming is fun!";
    int length = std::strlen(inputString);

    // Создаем динамический массив символов с помощью unique_ptr
    std::unique_ptr<char[]> charArray = std::make_unique<char[]>(length + 1);

    // Заполняем массив строкой
    std::cout << "1. Заполняем массив строкой: \"" << inputString << "\"" << std::endl;
    std::strcpy(charArray.get(), inputString);

    std::cout << "Содержимое массива: " << charArray.get() << std::endl;

    // Подсчитываем количество гласных и согласных
    int vowelCount = 0;
    int consonantCount = 0;

    for (int i = 0; i < length; ++i) {
        if (isVowel(charArray[i])) {
            vowelCount++;
        } else if (isConsonant(charArray[i])) {
            consonantCount++;
        }
    }

    std::cout << "\n2. Подсчет букв:" << std::endl;
    std::cout << "Гласные буквы: " << vowelCount << std::endl;
    std::cout << "Согласные буквы: " << consonantCount << std::endl;

    // Создаем новый массив только с гласными буквами
    if (vowelCount > 0) {
        std::unique_ptr<char[]> vowelsArray = std::make_unique<char[]>(vowelCount + 1);

        int index = 0;
        for (int i = 0; i < length; ++i) {
            if (isVowel(charArray[i])) {
                vowelsArray[index++] = charArray[i];
            }
        }
        vowelsArray[vowelCount] = '\0';

        std::cout << "\n3. Массив только с гласными буквами:" << std::endl;
        std::cout << "Гласные: " << vowelsArray.get() << std::endl;
        std::cout << "Количество гласных: " << vowelCount << std::endl;
    } else {
        std::cout << "\n3. В строке нет гласных букв" << std::endl;
    }

    std::cout << "\n4. Память автоматически освобождается при выходе из области видимости" << std::endl;

    return 0;
}