// functions.cpp
#include "../DatabaseConnection.hpp"
#include <iostream>

bool createFunctions(DatabaseConnection& db) {
    try {
        std::cout << "\nСоздание функций..." << std::endl;

        // 1. Функции для работы со статусами
        std::cout << "1. Создание функций для работы со статусами..." << std::endl;

        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION get_order_status_history(p_order_id INTEGER)
            RETURNS TABLE(
                history_id INTEGER,
                order_id INTEGER,
                old_status VARCHAR,
                new_status VARCHAR,
                changed_at TIMESTAMP,
                changed_by INTEGER,
                changed_by_name VARCHAR
            )
            LANGUAGE plpgsql
            AS $$
            BEGIN
                RETURN QUERY
                SELECT
                    h.history_id,
                    h.order_id,
                    h.old_status,
                    h.new_status,
                    h.changed_at,
                    h.changed_by,
                    u.name as changed_by_name
                FROM order_status_history h
                LEFT JOIN users u ON h.changed_by = u.user_id
                WHERE h.order_id = p_order_id
                ORDER BY h.changed_at DESC;

                IF NOT FOUND THEN
                    RETURN QUERY
                    SELECT
                        0 as history_id,
                        o.order_id,
                        NULL as old_status,
                        o.status as new_status,
                        o.order_date as changed_at,
                        o.user_id as changed_by,
                        u.name as changed_by_name
                    FROM orders o
                    JOIN users u ON o.user_id = u.user_id
                    WHERE o.order_id = p_order_id;
                END IF;
            END;
            $$;
        )");
        std::cout << "   Функция get_order_status_history создана" << std::endl;

        // 2. Дополнительные функции
        std::cout << "\n2. Создание дополнительных функций..." << std::endl;

        // get_order_status
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION get_order_status(p_order_id INTEGER)
            RETURNS VARCHAR
            LANGUAGE plpgsql
            AS $$
            DECLARE
                v_status VARCHAR;
            BEGIN
                SELECT status INTO v_status
                FROM orders WHERE order_id = p_order_id;

                IF NOT FOUND THEN
                    RETURN 'не найден';
                END IF;

                RETURN v_status;
            END;
            $$;
        )");
        std::cout << "   Функция get_order_status создана" << std::endl;

        // get_user_order_count
        db.executeNonQuery("DROP FUNCTION IF EXISTS get_user_order_count();");
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION get_user_order_count()
            RETURNS TABLE(
                user_id INTEGER,
                user_name VARCHAR,
                user_email VARCHAR,
                total_orders INTEGER,
                pending_orders INTEGER,
                completed_orders INTEGER,
                canceled_orders INTEGER,
                returned_orders INTEGER
            )
            LANGUAGE plpgsql
            AS $$
            BEGIN
                RETURN QUERY
                SELECT
                    u.user_id,
                    u.name as user_name,
                    u.email as user_email,
                    COUNT(o.order_id)::INTEGER as total_orders,
                    COUNT(o.order_id) FILTER (WHERE o.status = 'pending')::INTEGER as pending_orders,
                    COUNT(o.order_id) FILTER (WHERE o.status = 'completed')::INTEGER as completed_orders,
                    COUNT(o.order_id) FILTER (WHERE o.status = 'canceled')::INTEGER as canceled_orders,
                    COUNT(o.order_id) FILTER (WHERE o.status = 'returned')::INTEGER as returned_orders
                FROM users u
                LEFT JOIN orders o ON u.user_id = o.user_id
                GROUP BY u.user_id, u.name, u.email
                ORDER BY total_orders DESC, u.name;
            END;
            $$;
        )");
        std::cout << "   Функция get_user_order_count создана" << std::endl;

        // get_total_spent_by_user
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION get_total_spent_by_user(p_user_id INTEGER DEFAULT NULL)
            RETURNS TABLE(
                user_id INTEGER,
                user_name VARCHAR,
                user_email VARCHAR,
                total_spent DECIMAL(10,2),
                average_order_value DECIMAL(10,2),
                first_order_date TIMESTAMP,
                last_order_date TIMESTAMP,
                total_orders INTEGER
            )
            LANGUAGE plpgsql
            AS $$
            BEGIN
                RETURN QUERY
                SELECT
                    u.user_id,
                    u.name as user_name,
                    u.email as user_email,
                    COALESCE(SUM(o.total_price), 0) as total_spent,
                    COALESCE(AVG(o.total_price), 0) as average_order_value,
                    MIN(o.order_date) as first_order_date,
                    MAX(o.order_date) as last_order_date,
                    COUNT(o.order_id)::INTEGER as total_orders
                FROM users u
                LEFT JOIN orders o ON u.user_id = o.user_id
                    AND o.status IN ('completed')
                WHERE (p_user_id IS NULL OR u.user_id = p_user_id)
                GROUP BY u.user_id, u.name, u.email
                ORDER BY total_spent DESC, u.name;
            END;
            $$;
        )");
        std::cout << "   Функция get_total_spent_by_user создана" << std::endl;

        // can_return_order
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION can_return_order(p_order_id INTEGER)
            RETURNS TABLE(
                can_return BOOLEAN,
                reason VARCHAR,
                order_status VARCHAR,
                order_date TIMESTAMP,
                days_passed INTEGER
            )
            LANGUAGE plpgsql
            AS $$
            DECLARE
                v_order_status VARCHAR;
                v_order_date TIMESTAMP;
                v_days_passed INTEGER;
            BEGIN
                SELECT status, order_date INTO v_order_status, v_order_date
                FROM orders WHERE order_id = p_order_id;

                IF NOT FOUND THEN
                    RETURN QUERY SELECT FALSE, 'Заказ не найден', NULL, NULL, NULL;
                    RETURN;
                END IF;

                v_days_passed := EXTRACT(DAY FROM (CURRENT_TIMESTAMP - v_order_date))::INTEGER;

                IF v_order_status != 'completed' THEN
                    RETURN QUERY SELECT FALSE,
                        'Возврат возможен только для завершенных заказов',
                        v_order_status, v_order_date, v_days_passed;
                    RETURN;
                END IF;

                IF v_days_passed > 30 THEN
                    RETURN QUERY SELECT FALSE,
                        'Прошло более 30 дней с даты заказа',
                        v_order_status, v_order_date, v_days_passed;
                    RETURN;
                END IF;

                RETURN QUERY SELECT TRUE,
                    'Возврат возможен',
                    v_order_status, v_order_date, v_days_passed;
            END;
            $$;
        )");
        std::cout << "   Функция can_return_order создана" << std::endl;

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при создании функций: " << e.what() << std::endl;
        return false;
    }
}