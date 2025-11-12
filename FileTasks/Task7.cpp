#include <iostream>
#include <fstream>

using namespace std;

int Task7() {
    ifstream file("../task7_numbers.txt");
    
    if (!file.is_open()) {
        cout << "Ошибка: файл task7_numbers.txt не найден!" << endl;
        return 1;
    }
    
    int number;
    int count = 0;
    int sum = 0;
    double average = 0;
    
    while (file >> number) {
        count++;
        sum += number;
    }
    
    if (count > 0) {
        average = static_cast<double>(sum) / count;
    }
    
    cout << "Количество чисел: " << count << endl;
    cout << "Сумма: " << sum << endl;
    
    if (count > 0) {
        cout << "Среднее: " << average << endl;
    } else {
        cout << "Среднее: 0 (файл пуст)" << endl;
    }
    
    file.close();
    
    return 0;
}