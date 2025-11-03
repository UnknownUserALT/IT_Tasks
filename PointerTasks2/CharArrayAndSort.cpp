#include <iostream>
#include <cstring>

using namespace std;

void sortWords(char **words, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char *temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void printWords(char **words, int n, const string& title = "Массив строк:") {
    cout << title << endl;
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". " << words[i] << " (адрес: " << (void*)words[i] << ")" << endl;
    }
    cout << endl;
}

int CharArrayAndSort() {
    const int n = 7;
    
    char **words = new char*[n];
    
    words[0] = new char[20]; strcpy(words[0], "banana");
    words[1] = new char[20]; strcpy(words[1], "apple");
    words[2] = new char[20]; strcpy(words[2], "orange");
    words[3] = new char[20]; strcpy(words[3], "grape");
    words[4] = new char[20]; strcpy(words[4], "cherry");
    words[5] = new char[20]; strcpy(words[5], "pear");
    words[6] = new char[20]; strcpy(words[6], "kiwi");
    
    cout << "=== ДО сортировки ===" << endl;
    printWords(words, n, "Исходный массив строк:");
    
    sortWords(words, n);
    
    cout << "=== ПОСЛЕ сортировки ===" << endl;
    printWords(words, n, "Отсортированный массив строк:");
    
    cout << "=== ДЕМОНСТРАЦИЯ ===" << endl;
    cout << "Строки не копировались - изменился только порядок указателей:" << endl;
    cout << "Слово 'apple' все еще находится по адресу: " << (void*)words[0] << endl;
    cout << "Слово 'banana' все еще находится по адресу: " << (void*)words[1] << endl;
    
    // Освобождаем память
    for (int i = 0; i < n; i++) {
        delete[] words[i];
    }
    delete[] words;
    
    return 0;
}