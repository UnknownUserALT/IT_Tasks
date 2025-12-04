#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Структура для представления сотрудника
struct Employee {
    string name;
    int age;
    string position;
    double salary;
};

int Task1() {
    int n;
    cout << "Введите количество сотрудников: ";
    cin >> n;
    cin.ignore(); // Очищаем буфер после ввода числа
    
    vector<Employee> employees(n);
    
    // Вводим данные о каждом сотруднике
    for (int i = 0; i < n; i++) {
        cout << "Введите имя сотрудника: ";
        getline(cin, employees[i].name);
        
        cout << "Введите возраст сотрудника: ";
        cin >> employees[i].age;
        cin.ignore();
        
        cout << "Введите должность сотрудника: ";
        getline(cin, employees[i].position);
        
        cout << "Введите зарплату сотрудника: ";
        cin >> employees[i].salary;
        cin.ignore();
    }
    
    // Ищем самого молодого и самого старшего сотрудника
    int youngestIndex = 0;
    int oldestIndex = 0;
    
    for (int i = 1; i < n; i++) {
        if (employees[i].age < employees[youngestIndex].age) {
            youngestIndex = i;
        }
        if (employees[i].age > employees[oldestIndex].age) {
            oldestIndex = i;
        }
    }
    
    // Считаем среднюю зарплату
    double totalSalary = 0;
    for (int i = 0; i < n; i++) {
        totalSalary += employees[i].salary;
    }
    double averageSalary = totalSalary / n;
    
    // Выводим результаты
    cout << "Самый молодой сотрудник: " << employees[youngestIndex].name 
         << ", возраст: " << employees[youngestIndex].age 
         << ", должность: " << employees[youngestIndex].position 
         << ", зарплата: " << employees[youngestIndex].salary << endl;
    
    cout << "Самый старший сотрудник: " << employees[oldestIndex].name 
         << ", возраст: " << employees[oldestIndex].age 
         << ", должность: " << employees[oldestIndex].position 
         << ", зарплата: " << employees[oldestIndex].salary << endl;
    
    cout << "Средняя зарплата всех сотрудников: " << averageSalary << endl;
    
    return 0;
}