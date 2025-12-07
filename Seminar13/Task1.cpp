#include <iostream>
#include <string>

using namespace std;

// Базовый класс Publication
class Publication {
protected:
    string title;  // Название публикации

public:
    // Конструктор по умолчанию
    Publication() : title("") {}
    
    // Конструктор с параметром
    Publication(const string& t) : title(t) {}
    
    // Метод для получения названия
    string getTitle() const {
        return title;
    }

    // Виртуальный метод для вывода информации (для возможного расширения)
    virtual void displayInfo() const {
        cout << "Название: " << title;
    }
    
    // Виртуальный деструктор
    virtual ~Publication() {}
};

// Производный класс Book
class Book : public Publication {
private:
    string author;  // Автор книги
    int year;       // Год выпуска

public:
    // Конструктор по умолчанию
    Book() : author(""), year(0) {}
    
    // Конструктор с параметрами
    Book(const string& t, const string& a, int y) 
        : Publication(t), author(a), year(y) {}
    
    // Методы для получения значений
    string getAuthor() const {
        return author;
    }
    
    int getYear() const {
        return year;
    }
    
    void displayInfo() const override {
        cout << "Название: " << title 
             << ", Автор: " << author 
             << ", Год выпуска: " << year << endl;
    }
};

int Task1() {
    Book book1("Преступление и наказание", "Фёдор Достоевский", 1866);
    
    cout << "Информация о книге (созданной через конструктор с параметрами):" << endl;
    book1.displayInfo();
    
    cout << "\n" << string(50, '-') << "\n" << endl;
    
    Book book2("Мастер и Маргарита", "Михаил Булгаков", 1967);

    // Вывод информации о книге
    cout << "Информация о книге (созданной через сеттеры):" << endl;
    book2.displayInfo();
    
    // Демонстрация работы с указателем на базовый класс
    cout << "\n" << string(50, '-') << "\n" << endl;
    cout << "Демонстрация полиморфизма:" << endl;
    
    Publication* publicationPtr = &book1;
    cout << "Через указатель на базовый класс: ";
    publicationPtr->displayInfo();
    
    return 0;
}