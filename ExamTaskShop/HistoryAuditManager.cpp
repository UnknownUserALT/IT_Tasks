// history_audit_system.cpp
#include "DatabaseConnection.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <map>

// ================================
// 1. Класс для управления доступом к истории и аудиту
// ================================

class HistoryAuditManager {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    HistoryAuditManager(std::shared_ptr<DatabaseConnection> database) : db(database) {}

    // Лямбда-функция для проверки прав доступа
    using PermissionChecker = std::function<bool(int userId, const std::string& userRole,
                                                 const std::string& resourceType, int resourceId)>;

    // Базовая проверка прав для разных ролей
    PermissionChecker getPermissionChecker() {
        return [this](int userId, const std::string& userRole,
                      const std::string& resourceType, int resourceId) -> bool {

            // Администратор имеет доступ ко всему
            if (userRole == "admin") {
                return true;
            }

            // Менеджер имеет ограниченный доступ
            else if (userRole == "manager") {
                // Менеджер может видеть только заказы и связанные с ними ресурсы
                if (resourceType == "order" || resourceType == "order_history" ||
                    resourceType == "order_audit") {
                    return true;
                }
                return false;
            }

            // Покупатель может видеть только свои ресурсы
            else if (userRole == "customer") {
                if (resourceType == "order") {
                    // Проверяем, принадлежит ли заказ пользователю
                    auto result = db->executeQuery(
                        "SELECT EXISTS(SELECT 1 FROM orders WHERE order_id = $1 AND user_id = $2)",
                        resourceId, userId);
                    return !result.empty() && result[0][0] == "t";
                }
                else if (resourceType == "order_history") {
                    // Проверяем, принадлежит ли история заказа пользователю
                    auto result = db->executeQuery(
                        "SELECT EXISTS(SELECT 1 FROM orders o "
                        "JOIN order_status_history h ON o.order_id = h.order_id "
                        "WHERE h.history_id = $1 AND o.user_id = $2)",
                        resourceId, userId);
                    return !result.empty() && result[0][0] == "t";
                }
                // Покупатель не имеет доступа к аудиту
                return false;
            }

            return false;
        };
    }

    // ================================
    // 2. Методы для работы с историей статусов заказов
    // ================================

    // Получение истории статусов с проверкой прав
    std::vector<std::vector<std::string>> getOrderStatusHistory(int userId, int orderId) {
        auto permissionCheck = getPermissionChecker();
        auto userInfo = getUserInfo(userId);

        if (userInfo.empty()) {
            std::cerr << "Пользователь не найден" << std::endl;
            return {};
        }

        std::string userRole = userInfo[0][2];

        // Проверяем права доступа
        if (!permissionCheck(userId, userRole, "order", orderId)) {
            std::cerr << "Доступ запрещен: пользователь " << userId
                      << " не имеет прав на просмотр заказа " << orderId << std::endl;
            return {};
        }

        try {
            // Используем STL алгоритмы для обработки результата
            auto history = db->executeQuery(
                "SELECT * FROM get_order_status_history($1)", orderId);

            // Фильтруем результат в зависимости от роли
            std::vector<std::vector<std::string>> filteredHistory;

            if (userRole == "customer") {
                // Покупатель видит только свою историю
                std::copy_if(history.begin(), history.end(),
                           std::back_inserter(filteredHistory),
                           [userId](const std::vector<std::string>& row) {
                               int changedBy = std::stoi(row[5]);
                               return changedBy == userId || row[5].empty();
                           });
            } else {
                // Админ и менеджер видят всю историю
                filteredHistory = history;
            }

            // Используем лямбду для форматирования вывода
            auto formatHistoryEntry = [](const std::vector<std::string>& entry) {
                return "ID: " + entry[0] + " | Заказ: " + entry[1] +
                       " | Статус: " + entry[2] + " → " + entry[3] +
                       " | Дата: " + entry[4] + " | Изменил: " + entry[6];
            };

            // Выводим историю с использованием STL алгоритмов
            if (!filteredHistory.empty()) {
                std::cout << "\nИстория статусов заказа #" << orderId << ":" << std::endl;
                std::cout << "==========================================" << std::endl;

                std::for_each(filteredHistory.begin(), filteredHistory.end(),
                    [&formatHistoryEntry](const auto& entry) {
                        std::cout << formatHistoryEntry(entry) << std::endl;
                    }
                );
            }

            return filteredHistory;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка получения истории: " << e.what() << std::endl;
            return {};
        }
    }

    // Получение истории заказов, утвержденных менеджером
    std::vector<std::vector<std::string>> getManagerApprovedOrders(int managerId) {
        try {
            // Проверяем, что пользователь - менеджер
            auto userInfo = getUserInfo(managerId);
            if (userInfo.empty() || userInfo[0][2] != "manager") {
                std::cerr << "Пользователь не является менеджером" << std::endl;
                return {};
            }

            // Используем STL алгоритмы для агрегации данных
            auto orders = db->executeQuery(R"(
                SELECT DISTINCT o.order_id, u.name as customer_name,
                       o.status, o.total_price, o.order_date,
                       h.changed_at as approval_date
                FROM orders o
                JOIN users u ON o.user_id = u.user_id
                JOIN order_status_history h ON o.order_id = h.order_id
                WHERE h.changed_by = $1
                AND h.new_status = 'completed'
                ORDER BY h.changed_at DESC
            )", managerId);

            // Используем лямбду для подсчета статистики
            auto stats = std::accumulate(orders.begin(), orders.end(),
                std::map<std::string, int>(),
                [](std::map<std::string, int> acc, const std::vector<std::string>& row) {
                    acc["total_orders"]++;
                    acc["total_amount"] += static_cast<int>(std::stod(row[3]));
                    return acc;
                });

            if (!orders.empty()) {
                std::cout << "\nЗаказы, утвержденные менеджером #" << managerId << ":" << std::endl;
                std::cout << "Всего заказов: " << stats["total_orders"]
                          << " | Общая сумма: $" << stats["total_amount"] << std::endl;
                std::cout << "==========================================" << std::endl;

                std::for_each(orders.begin(), orders.end(),
                    [](const auto& row) {
                        std::cout << "Заказ #" << row[0] << " | Клиент: " << row[1]
                                  << " | Статус: " << row[2] << " | Сумма: $" << row[3]
                                  << " | Дата утверждения: " << row[5] << std::endl;
                    }
                );
            }

            return orders;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка получения заказов менеджера: " << e.what() << std::endl;
            return {};
        }
    }

    // ================================
    // 3. Методы для работы с аудит-логом
    // ================================

    // Получение аудит-лога с проверкой прав
    std::vector<std::vector<std::string>> getAuditLog(int userId,
                                                     const std::string& entityType = "",
                                                     int entityId = -1,
                                                     const std::string& startDate = "",
                                                     const std::string& endDate = "") {
        auto userInfo = getUserInfo(userId);
        if (userInfo.empty()) {
            std::cerr << "Пользователь не найден" << std::endl;
            return {};
        }

        std::string userRole = userInfo[0][2];

        // Проверка прав доступа к аудиту
        if (userRole == "customer") {
            std::cerr << "Покупатели не имеют доступа к аудит-логу" << std::endl;
            return {};
        }

        try {
            std::vector<std::vector<std::string>> auditLog;

            if (userRole == "admin") {
                // Администратор видит весь аудит-лог
                if (entityType.empty()) {
                    auditLog = db->executeQuery(
                        "SELECT * FROM get_audit_log($1::TIMESTAMP, $2::TIMESTAMP, NULL, NULL, NULL)",
                        startDate.empty() ? "NULL" : startDate,
                        endDate.empty() ? "NULL" : endDate);
                } else {
                    auditLog = db->executeQuery(
                        "SELECT * FROM get_audit_log($1::TIMESTAMP, $2::TIMESTAMP, $3, NULL, NULL)",
                        startDate.empty() ? "NULL" : startDate,
                        endDate.empty() ? "NULL" : endDate,
                        entityType);
                }
            }
            else if (userRole == "manager") {
                // Менеджер видит только аудит, связанный с заказами
                if (entityType.empty() || entityType == "order") {
                    auditLog = db->executeQuery(
                        "SELECT * FROM get_audit_log($1::TIMESTAMP, $2::TIMESTAMP, 'order', NULL, NULL)",
                        startDate.empty() ? "NULL" : startDate,
                        endDate.empty() ? "NULL" : endDate);
                } else {
                    std::cerr << "Менеджер может просматривать только аудит заказов" << std::endl;
                    return {};
                }
            }

            // Используем STL алгоритмы для анализа аудит-лога
            if (!auditLog.empty()) {
                analyzeAuditLog(auditLog, userRole);
                displayAuditLog(auditLog, userRole);
            }

            return auditLog;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка получения аудит-лога: " << e.what() << std::endl;
            return {};
        }
    }

    // Получение аудит-лога по пользователю
    std::vector<std::vector<std::string>> getAuditLogByUser(int requesterId, int targetUserId) {
        auto requesterInfo = getUserInfo(requesterId);
        auto targetInfo = getUserInfo(targetUserId);

        if (requesterInfo.empty() || targetInfo.empty()) {
            std::cerr << "Пользователь не найден" << std::endl;
            return {};
        }

        std::string requesterRole = requesterInfo[0][2];
        std::string targetRole = targetInfo[0][2];

        // Проверка прав
        if (requesterRole != "admin") {
            std::cerr << "Только администратор может просматривать аудит-лог других пользователей" << std::endl;
            return {};
        }

        try {
            auto auditLog = db->executeQuery(
                "SELECT * FROM get_audit_log_by_user($1)", targetUserId);

            if (!auditLog.empty()) {
                std::cout << "\nАудит-лог пользователя #" << targetUserId
                          << " (" << targetInfo[0][1] << ", роль: " << targetRole << "):" << std::endl;
                std::cout << "==========================================" << std::endl;

                // Группируем действия по типам сущностей
                std::map<std::string, int> actionsByEntity;
                std::for_each(auditLog.begin(), auditLog.end(),
                    [&actionsByEntity](const auto& row) {
                        actionsByEntity[row[1]]++;
                    });

                std::cout << "Статистика действий:" << std::endl;
                std::for_each(actionsByEntity.begin(), actionsByEntity.end(),
                    [](const auto& item) {
                        std::cout << "  " << item.first << ": " << item.second << " действий" << std::endl;
                    });

                // Показываем последние 5 действий
                int limit = std::min(5, static_cast<int>(auditLog.size()));
                std::cout << "\nПоследние " << limit << " действий:" << std::endl;

                for (int i = 0; i < limit; ++i) {
                    const auto& row = auditLog[i];
                    std::cout << "  [" << row[6] << "] " << row[1] << " #" << row[2]
                              << " -> " << row[3] << " (" << row[7] << ")" << std::endl;
                }
            }

            return auditLog;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка получения аудит-лога пользователя: " << e.what() << std::endl;
            return {};
        }
    }

    // ================================
    // 4. Расширенная аналитика и отчеты
    // ================================

    // Получение статистики по изменениям статусов
    void getStatusChangeStatistics(int userId, const std::string& period = "30 days") {
        auto userInfo = getUserInfo(userId);
        if (userInfo.empty()) {
            return;
        }

        std::string userRole = userInfo[0][2];

        if (userRole != "admin" && userRole != "manager") {
            std::cerr << "Доступ запрещен: только администраторы и менеджеры могут просматривать статистику" << std::endl;
            return;
        }

        try {
            // Используем STL алгоритмы для анализа статистики
            auto statistics = db->executeQuery(R"(
                SELECT
                    DATE(changed_at) as change_date,
                    new_status,
                    COUNT(*) as change_count,
                    COUNT(DISTINCT changed_by) as unique_users
                FROM order_status_history
                WHERE changed_at >= CURRENT_DATE - INTERVAL '1 day' * $1
                GROUP BY DATE(changed_at), new_status
                ORDER BY change_date DESC, change_count DESC
            )", period == "30 days" ? "30" : "7");

            if (!statistics.empty()) {
                std::cout << "\nСтатистика изменений статусов за последние " << period << ":" << std::endl;
                std::cout << "==========================================" << std::endl;

                // Группируем по датам
                std::map<std::string, std::vector<std::vector<std::string>>> statsByDate;
                std::for_each(statistics.begin(), statistics.end(),
                    [&statsByDate](const auto& row) {
                        statsByDate[row[0]].push_back(row);
                    });

                // Выводим статистику
                std::for_each(statsByDate.begin(), statsByDate.end(),
                    [](const auto& dateStats) {
                        std::cout << "\nДата: " << dateStats.first << std::endl;
                        std::for_each(dateStats.second.begin(), dateStats.second.end(),
                            [](const auto& stat) {
                                std::cout << "  Статус: " << stat[1]
                                          << " | Изменений: " << stat[2]
                                          << " | Пользователей: " << stat[3] << std::endl;
                            }
                        );
                    }
                );
            }

        } catch (const std::exception& e) {
            std::cerr << "Ошибка получения статистики: " << e.what() << std::endl;
        }
    }

    // Получение популярных действий в аудит-логе
    void getPopularAuditActions(int userId) {
        auto userInfo = getUserInfo(userId);
        if (userInfo.empty() || userInfo[0][2] != "admin") {
            std::cerr << "Только администратор может просматривать статистику аудита" << std::endl;
            return;
        }

        try {
            auto popularActions = db->executeQuery(R"(
                SELECT
                    entity_type,
                    operation,
                    performed_by,
                    u.name as user_name,
                    COUNT(*) as action_count
                FROM audit_log al
                JOIN users u ON al.performed_by = u.user_id
                WHERE al.performed_at >= CURRENT_DATE - INTERVAL '30 days'
                GROUP BY entity_type, operation, performed_by, u.name
                ORDER BY action_count DESC
                LIMIT 10
            )");

            if (!popularActions.empty()) {
                std::cout << "\nТоп-10 самых активных действий за 30 дней:" << std::endl;
                std::cout << "==========================================" << std::endl;

                std::for_each(popularActions.begin(), popularActions.end(),
                    [](const auto& row) {
                        std::cout << "  " << row[4] << "x | " << row[0]
                                  << " -> " << row[1] << " | Пользователь: "
                                  << row[3] << " (#" << row[2] << ")" << std::endl;
                    }
                );
            }

        } catch (const std::exception& e) {
            std::cerr << "Ошибка получения статистики аудита: " << e.what() << std::endl;
        }
    }

private:
    // Вспомогательные методы

    std::vector<std::vector<std::string>> getUserInfo(int userId) {
        try {
            return db->executeQuery(
                "SELECT user_id, name, role FROM users WHERE user_id = $1", userId);
        } catch (...) {
            return {};
        }
    }

    void analyzeAuditLog(const std::vector<std::vector<std::string>>& auditLog,
                        const std::string& userRole) {
        // Используем STL алгоритмы для анализа
        auto totalActions = auditLog.size();

        auto suspiciousActions = std::count_if(auditLog.begin(), auditLog.end(),
            [](const auto& row) {
                return row[3] == "delete" || (row[1] == "users" && row[3] == "update");
            });

        std::cout << "\nАнализ аудит-лога:" << std::endl;
        std::cout << "  Всего действий: " << totalActions << std::endl;
        std::cout << "  Подозрительных действий: " << suspiciousActions << std::endl;

        if (userRole == "admin" && suspiciousActions > 0) {
            std::cout << "  ВНИМАНИЕ: Обнаружены подозрительные действия!" << std::endl;
        }
    }

    void displayAuditLog(const std::vector<std::vector<std::string>>& auditLog,
                        const std::string& userRole) {
        std::cout << "\nАудит-лог (" << (userRole == "admin" ? "полный доступ" : "только заказы") << "):" << std::endl;
        std::cout << "==========================================" << std::endl;

        // Ограничиваем вывод для менеджеров
        int limit = userRole == "admin" ? auditLog.size() : std::min(10, static_cast<int>(auditLog.size()));

        for (int i = 0; i < limit && i < auditLog.size(); ++i) {
            const auto& row = auditLog[i];
            std::cout << "  [" << row[6] << "] " << row[1] << " #" << row[2]
                      << " -> " << row[3] << " | Выполнил: " << row[5] << std::endl;
        }

        if (userRole == "manager" && auditLog.size() > 10) {
            std::cout << "  ... и еще " << (auditLog.size() - 10) << " записей" << std::endl;
        }
    }
};

// ================================
// 5. Демонстрационная функция
// ================================

void demonstrateHistoryAuditSystem(std::shared_ptr<DatabaseConnection> db) {
    try {
        std::cout << "\n\n=== ДЕМОНСТРАЦИЯ СИСТЕМЫ ИСТОРИИ И АУДИТА ===" << std::endl;

        HistoryAuditManager manager(db);

        // Создаем тестовых пользователей разных ролей
        std::cout << "\n1. Создание тестовых пользователей..." << std::endl;

        db->executeNonQuery(
            "INSERT INTO users (name, email, role, password_hash) "
            "VALUES ('Админ Тестовый', 'admin_test@example.com', 'admin', 'hash1') "
            "ON CONFLICT (email) DO NOTHING");

        db->executeNonQuery(
            "INSERT INTO users (name, email, role, password_hash) "
            "VALUES ('Менеджер Тестовый', 'manager_test@example.com', 'manager', 'hash2') "
            "ON CONFLICT (email) DO NOTHING");

        db->executeNonQuery(
            "INSERT INTO users (name, email, role, password_hash) "
            "VALUES ('Покупатель Тестовый', 'customer_test@example.com', 'customer', 'hash3') "
            "ON CONFLICT (email) DO NOTHING");

        // Получаем ID пользователей
        auto adminId = db->executeQuery(
            "SELECT user_id FROM users WHERE email = 'admin_test@example.com'");
        auto managerId = db->executeQuery(
            "SELECT user_id FROM users WHERE email = 'manager_test@example.com'");
        auto customerId = db->executeQuery(
            "SELECT user_id FROM users WHERE email = 'customer_test@example.com'");

        if (adminId.empty() || managerId.empty() || customerId.empty()) {
            std::cerr << "Не удалось получить ID тестовых пользователей" << std::endl;
            return;
        }

        int adminUserId = std::stoi(adminId[0][0]);
        int managerUserId = std::stoi(managerId[0][0]);
        int customerUserId = std::stoi(customerId[0][0]);

        // Создаем тестовые заказы
        std::cout << "\n2. Создание тестовых заказов..." << std::endl;

        db->executeQuery(
            "INSERT INTO orders (user_id, status, total_price) "
            "VALUES ($1, 'completed', 100.00) "
            "RETURNING order_id", customerUserId);

        db->executeQuery(
            "INSERT INTO orders (user_id, status, total_price) "
            "VALUES ($1, 'pending', 200.00) "
            "RETURNING order_id", customerUserId);

        // Получаем ID созданных заказов
        auto orders = db->executeQuery(
            "SELECT order_id FROM orders WHERE user_id = $1 ORDER BY order_id DESC LIMIT 2",
            customerUserId);

        if (orders.size() < 2) {
            std::cerr << "Не удалось создать тестовые заказы" << std::endl;
            return;
        }

        int testOrder1 = std::stoi(orders[0][0]);
        int testOrder2 = std::stoi(orders[1][0]);

        // 3. Демонстрация проверки прав доступа
        std::cout << "\n3. Демонстрация проверки прав доступа..." << std::endl;

        auto permissionCheck = manager.getPermissionChecker();

        // Тестируем доступ для разных ролей
        std::vector<std::tuple<int, std::string, std::string, int, std::string>> testCases = {
            {adminUserId, "admin", "order", testOrder1, "Админ к заказу"},
            {managerUserId, "manager", "order", testOrder1, "Менеджер к заказу"},
            {customerUserId, "customer", "order", testOrder1, "Покупатель к своему заказу"},
            {customerUserId, "customer", "order", testOrder2, "Покупатель к своему заказу"},
            {adminUserId, "admin", "audit", 1, "Админ к аудиту"},
            {managerUserId, "manager", "audit", 1, "Менеджер к аудиту"},
            {customerUserId, "customer", "audit", 1, "Покупатель к аудиту"}
        };

        std::cout << "Результаты проверки прав:" << std::endl;
        std::for_each(testCases.begin(), testCases.end(),
            [&permissionCheck](const auto& testCase) {
                auto [userId, role, resource, resourceId, description] = testCase;
                bool hasAccess = permissionCheck(userId, role, resource, resourceId);
                std::cout << "  " << description << ": "
                          << (hasAccess ? "РАЗРЕШЕНО" : "ЗАПРЕЩЕНО") << std::endl;
            }
        );

        // 4. Демонстрация работы с историей статусов
        std::cout << "\n4. Демонстрация работы с историей статусов..." << std::endl;

        // Админ просматривает историю
        std::cout << "\nАдмин просматривает историю заказа:" << std::endl;
        manager.getOrderStatusHistory(adminUserId, testOrder1);

        // Покупатель просматривает свою историю
        std::cout << "\nПокупатель просматривает историю своего заказа:" << std::endl;
        manager.getOrderStatusHistory(customerUserId, testOrder1);

        // 5. Демонстрация работы с аудит-логом
        std::cout << "\n5. Демонстрация работы с аудит-логом..." << std::endl;

        // Админ просматривает полный аудит-лог
        std::cout << "\nАдмин просматривает аудит-лог:" << std::endl;
        manager.getAuditLog(adminUserId);

        // Менеджер просматривает аудит (только заказы)
        std::cout << "\nМенеджер просматривает аудит-лог:" << std::endl;
        manager.getAuditLog(managerUserId);

        // Покупатель пытается просмотреть аудит (должна быть ошибка)
        std::cout << "\nПокупатель пытается просмотреть аудит-лог:" << std::endl;
        manager.getAuditLog(customerUserId);

        // 6. Менеджер просматривает утвержденные заказы
        std::cout << "\n6. Менеджер просматривает утвержденные заказы..." << std::endl;

        // Добавляем запись в историю от имени менеджера
        db->executeQuery(
            "INSERT INTO order_status_history (order_id, old_status, new_status, changed_by) "
            "VALUES ($1, 'pending', 'completed', $2)",
            testOrder2, managerUserId);

        manager.getManagerApprovedOrders(managerUserId);

        // 7. Статистика и аналитика
        std::cout << "\n7. Статистика и аналитика..." << std::endl;

        // Админ просматривает статистику
        manager.getStatusChangeStatistics(adminUserId, "7 days");

        // Популярные действия в аудите
        manager.getPopularAuditActions(adminUserId);

        // 8. Использование STL алгоритмов для анализа данных
        std::cout << "\n8. Анализ данных с использованием STL алгоритмов..." << std::endl;

        // Получаем все записи истории
        auto allHistory = db->executeQuery(
            "SELECT changed_by, new_status FROM order_status_history LIMIT 20");

        if (!allHistory.empty()) {
            // Группируем изменения по пользователям
            std::map<int, std::map<std::string, int>> changesByUser;

            std::for_each(allHistory.begin(), allHistory.end(),
                [&changesByUser](const auto& row) {
                    int userId = std::stoi(row[0]);
                    std::string status = row[1];
                    changesByUser[userId][status]++;
                });

            std::cout << "Анализ активности пользователей:" << std::endl;
            std::for_each(changesByUser.begin(), changesByUser.end(),
                [](const auto& userChanges) {
                    std::cout << "  Пользователь #" << userChanges.first << ":" << std::endl;

                    // Находим самый частый статус
                    auto mostFrequent = std::max_element(
                        userChanges.second.begin(), userChanges.second.end(),
                        [](const auto& a, const auto& b) {
                            return a.second < b.second;
                        });

                    if (mostFrequent != userChanges.second.end()) {
                        std::cout << "    Чаще всего менял на: " << mostFrequent->first
                                  << " (" << mostFrequent->second << " раз)" << std::endl;
                    }
                }
            );
        }

        std::cout << "\n=== ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка демонстрации системы истории и аудита: " << e.what() << std::endl;
    }
}