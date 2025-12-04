#include <iostream>

using namespace std;

// Структура для хранения даты
struct Date {
    int year;
    int month;
    int day;
};

// Структура для хранения времени
struct Time {
    int hour;
    int minute;
    int second;
};

// Структура для полной даты и времени
struct DateTimeStruct {
    Date date;
    Time time;
};

// Объединение для хранения даты и времени
union DateTime {
    DateTimeStruct full;
    int components[6]; // год, месяц, день, час, минута, секунда
};

int Task6() {
    DateTime dt;
    
    cout << "Введите год: ";
    cin >> dt.full.date.year;
    
    cout << "Введите месяц: ";
    cin >> dt.full.date.month;
    
    cout << "Введите день: ";
    cin >> dt.full.date.day;
    
    cout << "Введите час: ";
    cin >> dt.full.time.hour;
    
    cout << "Введите минуту: ";
    cin >> dt.full.time.minute;
    
    cout << "Введите секунду: ";
    cin >> dt.full.time.second;
    
    // Выводим дату и время
    cout << "Дата и время: " 
         << dt.full.date.year << "-" 
         << dt.full.date.month << "-" 
         << dt.full.date.day << " " 
         << dt.full.time.hour << ":" 
         << dt.full.time.minute << ":" 
         << dt.full.time.second << endl;
    
    return 0;
}