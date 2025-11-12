#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int Task4() {
    ifstream sourceFile("../task4_source.txt");
    
    if (!sourceFile.is_open()) {
        cout << "Ошибка: файл task4_source.txt не найден!" << endl;
        return 1;
    }
    
    ofstream copyFile("../task4_copy.txt");
    
    if (!copyFile.is_open()) {
        cout << "Ошибка: не удалось создать файл task4_copy.txt!" << endl;
        sourceFile.close();
        return 1;
    }
    
    int lineCount = 0;
    string line;
    
    while (getline(sourceFile, line)) {
        copyFile << line << endl;
        lineCount++;
    }
    
    sourceFile.close();
    copyFile.close();
    
    cout << "Файл успешно скопирован." << endl;
    cout << "Количество скопированных строк: " << lineCount << endl;
    
    return 0;
}