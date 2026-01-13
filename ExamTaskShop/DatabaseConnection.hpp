// DatabaseConnection.hpp
#pragma once
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <pqxx/pqxx>

class DatabaseConnection {
private:
    std::unique_ptr<pqxx::connection> connection;
    std::unique_ptr<pqxx::work> currentTransaction;
    std::string lastTransactionStatus;

public:
    // Конструктор
    DatabaseConnection(const std::string &connectionString) {
        try {
            connection = std::make_unique<pqxx::connection>(connectionString);
            if (!connection->is_open()) {
                throw std::runtime_error("Не удалось подключиться к базе данных");
            }
            lastTransactionStatus = "No transaction";
        } catch (const std::exception &e) {
            throw std::runtime_error(std::string("Ошибка подключения: ") + e.what());
        }
    }

    // Деструктор
    ~DatabaseConnection() {
        try {
            if (currentTransaction) {
                currentTransaction->abort();
            }
            if (connection && connection->is_open()) {
                connection->close();
            }
        } catch (...) {
            // Игнорируем ошибки в деструкторе
        }
    }

    // Запрет копирования
    DatabaseConnection(const DatabaseConnection &) = delete;

    DatabaseConnection &operator=(const DatabaseConnection &) = delete;

    // Поддержка перемещения
    DatabaseConnection(DatabaseConnection &&) = default;

    DatabaseConnection &operator=(DatabaseConnection &&) = default;

    // Выполнение SQL-запроса для выборки данных
    std::vector<std::vector<std::string> > executeQuery(const std::string &query) {
        if (!connection || !connection->is_open()) {
            throw std::runtime_error("Соединение с базой данных не установлено");
        }

        try {
            pqxx::work transaction(*connection);
            pqxx::result result = transaction.exec(query);

            std::vector<std::vector<std::string> > rows;
            rows.reserve(result.size());

            for (const auto &row: result) {
                std::vector<std::string> columns;
                columns.reserve(row.size());

                for (const auto &field: row) {
                    columns.push_back(field.c_str());
                }
                rows.push_back(std::move(columns));
            }

            transaction.commit();
            lastTransactionStatus = "Committed";
            return rows;
        } catch (const std::exception &e) {
            lastTransactionStatus = std::string("Query failed: ") + e.what();
            throw std::runtime_error(std::string("Ошибка выполнения запроса: ") + e.what());
        }
    }

    // Выполнение параметризованного запроса
    template<typename... Args>
    std::vector<std::vector<std::string> > executeQuery(const std::string &query, Args &&... args) {
        if (!connection || !connection->is_open()) {
            throw std::runtime_error("Соединение с базой данных не установлено");
        }

        try {
            pqxx::work transaction(*connection);
            pqxx::result result = transaction.exec(query, pqxx::params(std::forward<Args>(args)...));

            std::vector<std::vector<std::string> > rows;
            rows.reserve(result.size());

            for (const auto &row: result) {
                std::vector<std::string> columns;
                columns.reserve(row.size());

                for (const auto &field: row) {
                    columns.push_back(field.c_str());
                }
                rows.push_back(std::move(columns));
            }

            transaction.commit();
            lastTransactionStatus = "Committed";
            return rows;
        } catch (const std::exception &e) {
            lastTransactionStatus = std::string("Query failed: ") + e.what();
            throw std::runtime_error(std::string("Ошибка выполнения запроса: ") + e.what());
        }
    }

    // Выполнение SQL-запросов без возврата данных
    void executeNonQuery(const std::string &query) {
        if (!connection || !connection->is_open()) {
            throw std::runtime_error("Соединение с базой данных не установлено");
        }

        try {
            pqxx::work transaction(*connection);
            transaction.exec(query);
            transaction.commit();
            lastTransactionStatus = "Committed";
        } catch (const std::exception &e) {
            lastTransactionStatus = std::string("NonQuery failed: ") + e.what();
            throw std::runtime_error(std::string("Ошибка выполнения запроса: ") + e.what());
        }
    }

    // Начало транзакции
    void beginTransaction() {
        if (!connection || !connection->is_open()) {
            throw std::runtime_error("Соединение с базой данных не установлено");
        }
        if (currentTransaction) {
            throw std::runtime_error("Транзакция уже начата");
        }

        try {
            currentTransaction = std::make_unique<pqxx::work>(*connection);
            lastTransactionStatus = "Active";
        } catch (const std::exception &e) {
            throw std::runtime_error(std::string("Ошибка начала транзакции: ") + e.what());
        }
    }

    // Завершение транзакции
    void commitTransaction() {
        if (!currentTransaction) {
            throw std::runtime_error("Нет активной транзакции");
        }

        try {
            currentTransaction->commit();
            currentTransaction.reset();
            lastTransactionStatus = "Committed";
        } catch (const std::exception &e) {
            currentTransaction->abort();
            currentTransaction.reset();
            lastTransactionStatus = std::string("Commit failed: ") + e.what();
            throw std::runtime_error(std::string("Ошибка завершения транзакции: ") + e.what());
        }
    }

    // Откат транзакции
    void rollbackTransaction() {
        if (!currentTransaction) {
            throw std::runtime_error("Нет активной транзакции");
        }

        try {
            currentTransaction->abort();
            currentTransaction.reset();
            lastTransactionStatus = "Rolled back";
        } catch (const std::exception &e) {
            currentTransaction.reset();
            lastTransactionStatus = std::string("Rollback failed: ") + e.what();
            throw std::runtime_error(std::string("Ошибка отката транзакции: ") + e.what());
        }
    }

    // Создание хранимой процедуры или функции
    void createFunction(const std::string &functionName, const std::string &functionBody) {
        std::string query = "CREATE OR REPLACE FUNCTION " + functionName + " " + functionBody;
        executeNonQuery(query);
    }

    // Создание триггера
    void createTrigger(const std::string &triggerName,
                       const std::string &tableName,
                       const std::string &timing, // BEFORE, AFTER, INSTEAD OF
                       const std::string &events, // INSERT, UPDATE, DELETE
                       const std::string &functionName) {
        std::string query =
                "CREATE OR REPLACE TRIGGER " + triggerName + "\n"
                "    " + timing + " " + events + " ON " + tableName + "\n"
                "    FOR EACH ROW\n"
                "    EXECUTE FUNCTION " + functionName + "()";

        executeNonQuery(query);
    }

    // Проверка состояния текущей транзакции
    std::string getTransactionStatus() const {
        return lastTransactionStatus;
    }

    // Проверка подключения
    bool isConnected() const {
        return connection && connection->is_open();
    }

    // Получение сырого соединения
    pqxx::connection &getRawConnection() {
        if (!connection || !connection->is_open()) {
            throw std::runtime_error("Соединение с базой данных не установлено");
        }
        return *connection;
    }

    // Получение текущей транзакции
    pqxx::work &getCurrentTransaction() {
        if (!currentTransaction) {
            throw std::runtime_error("Нет активной транзакции");
        }
        return *currentTransaction;
    }

    // Выполнение в рамках активной транзакции
    template<typename Func>
    auto executeInTransaction(Func &&func) -> decltype(func(std::declval<pqxx::work &>())) {
        if (!currentTransaction) {
            throw std::runtime_error("Нет активной транзакции");
        }
        return std::forward<Func>(func)(*currentTransaction);
    }

    template<typename... Args>
    int callProcedureReturnInt(const std::string &procedureName, Args &&... args) {
        std::string query = "SELECT " + procedureName + "(";
        for (size_t i = 0; i < sizeof...(args); ++i) {
            query += "$" + std::to_string(i + 1);
            if (i != sizeof...(args) - 1) query += ", ";
        }
        query += ")";

        auto result = executeQuery(query, std::forward<Args>(args)...);
        if (!result.empty() && !result[0].empty()) {
            return std::stoi(result[0][0]);
        }
        throw std::runtime_error("Процедура не вернула результат");
    }

    // Вызов хранимой процедуры
    template<typename... Args>
    pqxx::result callProcedure(const std::string& procedureName, Args&&... args) {
        std::string query = "CALL " + procedureName + "(";
        for (size_t i = 0; i < sizeof...(args); ++i) {
            query += "$" + std::to_string(i + 1);
            if (i != sizeof...(args) - 1) query += ", ";
        }
        query += ")";

        return executeQuery(query, std::forward<Args>(args)...);
    }

    // Альтернативно для функций, которые возвращают значение:
    template<typename... Args>
    pqxx::result callFunction(const std::string& functionName, Args&&... args) {
        std::string query = "SELECT " + functionName + "(";
        for (size_t i = 0; i < sizeof...(args); ++i) {
            query += "$" + std::to_string(i + 1);
            if (i != sizeof...(args) - 1) query += ", ";
        }
        query += ")";

        return executeQuery(query, std::forward<Args>(args)...);
    }
};
