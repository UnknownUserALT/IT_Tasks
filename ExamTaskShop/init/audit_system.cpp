// audit_system.cpp
#include "../DatabaseConnection.hpp"
#include <iostream>

bool setupAuditSystem(DatabaseConnection& db) {
    try {
        std::cout << "\nНастройка системы аудита..." << std::endl;

        // Триггерная функция для аудита
        std::cout << "1. Создание триггерной функции для аудита..." << std::endl;
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION audit_trigger_function()
            RETURNS TRIGGER
            LANGUAGE plpgsql
            AS $$
            DECLARE
                v_operation VARCHAR;
                v_entity_id INTEGER;
                v_user_id INTEGER;
            BEGIN
                IF TG_OP = 'INSERT' THEN
                    v_operation := 'insert';
                ELSIF TG_OP = 'UPDATE' THEN
                    v_operation := 'update';
                ELSIF TG_OP = 'DELETE' THEN
                    v_operation := 'delete';
                END IF;

                -- Определяем ID в зависимости от таблицы
                IF TG_TABLE_NAME = 'users' THEN
                    IF TG_OP = 'DELETE' THEN v_entity_id := OLD.user_id;
                    ELSE v_entity_id := NEW.user_id; END IF;
                ELSIF TG_TABLE_NAME = 'products' THEN
                    IF TG_OP = 'DELETE' THEN v_entity_id := OLD.product_id;
                    ELSE v_entity_id := NEW.product_id; END IF;
                ELSIF TG_TABLE_NAME = 'orders' THEN
                    IF TG_OP = 'DELETE' THEN v_entity_id := OLD.order_id;
                    ELSE v_entity_id := NEW.order_id; END IF;
                ELSIF TG_TABLE_NAME = 'order_items' THEN
                    IF TG_OP = 'DELETE' THEN v_entity_id := OLD.order_item_id;
                    ELSE v_entity_id := NEW.order_item_id; END IF;
                END IF;

                v_user_id := NULLIF(current_setting('app.user_id', TRUE), '')::INTEGER;

                INSERT INTO audit_log (
                    entity_type, entity_id, operation, performed_by, performed_at
                ) VALUES (
                    TG_TABLE_NAME, v_entity_id, v_operation, v_user_id, CURRENT_TIMESTAMP
                );

                IF TG_OP = 'DELETE' THEN
                    RETURN OLD;
                ELSE
                    RETURN NEW;
                END IF;
            END;
            $$;
        )");
        std::cout << "   Триггерная функция создана" << std::endl;

        // Применение триггеров
        std::cout << "\n2. Применение триггеров аудита..." << std::endl;

        std::vector<std::string> tables = {"users", "products", "orders", "order_items"};
        for (const auto& table : tables) {
            db.executeNonQuery("DROP TRIGGER IF EXISTS trg_audit_" + table + " ON " + table);
            db.executeNonQuery(
                "CREATE TRIGGER trg_audit_" + table + " " +
                "AFTER INSERT OR UPDATE OR DELETE ON " + table + " " +
                "FOR EACH ROW " +
                "EXECUTE FUNCTION audit_trigger_function()"
            );
            std::cout << "   Аудит настроен для таблицы: " << table << std::endl;
        }

        // Функция для получения аудит-лога
        std::cout << "\n3. Создание функций аудита..." << std::endl;

        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION get_audit_log_by_user(p_user_id INTEGER)
            RETURNS TABLE(
                log_id INTEGER,
                entity_type VARCHAR,
                entity_id INTEGER,
                operation VARCHAR,
                performed_by INTEGER,
                performed_by_name VARCHAR,
                performed_at TIMESTAMP,
                entity_details TEXT
            )
            LANGUAGE plpgsql
            AS $$
            BEGIN
                RETURN QUERY
                SELECT
                    al.log_id,
                    al.entity_type,
                    al.entity_id,
                    al.operation,
                    al.performed_by,
                    u.name as performed_by_name,
                    al.performed_at,
                    CASE al.entity_type
                        WHEN 'users' THEN 'Пользователь: ' || (SELECT name FROM users WHERE user_id = al.entity_id)
                        WHEN 'products' THEN 'Товар: ' || (SELECT name FROM products WHERE product_id = al.entity_id)
                        WHEN 'orders' THEN 'Заказ #' || al.entity_id::TEXT || ' - ' ||
                            (SELECT status FROM orders WHERE order_id = al.entity_id)
                        WHEN 'order_items' THEN 'Элемент заказа #' || al.entity_id::TEXT
                        ELSE 'Сущность #' || al.entity_id::TEXT
                    END as entity_details
                FROM audit_log al
                LEFT JOIN users u ON al.performed_by = u.user_id
                WHERE al.performed_by = p_user_id
                ORDER BY al.performed_at DESC;
            END;
            $$;
        )");
        std::cout << "   Функция get_audit_log_by_user создана" << std::endl;

        // Функция get_audit_log с фильтрами
        db.executeNonQuery("DROP FUNCTION IF EXISTS get_audit_log(TIMESTAMP, TIMESTAMP, VARCHAR, VARCHAR, INTEGER);");
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION get_audit_log(
                p_start_date TIMESTAMP DEFAULT NULL,
                p_end_date TIMESTAMP DEFAULT NULL,
                p_entity_type VARCHAR DEFAULT NULL,
                p_operation VARCHAR DEFAULT NULL,
                p_performed_by INTEGER DEFAULT NULL
            )
            RETURNS TABLE(
                log_id INTEGER,
                entity_type VARCHAR,
                entity_id INTEGER,
                operation VARCHAR,
                performed_by INTEGER,
                performed_at TIMESTAMP
            )
            LANGUAGE plpgsql
            AS $$
            BEGIN
                RETURN QUERY
                SELECT
                    al.log_id,
                    al.entity_type,
                    al.entity_id,
                    al.operation,
                    al.performed_by,
                    al.performed_at
                FROM audit_log al
                WHERE (p_start_date IS NULL OR al.performed_at >= p_start_date)
                  AND (p_end_date IS NULL OR al.performed_at <= p_end_date)
                  AND (p_entity_type IS NULL OR al.entity_type = p_entity_type)
                  AND (p_operation IS NULL OR al.operation = p_operation)
                  AND (p_performed_by IS NULL OR al.performed_by = p_performed_by)
                ORDER BY al.performed_at DESC;
            END;
            $$;
        )");
        std::cout << "   Функция get_audit_log создана" << std::endl;

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при настройке аудита: " << e.what() << std::endl;
        return false;
    }
}