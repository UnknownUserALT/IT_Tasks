// ManagerMenu.cpp
// Интерактивное меню менеджера
#include "DatabaseConnection.hpp"
#include "Manager.cpp"
#include <iostream>
#include <memory>
#include <limits>
#include <iomanip>

class ManagerMenu {
private:
    std::shared_ptr<DatabaseConnection> db;
    std::unique_ptr<Manager> manager;

    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void pause() {
        std::cout << "\nНажмите Enter для продолжения...";
        clearInputBuffer();
        std::cin.get();
    }

    void displayHeader() {
        std::cout << "\n╔════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║       ИНТЕРНЕТ-МАГАЗИН - ПАНЕЛЬ МЕНЕДЖЕРА      ║" << std::endl;
        std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
        std::cout << "Менеджер: " << manager->getName()
                  << " (ID: " << manager->getUserId() << ")" << std::endl;
        std::cout << "📋 Управление заказами и складом" << std::endl << std::endl;
    }

    void displayMenu() {
        std::cout << "┌────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│  1. Просмотр всех заказов                      │" << std::endl;
        std::cout << "│  2. Одобрить заказ                             │" << std::endl;
        std::cout << "│  3. Отменить заказ                             │" << std::endl;
        std::cout << "│  4. Просмотр статуса заказа                    │" << std::endl;
        std::cout << "│  5. Обновить остаток товара на складе          │" << std::endl;
        std::cout << "│  6. Просмотр товаров с низким остатком         │" << std::endl;
        std::cout << "│  7. История статусов заказа                    │" << std::endl;
        std::cout << "│  8. Статистика заказов                         │" << std::endl;
        std::cout << "│  9. Выход                                      │" << std::endl;
        std::cout << "└────────────────────────────────────────────────┘" << std::endl;
        std::cout << "\nВыберите действие (1-9): ";
    }

public:
    ManagerMenu(std::shared_ptr<DatabaseConnection> database, int userId)
        : db(database) {
        auto userInfo = db->executeQuery(
            "SELECT name, email FROM users WHERE user_id = $1 AND role = 'manager'",
            userId);

        if (userInfo.empty()) {
            throw std::runtime_error("Менеджер не найден или доступ запрещен");
        }

        std::string name = userInfo[0][0];
        std::string email = userInfo[0][1];

        manager = std::make_unique<Manager>(userId, name, email, db);
    }

    void run() {
        int choice = 0;
        bool running = true;

        while (running) {
            system("clear || cls");
            displayHeader();
            displayMenu();

            std::cin >> choice;

            if (std::cin.fail()) {
                clearInputBuffer();
                std::cout << "\n❌ Ошибка: Введите число от 1 до 9" << std::endl;
                pause();
                continue;
            }

            clearInputBuffer();

            switch (choice) {
                case 1:
                    viewAllOrders();
                    break;
                case 2:
                    approveOrder();
                    break;
                case 3:
                    cancelOrder();
                    break;
                case 4:
                    viewOrderStatus();
                    break;
                case 5:
                    updateStock();
                    break;
                case 6:
                    viewLowStockProducts();
                    break;
                case 7:
                    viewOrderHistory();
                    break;
                case 8:
                    viewStatistics();
                    break;
                case 9:
                    std::cout << "\n👋 До свидания, менеджер!" << std::endl;
                    running = false;
                    break;
                default:
                    std::cout << "\n❌ Неверный выбор" << std::endl;
                    pause();
            }
        }
    }

private:
    void viewAllOrders() {
        std::cout << "\n═══ ВСЕ ЗАКАЗЫ ═══" << std::endl;

        try {
            auto orders = db->executeQuery(
                "SELECT o.order_id, u.name, o.status, o.total_price, o.order_date, "
                "COUNT(oi.order_item_id) as items_count "
                "FROM orders o "
                "LEFT JOIN users u ON o.user_id = u.user_id "
                "LEFT JOIN order_items oi ON o.order_id = oi.order_id "
                "GROUP BY o.order_id, u.name "
                "ORDER BY o.order_date DESC");

            if (orders.empty()) {
                std::cout << "Заказов нет" << std::endl;
                pause();
                return;
            }

            std::cout << "\n┌─────────┬──────────────────────┬────────────┬───────────┬─────────────────────┬─────────┐" << std::endl;
            std::cout << "│ Заказ # │ Клиент               │ Статус     │ Сумма     │ Дата                │ Позиций │" << std::endl;
            std::cout << "├─────────┼──────────────────────┼────────────┼───────────┼─────────────────────┼─────────┤" << std::endl;

            for (const auto& order : orders) {
                std::cout << "│ " << std::setw(7) << std::left << order[0]
                         << " │ " << std::setw(20) << order[1]
                         << " │ " << std::setw(10) << order[2]
                         << " │ $" << std::setw(8) << std::right << order[3]
                         << " │ " << std::setw(19) << order[4].substr(0, 19)
                         << " │ " << std::setw(7) << order[5] << " │" << std::endl;
            }
            std::cout << "└─────────┴──────────────────────┴────────────┴───────────┴─────────────────────┴─────────┘" << std::endl;

            std::cout << "\nВсего заказов: " << orders.size() << std::endl;

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    void approveOrder() {
        std::cout << "\n═══ ОДОБРЕНИЕ ЗАКАЗА ═══" << std::endl;

        int orderId;
        std::cout << "Введите номер заказа для одобрения: ";
        std::cin >> orderId;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный номер заказа" << std::endl;
            pause();
            return;
        }

        clearInputBuffer();

        // Показываем информацию о заказе
        try {
            auto orderInfo = db->executeQuery(
                "SELECT o.status, u.name, o.total_price, "
                "COUNT(oi.order_item_id) as items_count "
                "FROM orders o "
                "LEFT JOIN users u ON o.user_id = u.user_id "
                "LEFT JOIN order_items oi ON o.order_id = oi.order_id "
                "WHERE o.order_id = $1 "
                "GROUP BY o.order_id, u.name",
                orderId);

            if (orderInfo.empty()) {
                std::cout << "❌ Заказ не найден" << std::endl;
                pause();
                return;
            }

            std::cout << "\nЗаказ #" << orderId << std::endl;
            std::cout << "Клиент: " << orderInfo[0][1] << std::endl;
            std::cout << "Статус: " << orderInfo[0][0] << std::endl;
            std::cout << "Сумма: $" << orderInfo[0][2] << std::endl;
            std::cout << "Позиций: " << orderInfo[0][3] << std::endl;

            // Показываем товары в заказе
            auto items = db->executeQuery(
                "SELECT p.name, oi.quantity, p.stock_quantity "
                "FROM order_items oi "
                "JOIN products p ON oi.product_id = p.product_id "
                "WHERE oi.order_id = $1",
                orderId);

            if (!items.empty()) {
                std::cout << "\nТовары в заказе:" << std::endl;
                std::cout << "┌─────────────────────────┬──────────┬──────────┐" << std::endl;
                std::cout << "│ Товар                   │ Заказано │ Остаток  │" << std::endl;
                std::cout << "├─────────────────────────┼──────────┼──────────┤" << std::endl;

                for (const auto& item : items) {
                    std::cout << "│ " << std::setw(23) << std::left << item[0]
                             << " │ " << std::setw(8) << std::right << item[1]
                             << " │ " << std::setw(8) << item[2] << " │" << std::endl;
                }
                std::cout << "└─────────────────────────┴──────────┴──────────┘" << std::endl;
            }

            std::cout << "\n⚠ Одобрить заказ? (y/n): ";
            char confirm;
            std::cin >> confirm;

            if (confirm != 'y' && confirm != 'Y') {
                std::cout << "Операция отменена" << std::endl;
                pause();
                return;
            }

            clearInputBuffer();

            if (manager->approveOrder(orderId)) {
                std::cout << "\n✓ Заказ успешно одобрен!" << std::endl;
            } else {
                std::cout << "\n❌ Не удалось одобрить заказ" << std::endl;
            }

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    void cancelOrder() {
        std::cout << "\n═══ ОТМЕНА ЗАКАЗА ═══" << std::endl;

        int orderId;
        std::cout << "Введите номер заказа для отмены: ";
        std::cin >> orderId;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный номер заказа" << std::endl;
            pause();
            return;
        }

        clearInputBuffer();

        std::cout << "⚠ Вы уверены, что хотите отменить заказ? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "Операция отменена" << std::endl;
            pause();
            return;
        }

        clearInputBuffer();

        if (manager->cancelOrder(orderId)) {
            std::cout << "✓ Заказ успешно отменен!" << std::endl;
        } else {
            std::cout << "❌ Не удалось отменить заказ" << std::endl;
        }

        pause();
    }

    void viewOrderStatus() {
        std::cout << "\n═══ ПРОСМОТР СТАТУСА ЗАКАЗА ═══" << std::endl;

        int orderId;
        std::cout << "Введите номер заказа: ";
        std::cin >> orderId;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный номер заказа" << std::endl;
            pause();
            return;
        }

        std::string status = manager->viewOrderStatus(orderId);
        std::cout << "\nСтатус заказа #" << orderId << ": " << status << std::endl;

        pause();
    }

    void updateStock() {
        std::cout << "\n═══ ОБНОВЛЕНИЕ ОСТАТКА ТОВАРА ═══" << std::endl;

        // Показываем товары
        try {
            auto products = db->executeQuery(
                "SELECT product_id, name, stock_quantity FROM products ORDER BY product_id");

            std::cout << "\nТекущие остатки:" << std::endl;
            std::cout << "┌────┬─────────────────────────┬──────────┐" << std::endl;
            std::cout << "│ ID │ Название                │ Остаток  │" << std::endl;
            std::cout << "├────┼─────────────────────────┼──────────┤" << std::endl;

            for (const auto& p : products) {
                std::cout << "│ " << std::setw(2) << p[0]
                         << " │ " << std::setw(23) << std::left << p[1]
                         << " │ " << std::setw(8) << std::right << p[2] << " │" << std::endl;
            }
            std::cout << "└────┴─────────────────────────┴──────────┘" << std::endl;

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
            pause();
            return;
        }

        int productId;
        int newQuantity;

        std::cout << "\nВведите ID товара: ";
        std::cin >> productId;

        std::cout << "Новый остаток: ";
        std::cin >> newQuantity;

        if (std::cin.fail() || newQuantity < 0) {
            clearInputBuffer();
            std::cout << "❌ Некорректные данные" << std::endl;
            pause();
            return;
        }

        clearInputBuffer();

        if (manager->updateStock(productId, newQuantity)) {
            std::cout << "✓ Остаток обновлен!" << std::endl;
        } else {
            std::cout << "❌ Не удалось обновить остаток" << std::endl;
        }

        pause();
    }

    void viewLowStockProducts() {
        std::cout << "\n═══ ТОВАРЫ С НИЗКИМ ОСТАТКОМ ═══" << std::endl;

        try {
            auto products = db->executeQuery(
                "SELECT product_id, name, stock_quantity FROM products "
                "WHERE stock_quantity <= 10 ORDER BY stock_quantity ASC");

            if (products.empty()) {
                std::cout << "✓ Все товары в достаточном количестве" << std::endl;
                pause();
                return;
            }

            std::cout << "\n┌────┬─────────────────────────┬──────────┬──────────────┐" << std::endl;
            std::cout << "│ ID │ Название                │ Остаток  │ Статус       │" << std::endl;
            std::cout << "├────┼─────────────────────────┼──────────┼──────────────┤" << std::endl;

            for (const auto& p : products) {
                int stock = std::stoi(p[2]);
                std::string status = (stock == 0) ? "⚠ НЕТ" : "⚠ Мало";
                std::cout << "│ " << std::setw(2) << p[0]
                         << " │ " << std::setw(23) << std::left << p[1]
                         << " │ " << std::setw(8) << std::right << p[2]
                         << " │ " << std::setw(12) << std::left << status << " │" << std::endl;
            }
            std::cout << "└────┴─────────────────────────┴──────────┴──────────────┘" << std::endl;

            std::cout << "\nРекомендуется пополнить склад!" << std::endl;

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    void viewOrderHistory() {
        std::cout << "\n═══ ИСТОРИЯ СТАТУСОВ ЗАКАЗА ═══" << std::endl;

        int orderId;
        std::cout << "Введите номер заказа: ";
        std::cin >> orderId;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "❌ Некорректный номер заказа" << std::endl;
            pause();
            return;
        }

        try {
            auto history = db->executeQuery(
                "SELECT * FROM get_order_status_history($1)", orderId);

            if (history.empty()) {
                std::cout << "История для заказа #" << orderId << " не найдена" << std::endl;
                pause();
                return;
            }

            std::cout << "\n┌─────┬──────────────┬───────────┬─────────────────────┬──────────────────────┐" << std::endl;
            std::cout << "│ ID  │ Старый       │ Новый     │ Дата изменения      │ Изменил              │" << std::endl;
            std::cout << "├─────┼──────────────┼───────────┼─────────────────────┼──────────────────────┤" << std::endl;

            for (const auto& record : history) {
                std::string oldStatus = record[2].empty() ? "NULL" : record[2];
                std::string changedBy = record[6].empty() ? "Система" : record[6];

                std::cout << "│ " << std::setw(3) << std::left << record[0]
                         << " │ " << std::setw(12) << oldStatus
                         << " │ " << std::setw(9) << record[3]
                         << " │ " << std::setw(19) << record[4].substr(0, 19)
                         << " │ " << std::setw(20) << changedBy.substr(0, 20) << " │" << std::endl;
            }
            std::cout << "└─────┴──────────────┴───────────┴─────────────────────┴──────────────────────┘" << std::endl;

            std::cout << "\nВсего изменений: " << history.size() << std::endl;

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    void viewStatistics() {
        std::cout << "\n═══ СТАТИСТИКА ЗАКАЗОВ ═══" << std::endl;

        try {
            // Общая статистика
            auto totalOrders = db->executeQuery("SELECT COUNT(*) FROM orders");
            auto pendingOrders = db->executeQuery(
                "SELECT COUNT(*) FROM orders WHERE status = 'pending'");
            auto completedOrders = db->executeQuery(
                "SELECT COUNT(*) FROM orders WHERE status = 'completed'");
            auto totalRevenue = db->executeQuery(
                "SELECT COALESCE(SUM(total_price), 0) FROM orders WHERE status = 'completed'");

            std::cout << "\n📊 Общая статистика:" << std::endl;
            std::cout << "  Всего заказов: " << totalOrders[0][0] << std::endl;
            std::cout << "  Ожидают одобрения: " << pendingOrders[0][0] << std::endl;
            std::cout << "  Выполненных: " << completedOrders[0][0] << std::endl;
            std::cout << "  Общая выручка: $" << totalRevenue[0][0] << std::endl;

            // Заказы, требующие внимания
            auto pendingList = db->executeQuery(
                "SELECT order_id, total_price, order_date FROM orders "
                "WHERE status = 'pending' ORDER BY order_date ASC LIMIT 10");

            if (!pendingList.empty()) {
                std::cout << "\n⚠ Заказы, требующие одобрения:" << std::endl;
                std::cout << "┌─────────┬───────────┬─────────────────────┐" << std::endl;
                std::cout << "│ Заказ # │ Сумма     │ Дата                │" << std::endl;
                std::cout << "├─────────┼───────────┼─────────────────────┤" << std::endl;

                for (const auto& order : pendingList) {
                    std::cout << "│ " << std::setw(7) << std::left << order[0]
                             << " │ $" << std::setw(8) << std::right << order[1]
                             << " │ " << std::setw(19) << order[2].substr(0, 19) << " │" << std::endl;
                }
                std::cout << "└─────────┴───────────┴─────────────────────┘" << std::endl;
            }

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }
};

void runManagerMenu(std::shared_ptr<DatabaseConnection> db, int userId) {
    try {
        ManagerMenu menu(db, userId);
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
    }
}
