#include <iostream>
#include <string>
#include <cctype>
#include <locale>
using namespace std;

int RegistrCorrect() {
       std::string s;
    std::getline(std::cin, s);

    bool new_sentence = true;

    for (size_t i = 0; i < s.size(); ++i) {
        // Пропускаем не-буквы
        if (s[i] == '.' || s[i] == '!' || s[i] == '?') {
            new_sentence = true;
            continue;
        }

        // Ищем начало буквы (предполагаем UTF-8)
        if (new_sentence && (s[i] & 0x80) == 0) {
            // ASCII символ
            if (s[i] >= 'a' && s[i] <= 'z') {
                s[i] = s[i] - 'a' + 'A';
                new_sentence = false;
            } else if (s[i] >= 'A' && s[i] <= 'Z') {
                new_sentence = false;
            }
        } else if (new_sentence && (s[i] & 0xE0) == 0xC0 && i + 1 < s.size()) {
            // 2-байтный UTF-8 символ (кириллица)
            unsigned char b1 = s[i];
            unsigned char b2 = s[i + 1];

            // Проверяем, что это строчная кириллическая буква: U+0430 – U+044F ('а'–'я')
            if (b1 == 0xD0 && b2 >= 0xB0 && b2 <= 0xBF) {
                // 'а'–'п' -> 'А'–'П': 0xD0 0xB0–0xBF → 0xD0 0x90–0x9F
                s[i + 1] = b2 - 0x20;
                new_sentence = false;
            } else if (b1 == 0xD1 && b2 >= 0x80 && b2 <= 0x8F) {
                // 'р'–'я' -> 'Р'–'Я': 0xD1 0x80–0x8F → 0xD0 0xA0–0xAF
                s[i] = 0xD0;
                s[i + 1] = b2 + 0x20;
                new_sentence = false;
            } else if ((b1 == 0xD0 && b2 >= 0x90 && b2 <= 0x9F) ||
                       (b1 == 0xD0 && b2 >= 0xA0 && b2 <= 0xAF)) {
                // Уже заглавная — просто сбрасываем флаг
                new_sentence = false;
            }
        } else if (!new_sentence && (s[i] & 0xE0) == 0xC0 && i + 1 < s.size()) {
            // Приведение остальных к строчным (опционально, по условию)
            unsigned char b1 = s[i];
            unsigned char b2 = s[i + 1];

            // Заглавные к строчным
            if (b1 == 0xD0 && b2 >= 0x90 && b2 <= 0x9F) {
                // 'А'–'П' → 'а'–'п'
                s[i + 1] = b2 + 0x20;
            } else if (b1 == 0xD0 && b2 >= 0xA0 && b2 <= 0xAF) {
                // 'Р'–'Я' → 'р'–'я'
                s[i] = 0xD1;
                s[i + 1] = b2 - 0x20;
            }
            // Пропускаем второй байт
            i++;
            continue;
        }

        // Пропускаем второй байт UTF-8 символа
        if ((s[i] & 0xE0) == 0xC0 && i + 1 < s.size()) {
            i++;
        }
    }

    std::cout << s << std::endl;
    return 0;
}