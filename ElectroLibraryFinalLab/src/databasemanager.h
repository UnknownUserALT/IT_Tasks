// DatabaseManager — единственная точка доступа к SQLite.
// Инкапсулирует все SQL-запросы; в UI-коде не должно быть ни одной строки SQL.
// Все запросы используют prepared statements (QSqlQuery::prepare + bindValue).

#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QMap>
#include <optional>
#include "book.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager() override;

    /// Открывает (или создаёт) файл БД в AppDataLocation и инициализирует схему
    bool open();

    /// Текст последней ошибки (для вывода пользователю)
    QString lastError() const { return m_lastError; }

    /// Возвращает объект соединения (нужен для QSqlTableModel и транзакций)
    QSqlDatabase database() const { return m_db; }

    // ─── CRUD-методы ───

    /// Добавляет книгу в БД. Возвращает id вставленной записи или -1 при ошибке.
    int  addBook(const Book &b);

    /// Обновляет существующую книгу по b.id. Возвращает true при успехе.
    bool updateBook(const Book &b);

    /// Удаляет книгу по id. Возвращает true при успехе.
    bool removeBook(int id);

    /// Возвращает книгу по id или std::nullopt, если не найдена.
    std::optional<Book> getBookById(int id);

    /// Проверяет, есть ли книга с таким ISBN (для предотвращения дубликатов при импорте)
    bool existsByIsbn(const QString &isbn);

    // ─── Статистика ───

    /// Количество книг по каждому статусу {"to_read": N, "reading": N, "done": N}
    QMap<QString, int> countByStatus();

    /// Количество книг по годам {2020: 3, 2021: 5, ...} — для виджета статистики
    QMap<int, int> countByYear();

    /// Возвращает все книги из БД (для экспорта в JSON)
    QList<Book> allBooks();

private:
    /// Создаёт таблицу books и индексы, если их ещё нет
    bool createSchemaIfNeeded();

    QSqlDatabase m_db;
    QString      m_lastError;
};
