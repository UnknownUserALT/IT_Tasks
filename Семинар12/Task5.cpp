#include <iostream>

using namespace std;

// Структура для хранения данных прямоугольника
struct Rectangle {
    double length;
    double width;
};

// Объединение для представления фигур
union Shape {
    double radius;           // Для круга
    Rectangle rectangle;     // Для прямоугольника
};

int Task5() {
    Shape shape;
    int choice;
    const double PI = 3.1416;
    
    while (true) {
        cout << "\nВыберите фигуру (1 - круг, 2 - прямоугольник, 0 - выход): ";
        cin >> choice;
        
        if (choice == 1) {
            // Работа с кругом
            cout << "Введите радиус круга: ";
            cin >> shape.radius;
            
            double area = PI * shape.radius * shape.radius;
            cout << "Площадь круга: " << area << endl;
            
        } else if (choice == 2) {
            // Работа с прямоугольником
            cout << "Введите длину прямоугольника: ";
            cin >> shape.rectangle.length;
            
            cout << "Введите ширину прямоугольника: ";
            cin >> shape.rectangle.width;
            
            double area = shape.rectangle.length * shape.rectangle.width;
            cout << "Площадь прямоугольника: " << area << endl;
            
        } else if (choice == 0) {
            cout << "Программа завершена." << endl;
            break;
            
        } else {
            cout << "Неверный выбор!" << endl;
        }
    }
    
    return 0;
}