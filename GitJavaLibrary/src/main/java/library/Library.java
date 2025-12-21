package main.java.library;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

public class Library {
    private List<Book> books;
    private OperationLog operationLog;

    public Library() {
        this.books = new ArrayList<>();
        this.operationLog = new OperationLog();
        operationLog.addEntry(OperationLog.OperationType.ADD_BOOK, "Библиотека инициализирована");
    }

    public String getStatistics() {
        int totalBooks = books.size();
        int availableBooks = 0;
        int borrowedBooks = 0;

        for (Book book : books) {
            if (book.isAvailable()) {
                availableBooks++;
            } else {
                borrowedBooks++;
            }
        }

        return String.format(
                "=== СТАТИСТИКА БИБЛИОТЕКИ ===\n" +
                        "Всего книг: %d\n" +
                        "Доступно: %d\n" +
                        "Выдано: %d\n" +
                        "=============================",
                totalBooks, availableBooks, borrowedBooks
        );
    }

    // Метод для добавления книги
    public boolean addBook(Book book) {
        if (findBookById(book.getId()) != null) {
            operationLog.addEntry(OperationLog.OperationType.ADD_BOOK,
                    "Ошибка: книга с ID=" + book.getId() + " уже существует");
            return false;
        }

        books.add(book);
        operationLog.addEntry(OperationLog.OperationType.ADD_BOOK,
                "Добавлена книга: ID=" + book.getId() + ", \"" + book.getTitle() + "\"");
        return true;
    }

    // Метод для поиска книги по ID
    public Book findBookById(int id) {
        for (Book book : books) {
            if (book.getId() == id) {
                return book;
            }
        }
        return null;
    }

    // Метод для поиска книг по автору
    public List<Book> findBooksByAuthor(String author) {
        List<Book> result = new ArrayList<>();
        for (Book book : books) {
            if (book.getAuthor().equalsIgnoreCase(author)) {
                result.add(book);
            }
        }
        return result;
    }

    // Метод для выдачи книги
    public boolean borrowBook(int id) {
        Book book = findBookById(id);
        if (book == null) {
            operationLog.addEntry(OperationLog.OperationType.BORROW,
                    "Ошибка: книга с ID=" + id + " не найдена");
            return false;
        }

        if (!book.isAvailable()) {
            operationLog.addEntry(OperationLog.OperationType.BORROW,
                    "Ошибка: книга ID=" + id + " \"" + book.getTitle() + "\" уже выдана");
            return false;
        }

        book.setAvailable(false);
        operationLog.addEntry(OperationLog.OperationType.BORROW,
                "Книга выдана: ID=" + id + ", \"" + book.getTitle() + "\"");
        return true;
    }

    // Метод для возврата книги
    public boolean returnBook(int id) {
        Book book = findBookById(id);
        if (book == null) {
            operationLog.addEntry(OperationLog.OperationType.RETURN,
                    "Ошибка: книга с ID=" + id + " не найдена");
            return false;
        }

        if (book.isAvailable()) {
            operationLog.addEntry(OperationLog.OperationType.RETURN,
                    "Ошибка: книга ID=" + id + " \"" + book.getTitle() + "\" уже в библиотеке");
            return false;
        }

        book.setAvailable(true);
        operationLog.addEntry(OperationLog.OperationType.RETURN,
                "Книга возвращена: ID=" + id + ", \"" + book.getTitle() + "\"");
        return true;
    }

    // Метод для получения списка доступных книг
    public List<Book> getAvailableBooks() {
        List<Book> availableBooks = new ArrayList<>();
        for (Book book : books) {
            if (book.isAvailable()) {
                availableBooks.add(book);
            }
        }
        return availableBooks;
    }

    // Метод для вывода журнала операций
    public void printOperationLog() {
        operationLog.printLog();
    }

    // Вложенный статический класс OperationLog
    public static class OperationLog {
        // Enum для типов операций
        public enum OperationType {
            ADD_BOOK("Добавление книги"),
            BORROW("Выдача книги"),
            RETURN("Возврат книги");

            private final String description;

            OperationType(String description) {
                this.description = description;
            }

            public String getDescription() {
                return description;
            }
        }

        // Внутренний класс для записи операции
        public class LogEntry {
            private OperationType type;
            private LocalDateTime timestamp;
            private String description;

            public LogEntry(OperationType type, String description) {
                this.type = type;
                this.timestamp = LocalDateTime.now();
                this.description = description;
            }

            public OperationType getType() {
                return type;
            }

            public LocalDateTime getTimestamp() {
                return timestamp;
            }

            public String getDescription() {
                return description;
            }

            @Override
            public String toString() {
                DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
                return String.format("[%s] %s: %s",
                        timestamp.format(formatter),
                        type.getDescription(),
                        description);
            }
        }

        private List<LogEntry> entries;

        public OperationLog() {
            this.entries = new ArrayList<>();
        }

        // Метод для добавления записи в журнал
        public void addEntry(OperationType type, String description) {
            LogEntry entry = new LogEntry(type, description);
            entries.add(entry);
        }

        // Метод для получения всех записей
        public List<LogEntry> getEntries() {
            return new ArrayList<>(entries);
        }

        // Метод для вывода журнала
        public void printLog() {
            System.out.println("=== ЖУРНАЛ ОПЕРАЦИЙ БИБЛИОТЕКИ ===");
            System.out.println("Всего записей: " + entries.size());
            System.out.println("---------------------------------");

            for (LogEntry entry : entries) {
                System.out.println(entry);
            }

            System.out.println("=================================");
        }
    }
}