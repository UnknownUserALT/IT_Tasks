#include <iostream>
#include <fstream>

using namespace std;

int Task5() {
    ifstream file("../task5_input.txt");
    
    if (!file.is_open()) {
        cout << "Ошибка: файл task5_input.txt не найден!" << endl;
        return 1;
    }
    
    int charCount = 0;
    char ch;
    
    while (file.get(ch)) {
        charCount++;
    }
    
    cout << "Количество символов в файле: " << charCount << endl;
    
    file.close();
    
    return 0;
}