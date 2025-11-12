#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int Task6() {
    ofstream outputFile("../task6_output.txt");
    
    if (!outputFile.is_open()) {
        cout << "Ошибка: не удалось создать файл task6_output.txt!" << endl;
        return 1;
    }
    
    string line;
    
    cout << "Введите строки (пустая строка завершает ввод):" << endl;
    
    while (true) {
        getline(cin, line);
        
        if (line.empty()) {
            break;
        }
        
        outputFile << line << endl;
    }
    
    outputFile.close();
    
    cout << "Данные записаны в файл task6_output.txt." << endl;
    
    return 0;
}