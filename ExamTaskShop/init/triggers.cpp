// triggers.cpp
#include "../DatabaseConnection.hpp"
#include <iostream>

bool createTriggers(DatabaseConnection& db) {
    try {
        std::cout << "\nСоздание триггеров..." << std::endl;

        // 1. Триггер для обновления order_date
        std::cout << "1. Создание триггера для обновления order_date..." << std::endl;
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION update_order_date_on_status_change()
            RETURNS TRIGGER
            LANGUAGE plpgsql
            AS $$
            BEGIN
                IF OLD.status != NEW.status THEN
                    NEW.order_date = CURRENT_TIMESTAMP;
                END IF;
                RETURN NEW;
            END;
            $$;

            DROP TRIGGER IF EXISTS trigger_update_order_date ON orders;
            CREATE TRIGGER trigger_update_order_date
            BEFORE UPDATE ON orders
            FOR EACH ROW
            EXECUTE FUNCTION update_order_date_on_status_change();
        )");
        std::cout << "   Триггер для обновления order_date создан" << std::endl;

        // 2. Триггер для обновления цен
        std::cout << "\n2. Создание триггера для учета изменений цен..." << std::endl;
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION update_order_total_on_price_change()
            RETURNS TRIGGER
            LANGUAGE plpgsql
            AS $$
            BEGIN
                IF OLD.price != NEW.price THEN
                    -- ... (код функции) ...
                END IF;
                RETURN NEW;
            END;
            $$;

            DROP TRIGGER IF EXISTS trigger_update_order_total ON products;
            CREATE TRIGGER trigger_update_order_total
            AFTER UPDATE OF price ON products
            FOR EACH ROW
            EXECUTE FUNCTION update_order_total_on_price_change();
        )");
        std::cout << "   Триггер для обновления цен создан" << std::endl;

        // 3. Триггер для логирования статусов
        std::cout << "\n3. Создание триггера аудита для изменений статуса..." << std::endl;
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION log_order_status_change()
            RETURNS TRIGGER
            LANGUAGE plpgsql
            AS $$
            BEGIN
                IF OLD.status != NEW.status THEN
                    INSERT INTO order_status_history (
                        order_id, old_status, new_status, changed_by
                    ) VALUES (
                        NEW.order_id, OLD.status, NEW.status,
                        NULLIF(current_setting('app.user_id', TRUE), '')::INTEGER
                    );
                END IF;
                RETURN NEW;
            END;
            $$;

            DROP TRIGGER IF EXISTS trigger_log_status_change ON orders;
            CREATE TRIGGER trigger_log_status_change
            AFTER UPDATE OF status ON orders
            FOR EACH ROW
            EXECUTE FUNCTION log_order_status_change();
        )");
        std::cout << "   Триггер аудита создан" << std::endl;

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при создании триггеров: " << e.what() << std::endl;
        return false;
    }
}