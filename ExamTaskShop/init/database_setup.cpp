// database_setup.cpp
#include "../DatabaseConnection.hpp"
#include <iostream>
#include <vector>

bool createTables(DatabaseConnection& db) {
    try {
        std::cout << "\nСоздание таблиц базы данных..." << std::endl;

        // Список запросов для создания таблиц с использованием лямбда
        std::vector<std::pair<std::string, std::string>> tableQueries = {
            {"users", R"(
                CREATE TABLE IF NOT EXISTS users (
                    user_id SERIAL PRIMARY KEY,
                    name VARCHAR(100) NOT NULL,
                    email VARCHAR(255) UNIQUE NOT NULL,
                    role VARCHAR(20) CHECK (role IN ('admin', 'manager', 'customer')) NOT NULL,
                    password_hash VARCHAR(255) NOT NULL,
                    loyalty_level INTEGER CHECK (loyalty_level IN (0, 1)) DEFAULT 0
                )
            )"},
            {"products", R"(
                CREATE TABLE IF NOT EXISTS products (
                    product_id SERIAL PRIMARY KEY,
                    name VARCHAR(200) NOT NULL,
                    price DECIMAL(10, 2) CHECK (price > 0) NOT NULL,
                    stock_quantity INTEGER CHECK (stock_quantity >= 0) DEFAULT 0 NOT NULL
                )
            )"},
            {"orders", R"(
                CREATE TABLE IF NOT EXISTS orders (
                    order_id SERIAL PRIMARY KEY,
                    user_id INTEGER NOT NULL,
                    status VARCHAR(20) CHECK (status IN ('pending', 'completed', 'canceled', 'returned')) DEFAULT 'pending',
                    total_price DECIMAL(10, 2) CHECK (total_price >= 0) DEFAULT 0,
                    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                    CONSTRAINT fk_user
                        FOREIGN KEY (user_id)
                        REFERENCES users(user_id)
                        ON DELETE CASCADE
                )
            )"},
            {"order_items", R"(
                CREATE TABLE IF NOT EXISTS order_items (
                    order_item_id SERIAL PRIMARY KEY,
                    order_id INTEGER NOT NULL,
                    product_id INTEGER NOT NULL,
                    quantity INTEGER CHECK (quantity > 0) NOT NULL,
                    price DECIMAL(10, 2) CHECK (price >= 0) NOT NULL,
                    CONSTRAINT fk_order
                        FOREIGN KEY (order_id)
                        REFERENCES orders(order_id)
                        ON DELETE CASCADE,
                    CONSTRAINT fk_product
                        FOREIGN KEY (product_id)
                        REFERENCES products(product_id)
                        ON DELETE RESTRICT
                )
            )"},
            {"order_status_history", R"(
                CREATE TABLE IF NOT EXISTS order_status_history (
                    history_id SERIAL PRIMARY KEY,
                    order_id INTEGER NOT NULL,
                    old_status VARCHAR(20),
                    new_status VARCHAR(20) NOT NULL,
                    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                    changed_by INTEGER,
                    reason TEXT,
                    CONSTRAINT fk_order_history
                        FOREIGN KEY (order_id)
                        REFERENCES orders(order_id)
                        ON DELETE CASCADE,
                    CONSTRAINT fk_changed_by
                        FOREIGN KEY (changed_by)
                        REFERENCES users(user_id)
                        ON DELETE SET NULL
                )
            )"},
            {"audit_log", R"(
                CREATE TABLE IF NOT EXISTS audit_log (
                    log_id SERIAL PRIMARY KEY,
                    entity_type VARCHAR(50) CHECK (entity_type IN ('order', 'product', 'user', 'orders', 'products', 'users', 'order_items')) NOT NULL,
                    entity_id INTEGER NOT NULL,
                    operation VARCHAR(20) CHECK (operation IN ('insert', 'update', 'delete')) NOT NULL,
                    performed_by INTEGER,
                    performed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                    CONSTRAINT fk_performed_by
                        FOREIGN KEY (performed_by)
                        REFERENCES users(user_id)
                        ON DELETE SET NULL
                )
            )"}
        };

        // Использование алгоритма for_each с лямбдой для создания таблиц
        std::for_each(tableQueries.begin(), tableQueries.end(),
            [&db](const auto& tableInfo) {
                try {
                    db.executeNonQuery(tableInfo.second);
                    std::cout << "Таблица '" << tableInfo.first << "' создана успешно" << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Ошибка создания таблицы '" << tableInfo.first
                              << "': " << e.what() << std::endl;
                    throw;
                }
            }
        );

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при создании таблиц: " << e.what() << std::endl;
        return false;
    }
}


bool createIndexes(DatabaseConnection& db) {
    try {
        std::cout << "\nСоздание индексов..." << std::endl;
        db.beginTransaction();

        db.executeInTransaction([](pqxx::work& transaction) {
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_users_email ON users(email)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_users_role ON users(role)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_products_price ON products(price)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_orders_user_id ON orders(user_id)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_orders_status ON orders(status)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_orders_date ON orders(order_date)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_order_items_order_id ON order_items(order_id)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_order_items_product_id ON order_items(product_id)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_status_history_order_id ON order_status_history(order_id)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_audit_log_entity ON audit_log(entity_type, entity_id)");
            transaction.exec("CREATE INDEX IF NOT EXISTS idx_audit_log_date ON audit_log(performed_at)");
        });

        db.commitTransaction();
        std::cout << "Индексы созданы успешно" << std::endl;
        return true;
    } catch (const std::exception& e) {
        db.rollbackTransaction();
        std::cerr << "Ошибка при создании индексов: " << e.what() << std::endl;
        return false;
    }
}

void verifyDatabaseSetup(DatabaseConnection& db) {
    try {
        std::cout << "\nПроверка создания таблиц..." << std::endl;
        auto tables = db.executeQuery(R"(
            SELECT table_name
            FROM information_schema.tables
            WHERE table_schema = 'public'
            AND table_type = 'BASE TABLE'
            ORDER BY table_name
        )");

        std::cout << "Созданные таблицы:" << std::endl;
        for (const auto& row : tables) {
            std::cout << "  - " << row[0] << std::endl;
        }

        std::cout << "\nПроверка ограничений..." << std::endl;
        auto constraints = db.executeQuery(R"(
            SELECT table_name, constraint_name, constraint_type
            FROM information_schema.table_constraints
            WHERE table_schema = 'public'
            AND table_name IN ('users', 'products', 'orders', 'order_items')
            ORDER BY table_name, constraint_type
        )");

        std::cout << "Созданные ограничения:" << std::endl;
        for (const auto& row : constraints) {
            std::cout << "  " << row[0] << " -> " << row[1] << " (" << row[2] << ")" << std::endl;
        }

        std::cout << "\nВсе таблицы успешно созданы!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при проверке базы данных: " << e.what() << std::endl;
    }
}