// reports_system.cpp
#include "DatabaseConnection.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <ctime>

// ================================
// 1. Класс для генерации отчетов
// ================================

class ReportManager {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    ReportManager(std::shared_ptr<DatabaseConnection> database) : db(database) {}

    // Проверка прав администратора
    bool isAdmin(int userId) {
        try {
            auto result = db->executeQuery(
                "SELECT role FROM users WHERE user_id = $1", userId);
            return !result.empty() && result[0][0] == "admin";
        } catch (...) {
            return false;
        }
    }

    // ================================
    // 2. Генерация CSV отчета "История изменений заказов и действий пользователей"
    // ================================

    bool generateOrderHistoryCSV(int userId, const std::string& outputPath = "order_history_report.csv") {
        // Проверка прав доступа
        if (!isAdmin(userId)) {
            std::cerr << "Ошибка: только администратор может генерировать отчеты" << std::endl;
            return false;
        }

        try {
            // Создаем хранимую функцию для генерации отчета, если она не существует
            createReportFunction();

            std::cout << "Генерация отчета CSV..." << std::endl;
            std::cout << "Пользователь: #" << userId << " (администратор)" << std::endl;
            std::cout << "Выходной файл: " << outputPath << std::endl;

            // Выполняем запрос для получения данных отчета
            auto reportData = db->executeQuery(R"(
                SELECT * FROM get_order_history_audit_report()
                ORDER BY event_timestamp DESC, order_id
            )");

            if (reportData.empty()) {
                std::cerr << "Нет данных для отчета" << std::endl;
                return false;
            }

            // Создаем CSV файл
            std::ofstream csvFile(outputPath);
            if (!csvFile.is_open()) {
                std::cerr << "Ошибка создания файла: " << outputPath << std::endl;
                return false;
            }

            // Заголовок CSV
            csvFile << "order_id,customer_name,order_status,order_total,order_date,"
                    << "history_type,event_description,event_timestamp,performed_by,performed_by_name,"
                    << "old_value,new_value,entity_type,entity_id,operation\n";

            // Используем STL алгоритмы для обработки данных
            int totalRecords = 0;
            double totalOrderAmount = 0.0;

            // Лямбда-функция для обработки каждой строки отчета
            auto processReportRow = [&](const std::vector<std::string>& row) {
                // Записываем данные в CSV
                for (size_t i = 0; i < row.size(); ++i) {
                    // Экранируем запятые и кавычки в CSV
                    std::string escapedValue = escapeCSV(row[i]);
                    csvFile << escapedValue;
                    if (i < row.size() - 1) csvFile << ",";
                }
                csvFile << "\n";

                // Собираем статистику
                totalRecords++;
                if (!row[3].empty() && row[0] != "0") { // Сумма заказа
                    try {
                        totalOrderAmount += std::stod(row[3]);
                    } catch (...) {}
                }
            };

            // Обрабатываем все строки отчета
            std::for_each(reportData.begin(), reportData.end(), processReportRow);

            csvFile.close();

            // Генерируем сводку отчета
            generateReportSummary(reportData, totalRecords, totalOrderAmount, outputPath);

            std::cout << "Отчет успешно создан!" << std::endl;
            std::cout << "  Всего записей: " << totalRecords << std::endl;
            std::cout << "  Общая сумма заказов: $" << totalOrderAmount << std::endl;
            std::cout << "  Файл: " << outputPath << std::endl;

            return true;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка генерации отчета: " << e.what() << std::endl;
            return false;
        }
    }

    // ================================
    // 3. Дополнительные отчеты
    // ================================

    // Отчет по активности пользователей
    bool generateUserActivityReport(int userId, const std::string& outputPath = "user_activity_report.csv") {
        if (!isAdmin(userId)) {
            std::cerr << "Ошибка: только администратор может генерировать отчеты" << std::endl;
            return false;
        }

        try {
            auto activityData = db->executeQuery(R"(
                SELECT
                    u.user_id,
                    u.name,
                    u.email,
                    u.role,
                    COUNT(DISTINCT o.order_id) as total_orders,
                    COALESCE(SUM(o.total_price), 0) as total_spent,
                    COUNT(DISTINCT al.log_id) as total_actions,
                    MAX(o.order_date) as last_order_date,
                    MAX(al.performed_at) as last_action_date
                FROM users u
                LEFT JOIN orders o ON u.user_id = o.user_id AND o.status = 'completed'
                LEFT JOIN audit_log al ON u.user_id = al.performed_by
                GROUP BY u.user_id, u.name, u.email, u.role
                ORDER BY total_actions DESC, total_spent DESC
            )");

            if (activityData.empty()) {
                std::cerr << "Нет данных для отчета активности" << std::endl;
                return false;
            }

            std::ofstream csvFile(outputPath);
            if (!csvFile.is_open()) {
                std::cerr << "Ошибка создания файла: " << outputPath << std::endl;
                return false;
            }

            // Заголовок
            csvFile << "user_id,name,email,role,total_orders,total_spent,total_actions,"
                    << "last_order_date,last_action_date\n";

            // Используем лямбду для обработки данных
            std::for_each(activityData.begin(), activityData.end(),
                [&csvFile](const auto& row) {
                    for (size_t i = 0; i < row.size(); ++i) {
                        csvFile << escapeCSV(row[i]);
                        if (i < row.size() - 1) csvFile << ",";
                    }
                    csvFile << "\n";
                }
            );

            csvFile.close();

            // Анализ данных с использованием STL алгоритмов
            analyzeUserActivity(activityData);

            std::cout << "Отчет активности пользователей создан: " << outputPath << std::endl;
            return true;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка генерации отчета активности: " << e.what() << std::endl;
            return false;
        }
    }

    // Отчет по возвратам
    bool generateReturnsReport(int userId, const std::string& outputPath = "returns_report.csv") {
        if (!isAdmin(userId)) {
            std::cerr << "Ошибка: только администратор может генерировать отчеты" << std::endl;
            return false;
        }

        try {
            auto returnsData = db->executeQuery(R"(
                SELECT
                    r.order_id,
                    o.user_id,
                    u.name as customer_name,
                    p.name as product_name,
                    r.quantity,
                    r.refund_amount,
                    r.return_date,
                    r.reason,
                    r.processed_by,
                    pu.name as processed_by_name,
                    o.order_date,
                    (r.return_date::date - o.order_date::date) as days_to_return
                FROM return_log r
                JOIN orders o ON r.order_id = o.order_id
                JOIN users u ON o.user_id = u.user_id
                JOIN products p ON r.product_id = p.product_id
                LEFT JOIN users pu ON r.processed_by = pu.user_id
                ORDER BY r.return_date DESC
            )");

            if (returnsData.empty()) {
                // Создаем пустой отчет, если нет данных
                std::ofstream csvFile(outputPath);
                csvFile << "order_id,customer_name,product_name,quantity,refund_amount,"
                        << "return_date,reason,processed_by_name,order_date,days_to_return\n";
                csvFile.close();
                std::cout << "Создан пустой отчет возвратов (нет данных)" << std::endl;
                return true;
            }

            std::ofstream csvFile(outputPath);
            csvFile << "order_id,customer_name,product_name,quantity,refund_amount,"
                    << "return_date,reason,processed_by_name,order_date,days_to_return\n";

            // Используем accumulate для подсчета статистики
            auto stats = std::accumulate(returnsData.begin(), returnsData.end(),
                std::map<std::string, double>(),
                [](std::map<std::string, double> acc, const std::vector<std::string>& row) {
                    acc["total_returns"]++;
                    acc["total_refund"] += std::stod(row[5]);
                    acc["total_quantity"] += std::stod(row[4]);
                    return acc;
                });

            std::for_each(returnsData.begin(), returnsData.end(),
                [&csvFile](const auto& row) {
                    for (size_t i = 0; i < row.size(); ++i) {
                        csvFile << escapeCSV(row[i]);
                        if (i < row.size() - 1) csvFile << ",";
                    }
                    csvFile << "\n";
                }
            );

            csvFile.close();

            std::cout << "Отчет возвратов создан: " << outputPath << std::endl;
            std::cout << "  Всего возвратов: " << stats["total_returns"] << std::endl;
            std::cout << "  Общая сумма возвратов: $" << stats["total_refund"] << std::endl;
            std::cout << "  Общее количество товаров: " << stats["total_quantity"] << std::endl;

            return true;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка генерации отчета возвратов: " << e.what() << std::endl;
            return false;
        }
    }

    // ================================
    // 4. Консольные отчеты (без CSV)
    // ================================

    void displaySalesReport(int userId) {
        if (!isAdmin(userId)) {
            std::cerr << "Ошибка: только администратор может просматривать отчеты" << std::endl;
            return;
        }

        try {
            auto salesData = db->executeQuery(R"(
                SELECT
                    DATE(o.order_date) as sale_date,
                    COUNT(DISTINCT o.order_id) as orders_count,
                    COUNT(DISTINCT o.user_id) as unique_customers,
                    SUM(o.total_price) as total_revenue,
                    AVG(o.total_price) as avg_order_value,
                    COUNT(DISTINCT oi.product_id) as unique_products_sold,
                    SUM(oi.quantity) as total_items_sold
                FROM orders o
                LEFT JOIN order_items oi ON o.order_id = oi.order_id
                WHERE o.status = 'completed'
                    AND o.order_date >= CURRENT_DATE - INTERVAL '30 days'
                GROUP BY DATE(o.order_date)
                ORDER BY sale_date DESC
            )");

            if (!salesData.empty()) {
                std::cout << "\nОтчет по продажам за последние 30 дней:" << std::endl;
                std::cout << "==========================================" << std::endl;

                // Используем STL алгоритмы для анализа
                auto totalRevenue = std::accumulate(salesData.begin(), salesData.end(), 0.0,
                    [](double sum, const auto& row) {
                        return sum + std::stod(row[3]);
                    });

                auto maxRevenueDay = std::max_element(salesData.begin(), salesData.end(),
                    [](const auto& a, const auto& b) {
                        return std::stod(a[3]) < std::stod(b[3]);
                    });

                std::cout << "Общая выручка: $" << totalRevenue << std::endl;
                if (maxRevenueDay != salesData.end()) {
                    std::cout << "Лучший день: " << (*maxRevenueDay)[0]
                              << " ($" << (*maxRevenueDay)[3] << ")" << std::endl;
                }

                std::cout << "\nДетали по дням:" << std::endl;
                std::for_each(salesData.begin(), salesData.end(),
                    [](const auto& row) {
                        std::cout << "  " << row[0] << ": $" << row[3]
                                  << " (" << row[1] << " заказов, "
                                  << row[6] << " товаров)" << std::endl;
                    }
                );
            }

        } catch (const std::exception& e) {
            std::cerr << "Ошибка генерации отчета продаж: " << e.what() << std::endl;
        }
    }

    // ================================
    // 5. Демонстрационная функция
    // ================================

    void demonstrateReportsSystem(int adminUserId) {
        if (!isAdmin(adminUserId)) {
            std::cerr << "Демонстрация отчетов доступна только администратору" << std::endl;
            return;
        }

        try {
            std::cout << "\n\n=== ДЕМОНСТРАЦИЯ СИСТЕМЫ ОТЧЕТОВ ===" << std::endl;
            std::cout << "Администратор: #" << adminUserId << std::endl;

            // 1. Основной отчет по истории изменений
            std::cout << "\n1. Генерация основного отчета (CSV)..." << std::endl;
            generateOrderHistoryCSV(adminUserId, "demo_order_history.csv");

            // 2. Отчет по активности пользователей
            std::cout << "\n2. Генерация отчета активности пользователей..." << std::endl;
            generateUserActivityReport(adminUserId, "demo_user_activity.csv");

            // 3. Отчет по возвратам
            std::cout << "\n3. Генерация отчета по возвратам..." << std::endl;
            generateReturnsReport(adminUserId, "demo_returns.csv");

            // 4. Консольные отчеты
            std::cout << "\n4. Консольные отчеты..." << std::endl;
            displaySalesReport(adminUserId);

            // 5. Создание тестовых данных для отчетов
            std::cout << "\n5. Создание тестовых данных для отчетов..." << std::endl;
            createTestReportData();

            // 6. Генерация отчетов с тестовыми данными
            std::cout << "\n6. Генерация отчетов с тестовыми данными..." << std::endl;
            generateOrderHistoryCSV(adminUserId, "test_order_history.csv");

            std::cout << "\n=== ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА ===" << std::endl;
            std::cout << "Созданные файлы отчетов:" << std::endl;
            std::cout << "  - demo_order_history.csv" << std::endl;
            std::cout << "  - demo_user_activity.csv" << std::endl;
            std::cout << "  - demo_returns.csv" << std::endl;
            std::cout << "  - test_order_history.csv" << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка демонстрации системы отчетов: " << e.what() << std::endl;
        }
    }

private:
    // Вспомогательные методы

    void createReportFunction() {
        try {
            db->executeNonQuery(R"(
                CREATE OR REPLACE FUNCTION get_order_history_audit_report()
                RETURNS TABLE(
                    order_id INTEGER,
                    customer_name VARCHAR,
                    order_status VARCHAR,
                    order_total DECIMAL(10,2),
                    order_date TIMESTAMP,
                    history_type TEXT,
                    event_description TEXT,
                    event_timestamp TIMESTAMP,
                    performed_by INTEGER,
                    performed_by_name VARCHAR,
                    old_value TEXT,
                    new_value TEXT,
                    entity_type VARCHAR,
                    entity_id INTEGER,
                    operation VARCHAR
                )
                LANGUAGE plpgsql
                AS $$
                BEGIN
                    -- История статусов заказов
                    RETURN QUERY
                    SELECT
                        o.order_id,
                        u.name as customer_name,
                        o.status as order_status,
                        o.total_price as order_total,
                        o.order_date,
                        'status_change' as history_type,
                        'Изменение статуса заказа' as event_description,
                        h.changed_at as event_timestamp,
                        h.changed_by as performed_by,
                        COALESCE(changer.name, 'Система') as performed_by_name,
                        h.old_status as old_value,
                        h.new_status as new_value,
                        'order' as entity_type,
                        o.order_id as entity_id,
                        'update' as operation
                    FROM orders o
                    JOIN users u ON o.user_id = u.user_id
                    LEFT JOIN order_status_history h ON o.order_id = h.order_id
                    LEFT JOIN users changer ON h.changed_by = changer.user_id
                    WHERE h.history_id IS NOT NULL

                    UNION ALL

                    -- Аудит-лог заказов
                    SELECT
                        al.entity_id as order_id,
                        u.name as customer_name,
                        o.status as order_status,
                        o.total_price as order_total,
                        o.order_date,
                        'audit_log' as history_type,
                        'Действие с заказом' as event_description,
                        al.performed_at as event_timestamp,
                        al.performed_by,
                        performer.name as performed_by_name,
                        NULL as old_value,
                        NULL as new_value,
                        al.entity_type,
                        al.entity_id,
                        al.operation
                    FROM audit_log al
                    JOIN orders o ON al.entity_id = o.order_id AND al.entity_type = 'order'
                    JOIN users u ON o.user_id = u.user_id
                    LEFT JOIN users performer ON al.performed_by = performer.user_id

                    UNION ALL

                    -- Создание заказов (первоначальный статус)
                    SELECT
                        o.order_id,
                        u.name as customer_name,
                        o.status as order_status,
                        o.total_price as order_total,
                        o.order_date,
                        'order_creation' as history_type,
                        'Создание заказа' as event_description,
                        o.order_date as event_timestamp,
                        o.user_id as performed_by,
                        u.name as performed_by_name,
                        NULL as old_value,
                        'pending' as new_value,
                        'order' as entity_type,
                        o.order_id as entity_id,
                        'insert' as operation
                    FROM orders o
                    JOIN users u ON o.user_id = u.user_id

                    ORDER BY event_timestamp DESC;
                END;
                $$;
            )");

            std::cout << "Хранимая функция для отчета создана/обновлена" << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка создания функции отчета: " << e.what() << std::endl;
            throw;
        }
    }

    static std::string escapeCSV(const std::string& input) {
        if (input.empty()) {
            return "\"\"";
        }

        bool needsQuotes = false;
        std::string result = input;

        // Проверяем, нужны ли кавычки
        if (result.find(',') != std::string::npos ||
            result.find('"') != std::string::npos ||
            result.find('\n') != std::string::npos ||
            result.find('\r') != std::string::npos) {
            needsQuotes = true;
        }

        // Экранируем кавычки
        size_t pos = 0;
        while ((pos = result.find('"', pos)) != std::string::npos) {
            result.replace(pos, 1, "\"\"");
            pos += 2;
        }

        if (needsQuotes) {
            result = "\"" + result + "\"";
        }

        return result;
    }

    void generateReportSummary(const std::vector<std::vector<std::string>>& reportData,
                              int totalRecords, double totalOrderAmount,
                              const std::string& outputPath) {
        std::string summaryPath = outputPath + ".summary.txt";
        std::ofstream summaryFile(summaryPath);

        if (summaryFile.is_open()) {
            summaryFile << "ОТЧЕТ: История изменений заказов и действий пользователей\n";
            summaryFile << "=======================================================\n\n";

            summaryFile << "Сводка:\n";
            summaryFile << "  Дата генерации: " << getCurrentDateTime() << "\n";
            summaryFile << "  Всего записей: " << totalRecords << "\n";
            summaryFile << "  Общая сумма заказов: $" << totalOrderAmount << "\n\n";

            // Анализ типов событий с использованием STL алгоритмов
            std::map<std::string, int> eventsByType;
            std::for_each(reportData.begin(), reportData.end(),
                [&eventsByType](const auto& row) {
                    if (row.size() > 5) {
                        eventsByType[row[5]]++;
                    }
                });

            summaryFile << "Распределение по типам событий:\n";
            std::for_each(eventsByType.begin(), eventsByType.end(),
                [&summaryFile](const auto& eventType) {
                    summaryFile << "  " << eventType.first << ": "
                               << eventType.second << " записей\n";
                });

            summaryFile << "\nСтатистика по операциям:\n";
            std::map<std::string, int> operationsCount;
            std::for_each(reportData.begin(), reportData.end(),
                [&operationsCount](const auto& row) {
                    if (row.size() > 14) {
                        operationsCount[row[14]]++;
                    }
                });

            std::for_each(operationsCount.begin(), operationsCount.end(),
                [&summaryFile](const auto& operation) {
                    summaryFile << "  " << operation.first << ": "
                               << operation.second << "\n";
                });

            summaryFile.close();
            std::cout << "Сводка отчета создана: " << summaryPath << std::endl;
        }
    }

    void analyzeUserActivity(const std::vector<std::vector<std::string>>& activityData) {
        if (activityData.empty()) return;

        // Используем STL алгоритмы для анализа
        auto mostActiveUser = std::max_element(activityData.begin(), activityData.end(),
            [](const auto& a, const auto& b) {
                int actionsA = std::stoi(a[6]);
                int actionsB = std::stoi(b[6]);
                return actionsA < actionsB;
            });

        auto topSpender = std::max_element(activityData.begin(), activityData.end(),
            [](const auto& a, const auto& b) {
                double spentA = std::stod(a[5]);
                double spentB = std::stod(b[5]);
                return spentA < spentB;
            });

        std::cout << "\nАнализ активности пользователей:" << std::endl;
        if (mostActiveUser != activityData.end()) {
            std::cout << "  Самый активный: " << (*mostActiveUser)[1]
                      << " (" << (*mostActiveUser)[6] << " действий)" << std::endl;
        }
        if (topSpender != activityData.end()) {
            std::cout << "  Самый крупный покупатель: " << (*topSpender)[1]
                      << " ($" << (*topSpender)[5] << ")" << std::endl;
        }
    }

    void createTestReportData() {
        try {
            // Создаем тестового пользователя для отчетов
            db->executeNonQuery(R"(
                INSERT INTO users (name, email, role, password_hash)
                VALUES ('Тест Отчетов', 'report_test@example.com', 'customer', 'hash123')
                ON CONFLICT (email) DO NOTHING
            )");

            auto testUser = db->executeQuery(
                "SELECT user_id FROM users WHERE email = 'report_test@example.com'");

            if (!testUser.empty()) {
                int testUserId = std::stoi(testUser[0][0]);

                // Создаем тестовые заказы
                for (int i = 0; i < 3; ++i) {
                    db->executeQuery(
                        "INSERT INTO orders (user_id, status, total_price) "
                        "VALUES ($1, 'completed', $2)",
                        testUserId, 50.0 * (i + 1));
                }

                // Создаем тестовые записи аудита
                db->executeQuery(
                    "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
                    "VALUES ('order', (SELECT order_id FROM orders WHERE user_id = $1 LIMIT 1), 'update', $2)",
                    testUserId, testUserId);

                std::cout << "Тестовые данные для отчетов созданы" << std::endl;
            }

        } catch (const std::exception& e) {
            std::cerr << "Ошибка создания тестовых данных: " << e.what() << std::endl;
        }
    }

    std::string getCurrentDateTime() {
        std::time_t now = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&now);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return std::string(buffer);
    }
};

// ================================
// 6. Интеграция с главной программой
// ================================

void setupReportSystem(DatabaseConnection& db) {
    try {
        std::cout << "\nНастройка системы отчетов..." << std::endl;

        // Создаем необходимые таблицы, если их нет
        db.executeNonQuery(R"(
            CREATE TABLE IF NOT EXISTS report_log (
                report_id SERIAL PRIMARY KEY,
                report_type VARCHAR(50) NOT NULL,
                generated_by INTEGER NOT NULL,
                generation_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                file_path VARCHAR(255),
                record_count INTEGER DEFAULT 0,
                CONSTRAINT fk_report_user
                    FOREIGN KEY (generated_by)
                    REFERENCES users(user_id)
                    ON DELETE CASCADE
            )
        )");

        // Создаем индекс для быстрого поиска отчетов
        db.executeNonQuery(
            "CREATE INDEX IF NOT EXISTS idx_report_log_date ON report_log(generation_date DESC)");

        std::cout << "Система отчетов настроена" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка настройки системы отчетов: " << e.what() << std::endl;
    }
}