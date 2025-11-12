#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int Task8() {
    ifstream file1("../task8_file1.txt");
    ifstream file2("../task8_file2.txt");
    
    if (!file1.is_open()) {
        cout << "Ошибка: файл task8_file1.txt не найден!" << endl;
        return 1;
    }
    
    if (!file2.is_open()) {
        cout << "Ошибка: файл task8_file2.txt не найден!" << endl;
        file1.close();
        return 1;
    }
    
    string line1, line2;
    bool filesAreIdentical = true;
    
    while (true) {
        bool gotLine1 = static_cast<bool>(getline(file1, line1));
        bool gotLine2 = static_cast<bool>(getline(file2, line2));
        
        if (gotLine1 != gotLine2) {
            filesAreIdentical = false;
            break;
        }
        
        if (!gotLine1 && !gotLine2) {
            break;
        }
        
        if (line1 != line2) {
            filesAreIdentical = false;
            break;
        }
    }
    
    file1.close();
    file2.close();
    
    if (filesAreIdentical) {
        cout << "Файлы идентичны." << endl;
    } else {
        cout << "Файлы различаются." << endl;
    }
    
    return 0;
}