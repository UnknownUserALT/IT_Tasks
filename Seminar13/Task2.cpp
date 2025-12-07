#include <iostream>
#include <string>

using namespace std;

// Базовый класс Shape
class Shape {
protected:
    double length;  // Длина фигуры

public:
    // Конструктор по умолчанию
    Shape() : length(0.0) {}
    
    // Конструктор с параметром
    Shape(double l) : length(l) {}
    
    // Геттер для длины
    double getLength() const {
        return length;
    }
    
    // Виртуальный метод для расчета площади (будет переопределен в производных классах)
    virtual double calculateArea() const {
        return 0.0;  // Базовая реализация
    }
    
    // Виртуальный метод для расчета периметра
    virtual double calculatePerimeter() const {
        return 0.0;  // Базовая реализация
    }
    
    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        cout << "Длина: " << length << endl;
    }
};

// Производный класс Rectangle
class Rectangle : public Shape {
private:
    double width;  // Ширина прямоугольника

public:
    // Конструктор по умолчанию
    Rectangle() : Shape(), width(0.0) {}
    
    // Конструктор с параметрами
    Rectangle(double l, double w) : Shape(l), width(w) {
        if (w < 0) width = 0.0;
    }
    
    // Геттер для ширины
    double getWidth() const {
        return width;
    }
    
    // Метод для расчета площади прямоугольника
    double calculateArea() const override {
        return length * width;
    }
    
    // Метод для расчета периметра прямоугольника
    double calculatePerimeter() const override {
        return 2 * (length + width);
    }
    
    // Метод для вывода информации о прямоугольнике
    void displayInfo() const override {
        cout << "Длина: " << length 
             << ", Ширина: " << width 
             << ", Площадь: " << calculateArea()
             << ", Периметр: " << calculatePerimeter() << endl;
    }

};

int Task2() {
    Rectangle rect1(5.0, 3.0);
    cout << "Прямоугольник 1:" << endl;
    rect1.displayInfo();

    Rectangle rect2(7.5, 4.2);

    cout << "Прямоугольник 2:" << endl;
    rect2.displayInfo();
    
    // Демонстрация работы геттеров
    cout << "\nДанные прямоугольника 2 (через геттеры):" << endl;
    cout << "Длина: " << rect2.getLength() << endl;
    cout << "Ширина: " << rect2.getWidth() << endl;
    cout << "Площадь: " << rect2.calculateArea() << endl;
    cout << "Периметр: " << rect2.calculatePerimeter() << endl;
    
    cout << "\n=== Демонстрация полиморфизма ===" << endl;
    // Демонстрация работы через указатель на базовый класс
    Shape* shapePtr = &rect1;
    cout << "Через указатель на базовый класс Shape:" << endl;
    shapePtr->displayInfo();
    
    return 0;
}