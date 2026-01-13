// stored_procedures.cpp
#include "stored_procedure.hpp"
#include <iostream>

bool createStoredProcedures(DatabaseConnection& db) {
    try {
        std::cout << "\nСоздание хранимых процедур..." << std::endl;

        // Хранимая функция create_order
        std::cout << "1. Создание функции create_order..." << std::endl;
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION create_order(
                p_user_id INTEGER,
                p_items JSONB,
                p_payment_method VARCHAR DEFAULT 'credit_card'
            ) RETURNS INTEGER
            LANGUAGE plpgsql
            AS $$
            DECLARE
                v_order_id INTEGER;
                v_total_price DECIMAL(10,2) := 0;
                v_item JSONB;
                v_product_id INTEGER;
                v_quantity INTEGER;
                v_price DECIMAL(10,2);
                v_stock INTEGER;
            BEGIN
                -- Проверяем существование пользователя
                IF NOT EXISTS (SELECT 1 FROM users WHERE user_id = p_user_id) THEN
                    RAISE EXCEPTION 'Пользователь с ID % не найден', p_user_id;
                END IF;

                -- Проверяем наличие товаров и считаем общую сумму
                FOR v_item IN SELECT * FROM jsonb_array_elements(p_items)
                LOOP
                    v_product_id := (v_item->>'product_id')::INTEGER;
                    v_quantity := (v_item->>'quantity')::INTEGER;

                    -- Получаем цену и остаток товара
                    SELECT price, stock_quantity INTO v_price, v_stock
                    FROM products WHERE product_id = v_product_id;

                    IF v_price IS NULL THEN
                        RAISE EXCEPTION 'Товар с ID % не найден', v_product_id;
                    END IF;

                    IF v_stock < v_quantity THEN
                        RAISE EXCEPTION 'Недостаточно товара ID % на складе (доступно: %, запрошено: %)',
                            v_product_id, v_stock, v_quantity;
                    END IF;

                    v_total_price := v_total_price + (v_price * v_quantity);
                END LOOP;

                -- Создаем заказ
                INSERT INTO orders (user_id, status, total_price, order_date)
                VALUES (p_user_id, 'pending', v_total_price, CURRENT_TIMESTAMP)
                RETURNING order_id INTO v_order_id;

                -- Добавляем позиции заказа и уменьшаем остатки
                FOR v_item IN SELECT * FROM jsonb_array_elements(p_items)
                LOOP
                    v_product_id := (v_item->>'product_id')::INTEGER;
                    v_quantity := (v_item->>'quantity')::INTEGER;

                    SELECT price INTO v_price FROM products WHERE product_id = v_product_id;

                    -- Добавляем позицию заказа
                    INSERT INTO order_items (order_id, product_id, quantity, price)
                    VALUES (v_order_id, v_product_id, v_quantity, v_price);

                    -- Уменьшаем остаток на складе
                    UPDATE products
                    SET stock_quantity = stock_quantity - v_quantity
                    WHERE product_id = v_product_id;
                END LOOP;

                -- Записываем в историю статусов
                INSERT INTO order_status_history (order_id, old_status, new_status, changed_by, reason)
                VALUES (v_order_id, NULL, 'pending', p_user_id, 'Заказ создан');

                RETURN v_order_id;
            END;
            $$;
        )");
        std::cout << "   Функция create_order создана" << std::endl;

        // Процедура update_order_status
        std::cout << "\n2. Создание процедуры update_order_status..." << std::endl;
        db.executeNonQuery(R"(
            CREATE OR REPLACE PROCEDURE update_order_status(
                p_order_id INTEGER,
                p_new_status VARCHAR,
                p_changed_by INTEGER DEFAULT NULL
            )
            LANGUAGE plpgsql
            AS $$
            DECLARE
                v_old_status VARCHAR;
                v_user_id INTEGER;
            BEGIN
                -- Получаем текущий статус заказа
                SELECT status, user_id INTO v_old_status, v_user_id
                FROM orders WHERE order_id = p_order_id;

                IF v_old_status IS NULL THEN
                    RAISE EXCEPTION 'Заказ с ID % не найден', p_order_id;
                END IF;

                -- Проверяем допустимость перехода статуса
                IF v_old_status = 'canceled' OR v_old_status = 'returned' THEN
                    RAISE EXCEPTION 'Невозможно изменить статус заказа в состоянии %', v_old_status;
                END IF;

                IF v_old_status = 'pending' AND p_new_status NOT IN ('completed', 'canceled') THEN
                    RAISE EXCEPTION 'Из статуса pending можно перейти только в completed или canceled';
                END IF;

                IF v_old_status = 'completed' AND p_new_status != 'returned' THEN
                    RAISE EXCEPTION 'Из статуса completed можно перейти только в returned';
                END IF;

                -- Обновляем статус
                UPDATE orders SET status = p_new_status WHERE order_id = p_order_id;

                -- Записываем в историю
                INSERT INTO order_status_history (order_id, old_status, new_status, changed_by, reason)
                VALUES (p_order_id, v_old_status, p_new_status, COALESCE(p_changed_by, v_user_id),
                        'Статус изменен с ' || v_old_status || ' на ' || p_new_status);

                -- Если заказ отменен, возвращаем товары на склад
                IF p_new_status = 'canceled' THEN
                    UPDATE products p
                    SET stock_quantity = p.stock_quantity + oi.quantity
                    FROM order_items oi
                    WHERE oi.order_id = p_order_id AND p.product_id = oi.product_id;
                END IF;
            END;
            $$;
        )");
        std::cout << "   Процедура update_order_status создана" << std::endl;

        // Процедура process_order_return
        std::cout << "\n3. Создание процедуры process_order_return..." << std::endl;
        db.executeNonQuery(R"(
            CREATE OR REPLACE PROCEDURE process_order_return(
                p_order_id INTEGER,
                p_reason TEXT DEFAULT NULL,
                p_processed_by INTEGER DEFAULT NULL
            )
            LANGUAGE plpgsql
            AS $$
            DECLARE
                v_status VARCHAR;
                v_order_date TIMESTAMP;
                v_days_passed INTEGER;
            BEGIN
                -- Получаем информацию о заказе
                SELECT status, order_date INTO v_status, v_order_date
                FROM orders WHERE order_id = p_order_id;

                IF v_status IS NULL THEN
                    RAISE EXCEPTION 'Заказ с ID % не найден', p_order_id;
                END IF;

                -- Проверяем, что заказ завершен
                IF v_status != 'completed' THEN
                    RAISE EXCEPTION 'Возврат возможен только для завершенных заказов (текущий статус: %)', v_status;
                END IF;

                -- Проверяем, не прошло ли более 30 дней
                v_days_passed := EXTRACT(DAY FROM (CURRENT_TIMESTAMP - v_order_date));
                IF v_days_passed > 30 THEN
                    RAISE EXCEPTION 'Возврат невозможен: прошло более 30 дней с момента заказа (% дней)', v_days_passed;
                END IF;

                -- Возвращаем товары на склад
                UPDATE products p
                SET stock_quantity = p.stock_quantity + oi.quantity
                FROM order_items oi
                WHERE oi.order_id = p_order_id AND p.product_id = oi.product_id;

                -- Обновляем статус заказа
                UPDATE orders SET status = 'returned' WHERE order_id = p_order_id;

                -- Записываем в историю
                INSERT INTO order_status_history (order_id, old_status, new_status, changed_by, reason)
                VALUES (p_order_id, 'completed', 'returned', p_processed_by,
                        COALESCE(p_reason, 'Возврат заказа'));
            END;
            $$;
        )");
        std::cout << "   Процедура process_order_return создана" << std::endl;

        // Функция get_order_status
        std::cout << "\n4. Создание функции get_order_status..." << std::endl;
        db.executeNonQuery(R"(
            CREATE OR REPLACE FUNCTION get_order_status(p_order_id INTEGER)
            RETURNS VARCHAR
            LANGUAGE plpgsql
            AS $$
            DECLARE
                v_status VARCHAR;
            BEGIN
                SELECT status INTO v_status FROM orders WHERE order_id = p_order_id;

                IF v_status IS NULL THEN
                    RETURN 'not_found';
                END IF;

                RETURN v_status;
            END;
            $$;
        )");
        std::cout << "   Функция get_order_status создана" << std::endl;


        std::cout << "\nВсе хранимые процедуры успешно созданы!" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при создании хранимых процедур: " << e.what() << std::endl;
        return false;
    }
}
