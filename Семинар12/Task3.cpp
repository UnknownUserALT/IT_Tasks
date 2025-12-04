#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Структура для представления книги
struct Book {
    string title;
    string author;
    int year;
    string genre;
};

// Структура для представления библиотеки
struct Library {
    vector<Book> books;
    int bookCount;
};

// Функция для добавления книги в библиотеку
void addBook(Library& library) {
    Book newBook;
    cin.ignore();
    
    cout << "Введите название книги: ";
    getline(cin, newBook.title);
    
    cout << "Введите автора книги: ";
    getline(cin, newBook.author);
    
    cout << "Введите год издания: ";
    cin >> newBook.year;
    cin.ignore();
    
    cout << "Введите жанр книги: ";
    getline(cin, newBook.genre);
    
    library.books.push_back(newBook);
    library.bookCount++;
    
    cout << "Книга добавлена в библиотеку!" << endl;
}

// Функция для удаления книги по названию
void removeBook(Library& library) {
    string title;
    cin.ignore();
    
    cout << "Введите название книги для удаления: ";
    getline(cin, title);
    
    bool found = false;
    for (int i = 0; i < library.books.size(); i++) {
        if (library.books[i].title == title) {
            library.books.erase(library.books.begin() + i);
            library.bookCount--;
            found = true;
            cout << "Книга удалена из библиотеки!" << endl;
            break;
        }
    }
    
    if (!found) {
        cout << "Книга с таким названием не найдена." << endl;
    }
}

// Функция для поиска книг по автору
void searchByAuthor(Library& library) {
    string author;
    cin.ignore();
    
    cout << "Введите имя автора для поиска: ";
    getline(cin, author);
    
    cout << "\nКниги автора " << author << ":" << endl;
    bool found = false;
    
    for (int i = 0; i < library.books.size(); i++) {
        if (library.books[i].author == author) {
            cout << "Название: " << library.books[i].title << endl;
            cout << "Год издания: " << library.books[i].year << endl;
            cout << "Жанр: " << library.books[i].genre << endl;
            cout << "-------------------" << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "Книги этого автора не найдены." << endl;
    }
}

// Функция для вывода всех книг в библиотеке
void showAllBooks(Library& library) {
    if (library.bookCount == 0) {
        cout << "Библиотека пуста." << endl;
        return;
    }
    
    cout << "\nВсе книги в библиотеке:" << endl;
    cout << "Количество книг: " << library.bookCount << endl;
    cout << "-------------------" << endl;
    
    for (int i = 0; i < library.books.size(); i++) {
        cout << "Название: " << library.books[i].title << endl;
        cout << "Автор: " << library.books[i].author << endl;
        cout << "Год издания: " << library.books[i].year << endl;
        cout << "Жанр: " << library.books[i].genre << endl;
        cout << "-------------------" << endl;
    }
}

int Task3() {
    Library library;
    library.bookCount = 0;
    
    int choice;
    
    while (true) {
        cout << "\nМеню библиотеки:" << endl;
        cout << "1. Добавить книгу" << endl;
        cout << "2. Удалить книгу по названию" << endl;
        cout << "3. Найти книги по автору" << endl;
        cout << "4. Показать все книги" << endl;
        cout << "5. Выйти" << endl;
        cout << "Выберите действие: ";
        cin >> choice;
        
        if (choice == 1) {
            addBook(library);
        } else if (choice == 2) {
            removeBook(library);
        } else if (choice == 3) {
            searchByAuthor(library);
        } else if (choice == 4) {
            showAllBooks(library);
        } else if (choice == 5) {
            cout << "Программа завершена." << endl;
            break;
        } else {
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }
    
    return 0;
}