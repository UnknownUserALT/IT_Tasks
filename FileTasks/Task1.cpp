#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int Task1() {
    ifstream inputFile("../task1_input.txt");
    
    if (inputFile.fail()) {
        cout << "Ошибка: файл input.txt не найден!" << endl;
        return 1; // Возвращаем код ошибки
    }
    
    cout << "Содержимое файла:" << endl;
    
    string line;
    while (getline(inputFile, line)) {
        cout << line << endl;
    }
    
    inputFile.close();
    return 0;
}