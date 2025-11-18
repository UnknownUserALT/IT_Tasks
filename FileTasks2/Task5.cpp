#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

struct Student {
    int id;
    char name[50];
    int age;
    double average_grade;
};

void printStudent(const Student& student) {
    cout << "ID: " << student.id << endl;
    cout << "Имя: " << student.name << endl;
    cout << "Возраст: " << student.age << endl;
    cout << "Средний балл: " << student.average_grade << endl;
    cout << "------------------------" << endl;
}

int Task5() {
    Student students[] = {
        {1, "Иван Петров", 20, 4.5},
        {2, "Мария Сидорова", 19, 4.8},
        {3, "Алексей Иванов", 21, 4.2}
    };
    const int count = sizeof(students) / sizeof(students[0]);
    
    ofstream outFile("../students.dat", ios::binary);
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи!" << endl;
        return 1;
    }
    
    for (int i = 0; i < count; ++i) {
        outFile.write(reinterpret_cast<const char*>(&students[i]), sizeof(Student));
    }
    outFile.close();
    
    cout << "Добавлено " << count << " студентов в файл students.dat" << endl << endl;
    
    ifstream inFile("../students.dat", ios::binary);
    if (!inFile) {
        cerr << "Ошибка открытия файла для чтения!" << endl;
        return 1;
    }
    
    vector<Student> studentsList;
    Student tempStudent;
    
    while (inFile.read(reinterpret_cast<char*>(&tempStudent), sizeof(Student))) {
        studentsList.push_back(tempStudent);
    }
    inFile.close();
    
    cout << "Список всех студентов:" << endl;
    cout << "========================" << endl;
    for (const auto& student : studentsList) {
        printStudent(student);
    }
    
    cout << "Общее количество студентов: " << studentsList.size() << endl << endl;
    
    if (!studentsList.empty()) {
        Student bestStudent = studentsList[0];
        for (const auto& student : studentsList) {
            if (student.average_grade > bestStudent.average_grade) {
                bestStudent = student;
            }
        }
        
        cout << "Студент с самым высоким средним баллом:" << endl;
        cout << "========================" << endl;
        printStudent(bestStudent);
    } else {
        cout << "В файле нет студентов!" << endl;
    }
    
    return 0;
}