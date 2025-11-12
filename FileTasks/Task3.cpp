#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int Task3() {
    ifstream file("../task3_text.txt");
    
    if (!file.is_open()) {
        cout << "Ошибка: файл text.txt не найден!" << endl;
        return 1;
    }
    
    int wordCount = 0;
    string word;
    
    while (file >> word) {
        wordCount++;
    }
    
    cout << "Количество слов в файле: " << wordCount << endl;
    
    file.close();
    
    return 0;
}