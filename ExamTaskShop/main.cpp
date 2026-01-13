// main.cpp
#include <iostream>
#include "DatabaseConnection.hpp"
#include "init/database_setup.hpp"
#include "init/stored_procedure.hpp"
#include "init/triggers.hpp"
#include "init/functions.hpp"
#include "init/audit_system.hpp"
#include "init/business_logic.hpp"
#include "init/order_operations.hpp"

// Include files not compiled separately by CMake
#include "DatabaseManager.cpp"
#include "HistoryAuditManager.cpp"
#include "ReportSystem.cpp"

void testExamples(DatabaseConnection& db);

// Функция для создания базы данных, если она не существует
bool ensureDatabaseExists(const std::string& dbName, const std::string& user, const std::string& password) {
    try {
        std::cout << "Проверка существования базы данных '" << dbName << "'..." << std::endl;

        // Подключаемся к системной базе данных postgres
        DatabaseConnection postgresDb(
            "host=localhost port=5432 dbname=postgres user=" + user + " password=" + password);

        if (!postgresDb.isConnected()) {
            std::cerr << "Не удалось подключиться к системной базе данных postgres" << std::endl;
            return false;
        }

        // Проверяем существование базы данных
        auto result = postgresDb.executeQuery(
            "SELECT 1 FROM pg_database WHERE datname = $1", dbName);

        if (result.empty()) {
            // База данных не существует - создаём её
            std::cout << "База данных '" << dbName << "' не найдена. Создание..." << std::endl;

            // CREATE DATABASE нельзя выполнить внутри транзакции,
            // используем отдельное подключение с autocommit
            pqxx::connection conn("host=localhost port=5432 dbname=postgres user=" + user + " password=" + password);
            pqxx::nontransaction ntx(conn);
            ntx.exec("CREATE DATABASE " + dbName + " ENCODING 'UTF8'");

            std::cout << "База данных '" << dbName << "' успешно создана!" << std::endl;
        } else {
            std::cout << "База данных '" << dbName << "' уже существует." << std::endl;
        }

        return true;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при проверке/создании базы данных: " << e.what() << std::endl;
        return false;
    }
}

int main() {
  try {
        // Сначала проверяем/создаём базу данных
        if (!ensureDatabaseExists("online_store", "postgres", "288288")) {
            std::cerr << "Не удалось создать базу данных" << std::endl;
            return 1;
        }

        // Создаем подключение для настройки
        DatabaseConnection setupDb(
            "host=localhost port=5432 dbname=online_store user=postgres password=288288");

        if (!setupDb.isConnected()) {
            std::cerr << "Не удалось подключиться к базе данных" << std::endl;
            return 1;
        }

        std::cout << "Подключение к базе данных успешно установлено" << std::endl;

        // Настройка базы данных
        std::cout << "\n=== НАСТРОЙКА БАЗЫ ДАННЫХ ===" << std::endl;

        if (!createTables(setupDb)) return 1;
        if (!createIndexes(setupDb)) return 1;
        verifyDatabaseSetup(setupDb);
        if (!createStoredProcedures(setupDb)) return 1;
        if (!createTriggers(setupDb)) return 1;
        if (!createFunctions(setupDb)) return 1;
        if (!setupAuditSystem(setupDb)) return 1;
        setupReportSystem(setupDb);

        // Создаем подключение для демонстрации
        auto demoDb = std::make_shared<DatabaseConnection>(
            "host=localhost port=5432 dbname=online_store user=postgres password=288288");

        if (!demoDb->isConnected()) {
            std::cerr << "Не удалось создать подключение для демонстрации" << std::endl;
            return 1;
        }

        // Создание тестовых данных ПЕРЕД демонстрациями
        std::cout << "\n\n=== СОЗДАНИЕ ТЕСТОВЫХ ДАННЫХ ===" << std::endl;

        // Тестовые пользователи
        setupDb.executeNonQuery(R"(
            INSERT INTO users (name, email, role, password_hash) VALUES
            ('Иван Покупатель', 'ivan@example.com', 'customer', 'hash1'),
            ('Мария Менеджер', 'maria@example.com', 'manager', 'hash2'),
            ('Админ Системы', 'admin@example.com', 'admin', 'hash3')
            ON CONFLICT (email) DO NOTHING
        )");
        std::cout << "Тестовые пользователи созданы" << std::endl;

        // Тестовые товары
        setupDb.executeNonQuery(R"(
            INSERT INTO products (name, price, stock_quantity) VALUES
            ('Ноутбук', 75000.00, 10),
            ('Смартфон', 45000.00, 25),
            ('Наушники', 5000.00, 50),
            ('Клавиатура', 8000.00, 30)
            ON CONFLICT DO NOTHING
        )");
        std::cout << "Тестовые товары созданы" << std::endl;

        // Тестовые заказы
        auto userResult = setupDb.executeQuery("SELECT user_id FROM users LIMIT 1");
        if (!userResult.empty()) {
            int testUserId = std::stoi(userResult[0][0]);
            setupDb.executeQuery(
                "INSERT INTO orders (user_id, status, total_price) VALUES ($1, 'pending', 80000.00), "
                "($1, 'completed', 45000.00) ON CONFLICT DO NOTHING", testUserId);
            std::cout << "Тестовые заказы созданы" << std::endl;
        }

        // Демонстрация систем
        std::cout << "\n\n=== ДЕМОНСТРАЦИЯ БИЗНЕС-ЛОГИКИ ===" << std::endl;
        demonstrateBusinessLogic(demoDb);

        std::cout << "\n\n=== ДЕМОНСТРАЦИЯ СИСТЕМЫ ЗАКАЗОВ ===" << std::endl;
        OrderFacade orderSystem(demoDb);
        orderSystem.demonstrateOrderSystem();

        std::cout << "\n\n=== ДЕМОНСТРАЦИЯ СИСТЕМЫ ИСТОРИИ И АУДИТА ===" << std::endl;
        demonstrateHistoryAuditSystem(demoDb);

        // Создаем администратора для демонстрации отчетов
        std::cout << "\n\n=== СОЗДАНИЕ АДМИНИСТРАТОРА ДЛЯ ОТЧЕТОВ ===" << std::endl;

        setupDb.executeNonQuery(R"(
            INSERT INTO users (name, email, role, password_hash)
            VALUES ('Администратор Отчетов', 'admin_reports@example.com', 'admin', 'admin_hash')
            ON CONFLICT (email) DO NOTHING
        )");

        auto adminResult = setupDb.executeQuery(
            "SELECT user_id FROM users WHERE email = 'admin_reports@example.com'");

        if (!adminResult.empty()) {
            int adminId = std::stoi(adminResult[0][0]);

            std::cout << "\n\n=== ДЕМОНСТРАЦИЯ СИСТЕМЫ ОТЧЕТОВ ===" << std::endl;
            ReportManager reportManager(demoDb);
            reportManager.demonstrateReportsSystem(adminId);
        } else {
            std::cerr << "Не удалось создать администратора для отчетов" << std::endl;
        }

        // Тестовые примеры
        std::cout << "\n\n=== ТЕСТОВЫЕ ПРИМЕРЫ ===" << std::endl;
        testExamples(setupDb);

        std::cout << "\n\n=== СИСТЕМА ИНТЕРНЕТ-МАГАЗИНА УСПЕШНО СОЗДАНА ===" << std::endl;
        std::cout << "Все компоненты успешно настроены!" << std::endl;
        std::cout << "\nСозданные файлы отчетов:" << std::endl;
        std::cout << "  - demo_order_history.csv" << std::endl;
        std::cout << "  - demo_order_history.csv.summary.txt" << std::endl;
        std::cout << "  - demo_user_activity.csv" << std::endl;
        std::cout << "  - demo_returns.csv" << std::endl;
        std::cout << "  - test_order_history.csv" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

void testExamples(DatabaseConnection& db) {
    try {
        std::cout << "\n\nТестирование системы..." << std::endl;

        // Пример 1: Создание заказа через процедуру
        std::cout << "\n1. Тестирование создания заказа..." << std::endl;
        try {
            std::string items_json = R"([
                {"product_id": 1, "quantity": 2},
                {"product_id": 2, "quantity": 1}
            ])";

            auto result = db.executeQuery("SELECT create_order($1, $2, $3)",
                                          1, items_json, "credit_card");

            if (!result.empty()) {
                int order_id = std::stoi(result[0][0]);
                std::cout << "   Заказ создан! ID: " << order_id << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "   Ошибка: " << e.what() << std::endl;
        }

        // Пример 2: Получение статуса заказа
        std::cout << "\n2. Получение статуса заказа..." << std::endl;
        try {
            auto status = db.executeQuery("SELECT get_order_status(1)");
            if (!status.empty()) {
                std::cout << "   Статус заказа 1: " << status[0][0] << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "   Ошибка: " << e.what() << std::endl;
        }

        // Пример 3: Получение статистики пользователей
        std::cout << "\n3. Статистика заказов пользователей..." << std::endl;
        try {
            auto stats = db.executeQuery("SELECT * FROM get_user_order_count() LIMIT 3");
            if (!stats.empty()) {
                for (const auto& row : stats) {
                    std::cout << "   " << row[1] << ": " << row[3] << " заказов" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "   Ошибка: " << e.what() << std::endl;
        }

        // Пример 4: Тестирование аудита
        std::cout << "\n4. Тестирование системы аудита..." << std::endl;
        try {
            // Устанавливаем контекст пользователя
            db.executeNonQuery("SELECT set_config('app.user_id', '1', FALSE)");

            // Создаем тестовый товар
            db.executeNonQuery(
                "INSERT INTO products (name, price, stock_quantity) "
                "VALUES ('Тестовый товар для аудита', 50.00, 20)"
            );

            // Получаем аудит-лог
            auto audit_log = db.executeQuery("SELECT * FROM get_audit_log_by_user(1) LIMIT 3");
            if (!audit_log.empty()) {
                std::cout << "   Операции пользователя 1:" << std::endl;
                for (const auto& row : audit_log) {
                    std::cout << "   - " << row[1] << " #" << row[2]
                              << " (" << row[3] << ")" << std::endl;
                }
            }

            // Сбрасываем контекст
            db.executeNonQuery("SELECT set_config('app.user_id', '', FALSE)");
        } catch (const std::exception& e) {
            std::cerr << "   Ошибка: " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при тестировании: " << e.what() << std::endl;
    }
}