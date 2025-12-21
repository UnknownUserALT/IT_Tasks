package main.java.library;

import java.util.List;

public class Main {
    public static void main(String[] args) {

        Library library = new Library(); // Добавление книг
        // Создание книг
        Book book1 = new Book(1, "Война и мир", "Л.Н. Толстой", 1869, "978-5-17-090335-2");
        Book book2 = new Book(2, "Преступление и наказание", "Ф.М. Достоевский", 1866, "978-5-04-089695-7");
        Book book3 = new Book(3, "Анна Каренина", "Л.Н. Толстой", 1877, "978-5-17-089680-0");

        // Добавление книг в библиотеку
        library.addBook(book1);
        library.addBook(book2);
        library.addBook(book3);

        // Попытка добавить книгу с существующим ID
        Book book4 = new Book(1, "Дубровский", "А.С. Пушкин", 1841, "978-5-17-123456-7");
        library.addBook(book4); // Эта операция не выполнится

        // Поиск книг по автору
        System.out.println("Книги Л.Н. Толстого:");
        List<Book> tolstoyBooks = library.findBooksByAuthor("Л.Н. Толстой");
        for (Book book : tolstoyBooks) {
            System.out.println("  - " + book.getTitle());
        }

        // Выдача книг
        System.out.println("\nВыдача книг:");
        library.borrowBook(1);
        library.borrowBook(2);
        library.borrowBook(1); // Попытка повторной выдачи

        // Просмотр доступных книг
        System.out.println("\nДоступные книги:");
        List<Book> availableBooks = library.getAvailableBooks();
        for (Book book : availableBooks) {
            System.out.println("  - " + book.getTitle());
        }

        // Возврат книги
        System.out.println("\nВозврат книги:");
        library.returnBook(1);

        // Вывод журнала операций
        System.out.println("\nЖурнал операций библиотеки:");
        library.printOperationLog();
    }
}