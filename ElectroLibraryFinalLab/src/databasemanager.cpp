// Реализация DatabaseManager.
// Все SQL-запросы — через prepared statements (prepare + bindValue).
// Конкатенация строк в SQL запрещена (защита от SQL-инъекций).

#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent) {}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen()) m_db.close();
}

bool DatabaseManager::open()
{
    // Путь к файлу БД: ~/.local/share/BMSTU/LibraryCatalog/library.db (Linux)
    const QString dir = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    const QString path = dir + "/library.db";

    // QSQLITE — встроенный драйвер SQLite в Qt, не требует установки
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        return false;
    }
    return createSchemaIfNeeded();
}

bool DatabaseManager::createSchemaIfNeeded()
{
    QSqlQuery q(m_db);

    // Создаём таблицу books, если её ещё нет (CREATE IF NOT EXISTS)
    const QString sql = R"(
        CREATE TABLE IF NOT EXISTS books (
            id       INTEGER PRIMARY KEY AUTOINCREMENT,
            title    TEXT NOT NULL,
            author   TEXT NOT NULL,
            year     INTEGER,
            isbn     TEXT UNIQUE,
            genre    TEXT,
            status   TEXT NOT NULL DEFAULT 'to_read',
            rating   INTEGER DEFAULT 0,
            pages    INTEGER,
            notes    TEXT,
            added_at TEXT NOT NULL DEFAULT (datetime('now'))
        )
    )";
    if (!q.exec(sql)) {
        m_lastError = q.lastError().text();
        return false;
    }

    // Индексы для ускорения фильтрации по статусу и поиска по автору
    if (!q.exec("CREATE INDEX IF NOT EXISTS idx_books_status ON books(status)")) {
        m_lastError = q.lastError().text();
        return false;
    }
    if (!q.exec("CREATE INDEX IF NOT EXISTS idx_books_author ON books(author)")) {
        m_lastError = q.lastError().text();
        return false;
    }

    return true;
}

int DatabaseManager::addBook(const Book &b)
{
    QSqlQuery q(m_db);
    q.prepare(R"(
        INSERT INTO books (title, author, year, isbn, genre, status, rating, pages, notes)
        VALUES (:title, :author, :year, :isbn, :genre, :status, :rating, :pages, :notes)
    )");
    q.bindValue(":title",  b.title);
    q.bindValue(":author", b.author);
    q.bindValue(":year",   b.year);
    // Пустой ISBN записываем как NULL, чтобы UNIQUE-ограничение не срабатывало
    q.bindValue(":isbn",   b.isbn.isEmpty() ? QVariant() : b.isbn);
    q.bindValue(":genre",  b.genre);
    q.bindValue(":status", b.status);
    q.bindValue(":rating", b.rating);
    q.bindValue(":pages",  b.pages);
    q.bindValue(":notes",  b.notes);

    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return -1;
    }
    // lastInsertId() возвращает id только сразу после INSERT
    return q.lastInsertId().toInt();
}

bool DatabaseManager::updateBook(const Book &b)
{
    QSqlQuery q(m_db);
    q.prepare(R"(
        UPDATE books SET title=:title, author=:author, year=:year, isbn=:isbn,
            genre=:genre, status=:status, rating=:rating, pages=:pages, notes=:notes
        WHERE id=:id
    )");
    q.bindValue(":title",  b.title);
    q.bindValue(":author", b.author);
    q.bindValue(":year",   b.year);
    q.bindValue(":isbn",   b.isbn.isEmpty() ? QVariant() : b.isbn);
    q.bindValue(":genre",  b.genre);
    q.bindValue(":status", b.status);
    q.bindValue(":rating", b.rating);
    q.bindValue(":pages",  b.pages);
    q.bindValue(":notes",  b.notes);
    q.bindValue(":id",     b.id);

    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::removeBook(int id)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM books WHERE id=:id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    return true;
}

std::optional<Book> DatabaseManager::getBookById(int id)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, title, author, year, isbn, genre, status, rating, pages, notes FROM books WHERE id=:id");
    q.bindValue(":id", id);
    if (!q.exec() || !q.next())
        return std::nullopt;

    Book b;
    b.id     = q.value(0).toInt();
    b.title  = q.value(1).toString();
    b.author = q.value(2).toString();
    b.year   = q.value(3).toInt();
    b.isbn   = q.value(4).toString();
    b.genre  = q.value(5).toString();
    b.status = q.value(6).toString();
    b.rating = q.value(7).toInt();
    b.pages  = q.value(8).toInt();
    b.notes  = q.value(9).toString();
    return b;
}

bool DatabaseManager::existsByIsbn(const QString &isbn)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT COUNT(*) FROM books WHERE isbn=:isbn");
    q.bindValue(":isbn", isbn);
    if (!q.exec() || !q.next())
        return false;
    return q.value(0).toInt() > 0;
}

QMap<QString, int> DatabaseManager::countByStatus()
{
    // Инициализируем все три статуса нулями на случай, если какого-то нет в БД
    QMap<QString, int> result;
    result["to_read"] = 0;
    result["reading"] = 0;
    result["done"]    = 0;

    QSqlQuery q(m_db);
    q.prepare("SELECT status, COUNT(*) FROM books GROUP BY status");
    if (q.exec()) {
        while (q.next())
            result[q.value(0).toString()] = q.value(1).toInt();
    }
    return result;
}

QMap<int, int> DatabaseManager::countByYear()
{
    QMap<int, int> result;
    QSqlQuery q(m_db);
    // Пропускаем книги без года (year = 0)
    q.prepare("SELECT year, COUNT(*) FROM books WHERE year > 0 GROUP BY year ORDER BY year");
    if (q.exec()) {
        while (q.next())
            result[q.value(0).toInt()] = q.value(1).toInt();
    }
    return result;
}

QList<Book> DatabaseManager::allBooks()
{
    QList<Book> list;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, title, author, year, isbn, genre, status, rating, pages, notes FROM books");
    if (q.exec()) {
        while (q.next()) {
            Book b;
            b.id     = q.value(0).toInt();
            b.title  = q.value(1).toString();
            b.author = q.value(2).toString();
            b.year   = q.value(3).toInt();
            b.isbn   = q.value(4).toString();
            b.genre  = q.value(5).toString();
            b.status = q.value(6).toString();
            b.rating = q.value(7).toInt();
            b.pages  = q.value(8).toInt();
            b.notes  = q.value(9).toString();
            list.append(b);
        }
    }
    return list;
}
