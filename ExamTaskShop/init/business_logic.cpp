// business_logic.cpp
#include "business_logic.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <memory>
#include <map>

// Структура для хранения информации о заказе
struct OrderInfo {
    int orderId;
    int userId;
    std::string status;
    double totalPrice;
    std::string orderDate;

    OrderInfo(int id, int uid, const std::string& stat, double price, const std::string& date)
        : orderId(id), userId(uid), status(stat), totalPrice(price), orderDate(date) {}
};

// Класс для работы с заказами с использованием STL алгоритмов
class OrderManager {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    OrderManager(std::shared_ptr<DatabaseConnection> database) : db(database) {}

    // 1. Фильтрация заказов по статусу с использованием std::copy_if и лямбды
    std::vector<OrderInfo> filterOrdersByStatus(int userId, const std::string& targetStatus) {
        std::vector<OrderInfo> filteredOrders;

        try {
            // Получение всех заказов пользователя
            auto ordersResult = db->executeQuery(
                "SELECT order_id, user_id, status, total_price, order_date "
                "FROM orders WHERE user_id = $1", userId);

            // Преобразование результата в вектор OrderInfo
            std::vector<OrderInfo> allOrders;
            std::transform(ordersResult.begin(), ordersResult.end(),
                          std::back_inserter(allOrders),
                          [](const std::vector<std::string>& row) -> OrderInfo {
                              int id = std::stoi(row[0]);
                              int uid = std::stoi(row[1]);
                              double price = std::stod(row[3]);
                              return OrderInfo(id, uid, row[2], price, row[4]);
                          });

            // Использование std::copy_if с лямбдой для фильтрации
            std::copy_if(allOrders.begin(), allOrders.end(),
                        std::back_inserter(filteredOrders),
                        [&targetStatus](const OrderInfo& order) {
                            // Лямбда-функция для фильтрации
                            return order.status == targetStatus;
                        });

            // Альтернативный вариант с использованием алгоритма remove_copy_if
            std::vector<OrderInfo> alternativeFiltered;
            std::remove_copy_if(allOrders.begin(), allOrders.end(),
                               std::back_inserter(alternativeFiltered),
                               [&targetStatus](const OrderInfo& order) {
                                   return order.status != targetStatus;
                               });

            std::cout << "Найдено заказов со статусом '" << targetStatus
                      << "': " << filteredOrders.size() << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка фильтрации заказов: " << e.what() << std::endl;
        }

        return filteredOrders;
    }

    // 2. Фильтрация заказов по диапазону дат с использованием лямбды
    std::vector<OrderInfo> filterOrdersByDateRange(const std::string& startDate,
                                                  const std::string& endDate,
                                                  const std::string& statusFilter = "") {
        std::vector<OrderInfo> filteredOrders;

        try {
            // Получение всех заказов
            std::string query = "SELECT order_id, user_id, status, total_price, order_date "
                               "FROM orders WHERE order_date BETWEEN $1 AND $2";

            if (!statusFilter.empty()) {
                query += " AND status = $3";
                auto ordersResult = db->executeQuery(query, startDate, endDate, statusFilter);

                std::transform(ordersResult.begin(), ordersResult.end(),
                              std::back_inserter(filteredOrders),
                              [](const std::vector<std::string>& row) -> OrderInfo {
                                  return OrderInfo(std::stoi(row[0]), std::stoi(row[1]),
                                                   row[2], std::stod(row[3]), row[4]);
                              });
            } else {
                auto ordersResult = db->executeQuery(query, startDate, endDate);

                // Использование лямбды в transform
                auto createOrderInfo = [](const std::vector<std::string>& row) -> OrderInfo {
                    return OrderInfo(std::stoi(row[0]), std::stoi(row[1]),
                                     row[2], std::stod(row[3]), row[4]);
                };

                std::transform(ordersResult.begin(), ordersResult.end(),
                              std::back_inserter(filteredOrders), createOrderInfo);
            }

        } catch (const std::exception& e) {
            std::cerr << "Ошибка фильтрации по датам: " << e.what() << std::endl;
        }

        return filteredOrders;
    }

    // 3. Агрегация данных: подсчет общей суммы заказов пользователя с std::accumulate
    double calculateUserTotalSpent(int userId) {
        double totalSpent = 0.0;

        try {
            auto ordersResult = db->executeQuery(
                "SELECT total_price FROM orders WHERE user_id = $1 AND status = 'completed'",
                userId);

            // Преобразование в вектор double
            std::vector<double> orderAmounts;
            std::transform(ordersResult.begin(), ordersResult.end(),
                          std::back_inserter(orderAmounts),
                          [](const std::vector<std::string>& row) {
                              return std::stod(row[0]);
                          });

            // Использование std::accumulate с лямбдой
            totalSpent = std::accumulate(orderAmounts.begin(), orderAmounts.end(), 0.0,
                [](double sum, double amount) {
                    return sum + amount;
                });

            // Альтернативный вариант с использованием функционального объекта
            struct Adder {
                double operator()(double sum, double amount) const {
                    return sum + amount;
                }
            };

            double alternativeTotal = std::accumulate(orderAmounts.begin(),
                                                     orderAmounts.end(), 0.0, Adder());

            std::cout << "Общая сумма потраченная пользователем " << userId
                      << ": $" << totalSpent << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка расчета общей суммы: " << e.what() << std::endl;
        }

        return totalSpent;
    }

    // 4. Подсчет количества заказов в определенном статусе с использованием алгоритмов STL
    std::map<std::string, int> countOrdersByStatus(int userId = -1) {
        std::map<std::string, int> statusCounts;

        try {
            std::string query = "SELECT status FROM orders";
            std::vector<std::vector<std::string>> ordersResult;

            if (userId != -1) {
                query += " WHERE user_id = $1";
                ordersResult = db->executeQuery(query, userId);
            } else {
                ordersResult = db->executeQuery(query);
            }

            // Преобразование в вектор статусов
            std::vector<std::string> statuses;
            std::transform(ordersResult.begin(), ordersResult.end(),
                          std::back_inserter(statuses),
                          [](const std::vector<std::string>& row) {
                              return row[0];
                          });

            // Использование std::for_each для подсчета
            std::for_each(statuses.begin(), statuses.end(),
                [&statusCounts](const std::string& status) {
                    statusCounts[status]++;
                });

            // Альтернативный вариант с использованием лямбды и алгоритма count_if
            std::vector<std::string> uniqueStatuses = {"pending", "completed", "canceled", "returned"};

            std::cout << "\nСтатистика заказов:" << std::endl;
            std::for_each(uniqueStatuses.begin(), uniqueStatuses.end(),
                [&statuses, &statusCounts](const std::string& status) {
                    int count = std::count_if(statuses.begin(), statuses.end(),
                        [&status](const std::string& s) {
                            return s == status;
                        });
                    std::cout << "  " << status << ": " << count << std::endl;
                    statusCounts[status] = count;
                });

        } catch (const std::exception& e) {
            std::cerr << "Ошибка подсчета заказов по статусам: " << e.what() << std::endl;
        }

        return statusCounts;
    }

    // 5. Проверка прав доступа с использованием лямбда-функции
   bool checkUserPermission(int userId, const std::string& action,
                           const std::string& targetEntity = "", int entityId = -1) {

        try {
            // Получение роли пользователя
            auto userResult = db->executeQuery(
                "SELECT role FROM users WHERE user_id = $1", userId);

            if (userResult.empty()) {
                std::cerr << "Пользователь не найден: " << userId << std::endl;
                return false;
            }

            std::string userRole = userResult[0][0];

            // Лямбда-функция для проверки прав доступа с захватом userId
            auto checkPermission = [&, userId, userRole, action, targetEntity, entityId]() -> bool {
                // Проверка прав в зависимости от роли и действия
                if (userRole == "admin") {
                    // Админ имеет все права
                    return true;
                }
                else if (userRole == "manager") {
                    // Менеджер может управлять заказами и товарами
                    std::vector<std::string> allowedActions = {"view_orders", "update_order_status",
                                                              "manage_products", "view_reports"};
                    return std::find(allowedActions.begin(), allowedActions.end(), action)
                           != allowedActions.end();
                }
                else if (userRole == "customer") {
                    // Покупатель может управлять только своими заказами
                    if (action == "view_own_orders" || action == "cancel_own_order") {
                        if (targetEntity == "order" && entityId != -1) {
                            // Проверяем, принадлежит ли заказ пользователю
                            auto orderCheck = db->executeQuery(
                                "SELECT user_id FROM orders WHERE order_id = $1", entityId);

                            if (!orderCheck.empty()) {
                                int orderOwnerId = std::stoi(orderCheck[0][0]);
                                return orderOwnerId == userId;  // Сравниваем с захваченным userId
                            }
                        }
                        return false;
                    }
                    std::vector<std::string> allowedActions = {"view_own_orders", "create_order",
                                                              "cancel_own_order", "view_products"};
                    return std::find(allowedActions.begin(), allowedActions.end(), action)
                           != allowedActions.end();
                }

                return false;
            };

            bool hasPermission = checkPermission();

            std::cout << "Проверка прав: пользователь " << userId
                      << " (роль: " << userRole << ") "
                      << "действие: " << action << " - "
                      << (hasPermission ? "РАЗРЕШЕНО" : "ЗАПРЕЩЕНО") << std::endl;

            return hasPermission;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка проверки прав доступа: " << e.what() << std::endl;
            return false;
        }
    }

    // 6. Проверка возможности изменения статуса заказа с лямбдой
    bool canChangeOrderStatus(int userId, int orderId, const std::string& newStatus) {
        // Комплексная лямбда-функция для проверки
        auto statusChangeValidator = [this, userId, orderId, newStatus]() -> std::pair<bool, std::string> {
            try {
                // 1. Проверка существования заказа
                auto orderResult = db->executeQuery(
                    "SELECT user_id, status FROM orders WHERE order_id = $1", orderId);

                if (orderResult.empty()) {
                    return {false, "Заказ не найден"};
                }

                int orderOwner = std::stoi(orderResult[0][0]);
                std::string currentStatus = orderResult[0][1];

                // 2. Проверка прав доступа
                auto userResult = db->executeQuery(
                    "SELECT role FROM users WHERE user_id = $1", userId);

                if (userResult.empty()) {
                    return {false, "Пользователь не найден"};
                }

                std::string userRole = userResult[0][0];

                // 3. Лямбда для проверки правил изменения статуса
                auto checkStatusTransition = [&userRole, &currentStatus, &newStatus, orderOwner, userId]() -> bool {
                    // Правила для покупателя
                    if (userRole == "customer") {
                        if (orderOwner != userId) {
                            return false;
                        }
                        // Покупатель может только отменять pending заказы
                        return (newStatus == "canceled" && currentStatus == "pending");
                    }

                    // Правила для менеджера
                    if (userRole == "manager") {
                        // Менеджер может утверждать и отменять pending заказы
                        if (currentStatus == "pending") {
                            return (newStatus == "completed" || newStatus == "canceled");
                        }
                        // Менеджер может обрабатывать возвраты
                        if (currentStatus == "completed" && newStatus == "returned") {
                            return true;
                        }
                        return false;
                    }

                    // Правила для администратора
                    if (userRole == "admin") {
                        // Админ может все, кроме нелогичных переходов
                        std::vector<std::string> invalidTransitions = {
                            "canceled_to_pending", "returned_to_pending",
                            "returned_to_completed", "canceled_to_completed"
                        };

                        std::string transition = currentStatus + "_to_" + newStatus;
                        return std::find(invalidTransitions.begin(), invalidTransitions.end(),
                                        transition) == invalidTransitions.end();
                    }

                    return false;
                };

                if (!checkStatusTransition()) {
                    return {false, "Недопустимый переход статуса для вашей роли"};
                }

                // 4. Дополнительные проверки
                if (newStatus == "returned" && currentStatus == "completed") {
                    // Проверяем, прошло ли более 30 дней
                    auto orderDateResult = db->executeQuery(
                        "SELECT order_date FROM orders WHERE order_id = $1", orderId);

                    if (!orderDateResult.empty()) {
                        // Здесь можно добавить проверку времени
                        // (упрощенная версия)
                    }
                }

                return {true, "Можно изменить"};

            } catch (const std::exception& e) {
                return {false, std::string("Ошибка проверки: ") + e.what()};
            }
        };

        auto [canChange, reason] = statusChangeValidator();

        if (!canChange) {
            std::cerr << "Невозможно изменить статус заказа " << orderId
                      << ": " << reason << std::endl;
        } else {
            std::cout << "Можно изменить статус заказа " << orderId
                      << " на '" << newStatus << "'" << std::endl;
        }

        return canChange;
    }

    // 7. Поиск заказов с наибольшей суммой с использованием алгоритмов STL
    std::vector<OrderInfo> findTopOrders(int limit = 10, const std::string& statusFilter = "") {
        std::vector<OrderInfo> topOrders;

        try {
            std::string query = "SELECT order_id, user_id, status, total_price, order_date "
                               "FROM orders ";

            if (!statusFilter.empty()) {
                query += "WHERE status = $1 ";
            }

            query += "ORDER BY total_price DESC LIMIT " + std::to_string(limit);

            std::vector<std::vector<std::string>> ordersResult;
            if (!statusFilter.empty()) {
                ordersResult = db->executeQuery(query, statusFilter);
            } else {
                ordersResult = db->executeQuery(query);
            }

            // Преобразование результата
            std::transform(ordersResult.begin(), ordersResult.end(),
                          std::back_inserter(topOrders),
                          [](const std::vector<std::string>& row) -> OrderInfo {
                              return OrderInfo(std::stoi(row[0]), std::stoi(row[1]),
                                               row[2], std::stod(row[3]), row[4]);
                          });

            // Использование алгоритма sort с пользовательской лямбдой
            std::sort(topOrders.begin(), topOrders.end(),
                [](const OrderInfo& a, const OrderInfo& b) {
                    return a.totalPrice > b.totalPrice; // Сортировка по убыванию суммы
                });

            // Вывод топ заказов
            std::cout << "\nТоп-" << limit << " самых дорогих заказов:" << std::endl;
            std::for_each(topOrders.begin(), topOrders.end(),
                [](const OrderInfo& order) {
                    std::cout << "  Заказ #" << order.orderId
                              << " - $" << order.totalPrice
                              << " (" << order.status << ")" << std::endl;
                });

        } catch (const std::exception& e) {
            std::cerr << "Ошибка поиска топ заказов: " << e.what() << std::endl;
        }

        return topOrders;
    }

    // 8. Группировка заказов по пользователям с использованием std::accumulate
    std::map<int, double> groupOrdersByUser() {
        std::map<int, double> userTotals;

        try {
            auto ordersResult = db->executeQuery(
                "SELECT user_id, total_price FROM orders WHERE status = 'completed'");

            // Использование std::accumulate для агрегации
            userTotals = std::accumulate(ordersResult.begin(), ordersResult.end(),
                                        std::map<int, double>(),
                [](std::map<int, double> totals, const std::vector<std::string>& row) {
                    int userId = std::stoi(row[0]);
                    double amount = std::stod(row[1]);
                    totals[userId] += amount;
                    return totals;
                });

            // Сортировка пользователей по сумме заказов (от большего к меньшему)
            std::vector<std::pair<int, double>> sortedUsers(userTotals.begin(), userTotals.end());
            std::sort(sortedUsers.begin(), sortedUsers.end(),
                [](const auto& a, const auto& b) {
                    return a.second > b.second;
                });

            std::cout << "\nТоп пользователей по сумме заказов:" << std::endl;
            std::for_each(sortedUsers.begin(),
                         sortedUsers.size() > 5 ? sortedUsers.begin() + 5 : sortedUsers.end(),
                [](const auto& userData) {
                    std::cout << "  Пользователь #" << userData.first
                              << ": $" << userData.second << std::endl;
                });

        } catch (const std::exception& e) {
            std::cerr << "Ошибка группировки заказов: " << e.what() << std::endl;
        }

        return userTotals;
    }
};

// Функция для демонстрации работы бизнес-логики
void demonstrateBusinessLogic(std::shared_ptr<DatabaseConnection> db) {
    try {
        std::cout << "\n\n=== ДЕМОНСТРАЦИЯ БИЗНЕС-ЛОГИКИ С ЛЯМБДАМИ И STL ===" << std::endl;

        OrderManager orderManager(db);

        // 1. Демонстрация фильтрации заказов
        std::cout << "\n1. Фильтрация заказов по статусу 'pending':" << std::endl;
        auto pendingOrders = orderManager.filterOrdersByStatus(1, "pending");

        // 2. Демонстрация агрегации данных
        std::cout << "\n2. Агрегация данных пользователя:" << std::endl;
        double totalSpent = orderManager.calculateUserTotalSpent(1);

        // 3. Подсчет заказов по статусам
        std::cout << "\n3. Статистика заказов по статусам:" << std::endl;
        auto statusCounts = orderManager.countOrdersByStatus();

        // 4. Проверка прав доступа
        std::cout << "\n4. Проверка прав доступа:" << std::endl;
        bool canViewOrders = orderManager.checkUserPermission(1, "view_orders");
        bool canManageProducts = orderManager.checkUserPermission(2, "manage_products");

        // 5. Проверка возможности изменения статуса
        std::cout << "\n5. Проверка изменения статуса заказа:" << std::endl;
        bool canCancel = orderManager.canChangeOrderStatus(1, 1, "canceled");

        // 6. Поиск топ заказов
        std::cout << "\n6. Поиск самых дорогих заказов:" << std::endl;
        auto topOrders = orderManager.findTopOrders(5);

        // 7. Группировка заказов по пользователям
        std::cout << "\n7. Группировка заказов по пользователям:" << std::endl;
        auto userGroups = orderManager.groupOrdersByUser();

        // 8. Дополнительный пример: сложная фильтрация с лямбдой
        std::cout << "\n8. Сложная фильтрация заказов:" << std::endl;

        // Получение всех заказов
        auto allOrdersResult = db->executeQuery(
            "SELECT order_id, user_id, status, total_price, order_date FROM orders LIMIT 20");

        std::vector<OrderInfo> allOrders;
        std::transform(allOrdersResult.begin(), allOrdersResult.end(),
                      std::back_inserter(allOrders),
                      [](const std::vector<std::string>& row) -> OrderInfo {
                          return OrderInfo(std::stoi(row[0]), std::stoi(row[1]),
                                           row[2], std::stod(row[3]), row[4]);
                      });

        // Использование std::partition для разделения заказов на дорогие и дешевые
        auto partitionPoint = std::partition(allOrders.begin(), allOrders.end(),
            [](const OrderInfo& order) {
                return order.totalPrice > 100.0; // Дорогие заказы
            });

        std::cout << "  Дорогие заказы (> $100): "
                  << std::distance(allOrders.begin(), partitionPoint) << std::endl;
        std::cout << "  Дешевые заказы (<= $100): "
                  << std::distance(partitionPoint, allOrders.end()) << std::endl;

        // 9. Использование std::all_of, std::any_of, std::none_of
        std::cout << "\n9. Проверка условий для заказов:" << std::endl;

        bool allCompleted = std::all_of(allOrders.begin(), allOrders.end(),
            [](const OrderInfo& order) {
                return order.status == "completed";
            });

        bool anyPending = std::any_of(allOrders.begin(), allOrders.end(),
            [](const OrderInfo& order) {
                return order.status == "pending";
            });

        bool noneCanceled = std::none_of(allOrders.begin(), allOrders.end(),
            [](const OrderInfo& order) {
                return order.status == "canceled";
            });

        std::cout << "  Все заказы завершены: " << (allCompleted ? "ДА" : "НЕТ") << std::endl;
        std::cout << "  Есть pending заказы: " << (anyPending ? "ДА" : "НЕТ") << std::endl;
        std::cout << "  Нет canceled заказов: " << (noneCanceled ? "ДА" : "НЕТ") << std::endl;

        std::cout << "\n=== ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка демонстрации бизнес-логики: " << e.what() << std::endl;
    }
}