// AdminMenu.cpp
// Интерактивное меню администратора
#include "DatabaseConnection.hpp"
#include "Admin.cpp"
#include <iostream>
#include <memory>
#include <limits>
#include <iomanip>

class AdminMenu {
private:
    std::shared_ptr<DatabaseConnection> db;
    std::unique_ptr<Admin> admin;

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
        std::cout << "║       ИНТЕРНЕТ-МАГАЗИН - ПАНЕЛЬ АДМИНА         ║" << std::endl;
        std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
        std::cout << "Администратор: " << admin->getName()
                  << " (ID: " << admin->getUserId() << ")" << std::endl;
        std::cout << "🔑 Полный доступ к системе" << std::endl << std::endl;
    }

    void displayMenu() {
        std::cout << "┌────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│  1. Управление товарами                        │" << std::endl;
        std::cout << "│  2. Управление заказами                        │" << std::endl;
        std::cout << "│  3. Просмотр всех пользователей                │" << std::endl;
        std::cout << "│  4. Просмотр аудит-лога                        │" << std::endl;
        std::cout << "│  5. Генерация отчетов                          │" << std::endl;
        std::cout << "│  6. Статистика системы                         │" << std::endl;
        std::cout << "│  7. Выход                                      │" << std::endl;
        std::cout << "└────────────────────────────────────────────────┘" << std::endl;
        std::cout << "\nВыберите действие (1-7): ";
    }

public:
    AdminMenu(std::shared_ptr<DatabaseConnection> database, int userId)
        : db(database) {
        auto userInfo = db->executeQuery(
            "SELECT name, email FROM users WHERE user_id = $1 AND role = 'admin'",
            userId);

        if (userInfo.empty()) {
            throw std::runtime_error("Администратор не найден или доступ запрещен");
        }

        std::string name = userInfo[0][0];
        std::string email = userInfo[0][1];

        admin = std::make_unique<Admin>(userId, name, email, db);
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
                std::cout << "\n❌ Ошибка: Введите число от 1 до 7" << std::endl;
                pause();
                continue;
            }

            clearInputBuffer();

            switch (choice) {
                case 1:
                    manageProducts();
                    break;
                case 2:
                    manageOrders();
                    break;
                case 3:
                    viewAllUsers();
                    break;
                case 4:
                    viewAuditLog();
                    break;
                case 5:
                    generateReports();
                    break;
                case 6:
                    viewStatistics();
                    break;
                case 7:
                    std::cout << "\n👋 До свидания, администратор!" << std::endl;
                    running = false;
                    break;
                default:
                    std::cout << "\n❌ Неверный выбор" << std::endl;
                    pause();
            }
        }
    }

private:
    void manageProducts() {
        std::cout << "\n═══ УПРАВЛЕНИЕ ТОВАРАМИ ═══" << std::endl;
        std::cout << "1. Добавить товар" << std::endl;
        std::cout << "2. Обновить товар" << std::endl;
        std::cout << "3. Удалить товар" << std::endl;
        std::cout << "4. Просмотр всех товаров" << std::endl;
        std::cout << "5. Назад" << std::endl;
        std::cout << "\nВыбор: ";

        int choice;
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1: addProduct(); break;
            case 2: updateProduct(); break;
            case 3: deleteProduct(); break;
            case 4: viewAllProducts(); break;
            case 5: return;
        }
        pause();
    }

    void addProduct() {
        std::cout << "\n--- Добавление товара ---" << std::endl;

        std::string name;
        double price;
        int quantity;

        std::cout << "Название товара: ";
        std::getline(std::cin, name);

        std::cout << "Цена: ";
        std::cin >> price;

        std::cout << "Количество на складе: ";
        std::cin >> quantity;

        if (admin->addProduct(name, price, quantity)) {
            std::cout << "✓ Товар добавлен!" << std::endl;
        }
    }

    void updateProduct() {
        viewAllProducts();

        std::cout << "\n--- Обновление товара ---" << std::endl;
        int productId;
        std::cout << "ID товара для обновления: ";
        std::cin >> productId;
        clearInputBuffer();

        std::cout << "Новое название (Enter для пропуска): ";
        std::string name;
        std::getline(std::cin, name);

        std::cout << "Новая цена (0 для пропуска): ";
        double price;
        std::cin >> price;

        std::cout << "Новое количество (-1 для пропуска): ";
        int quantity;
        std::cin >> quantity;

        if (admin->updateProduct(productId, name, price, quantity)) {
            std::cout << "✓ Товар обновлен!" << std::endl;
        }
    }

    void deleteProduct() {
        viewAllProducts();

        std::cout << "\n--- Удаление товара ---" << std::endl;
        int productId;
        std::cout << "ID товара для удаления: ";
        std::cin >> productId;

        std::cout << "⚠ Вы уверены? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            if (admin->deleteProduct(productId)) {
                std::cout << "✓ Товар удален!" << std::endl;
            }
        }
    }

    void viewAllProducts() {
        try {
            auto products = db->executeQuery(
                "SELECT product_id, name, price, stock_quantity FROM products ORDER BY product_id");

            std::cout << "\n┌────┬─────────────────────────┬──────────┬──────────┐" << std::endl;
            std::cout << "│ ID │ Название                │ Цена     │ Остаток  │" << std::endl;
            std::cout << "├────┼─────────────────────────┼──────────┼──────────┤" << std::endl;

            for (const auto& p : products) {
                std::cout << "│ " << std::setw(2) << p[0]
                         << " │ " << std::setw(23) << std::left << p[1]
                         << " │ $" << std::setw(7) << std::right << p[2]
                         << " │ " << std::setw(8) << p[3] << " │" << std::endl;
            }
            std::cout << "└────┴─────────────────────────┴──────────┴──────────┘" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }
    }

    void manageOrders() {
        std::cout << "\n═══ УПРАВЛЕНИЕ ЗАКАЗАМИ ═══" << std::endl;

        try {
            auto orders = admin->viewAllOrders();

            std::cout << "\n┌─────────┬──────────────────────┬────────────┬───────────┬─────────┐" << std::endl;
            std::cout << "│ Заказ # │ Клиент               │ Статус     │ Сумма     │ Позиций │" << std::endl;
            std::cout << "├─────────┼──────────────────────┼────────────┼───────────┼─────────┤" << std::endl;

            for (const auto& order : orders) {
                std::cout << "│ " << std::setw(7) << std::left << order[0]
                         << " │ " << std::setw(20) << order[1]
                         << " │ " << std::setw(10) << order[2]
                         << " │ $" << std::setw(8) << std::right << order[3]
                         << " │ " << std::setw(7) << order[5] << " │" << std::endl;
            }
            std::cout << "└─────────┴──────────────────────┴────────────┴───────────┴─────────┘" << std::endl;

            std::cout << "\nВсего заказов: " << orders.size() << std::endl;

            std::cout << "\nИзменить статус заказа? (y/n): ";
            char choice;
            std::cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                int orderId;
                std::string newStatus;

                std::cout << "ID заказа: ";
                std::cin >> orderId;

                std::cout << "Новый статус (pending/completed/canceled/returned): ";
                std::cin >> newStatus;

                if (admin->updateOrderStatus(orderId, newStatus)) {
                    std::cout << "✓ Статус обновлен!" << std::endl;
                }
            }

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    void viewAllUsers() {
        std::cout << "\n═══ ВСЕ ПОЛЬЗОВАТЕЛИ ═══" << std::endl;

        try {
            auto users = db->executeQuery(
                "SELECT user_id, name, email, role, loyalty_level FROM users ORDER BY user_id");

            std::cout << "\n┌────┬─────────────────────────┬──────────────────────────┬──────────┬─────────┐" << std::endl;
            std::cout << "│ ID │ Имя                     │ Email                    │ Роль     │ Статус  │" << std::endl;
            std::cout << "├────┼─────────────────────────┼──────────────────────────┼──────────┼─────────┤" << std::endl;

            for (const auto& user : users) {
                std::string status = (user[4] == "1") ? "PREMIUM" : "Обычный";
                std::cout << "│ " << std::setw(2) << user[0]
                         << " │ " << std::setw(23) << std::left << user[1]
                         << " │ " << std::setw(24) << user[2]
                         << " │ " << std::setw(8) << user[3]
                         << " │ " << std::setw(7) << status << " │" << std::endl;
            }
            std::cout << "└────┴─────────────────────────┴──────────────────────────┴──────────┴─────────┘" << std::endl;

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    void viewAuditLog() {
        std::cout << "\n═══ АУДИТ-ЛОГ СИСТЕМЫ ═══" << std::endl;

        try {
            auto logs = db->executeQuery(
                "SELECT al.log_id, al.entity_type, al.entity_id, al.operation, "
                "u.name, al.performed_at FROM audit_log al "
                "LEFT JOIN users u ON al.performed_by = u.user_id "
                "ORDER BY al.performed_at DESC LIMIT 50");

            std::cout << "\n┌──────┬──────────────┬───────┬──────────┬──────────────────────┬─────────────────────┐" << std::endl;
            std::cout << "│ ID   │ Тип          │ ID    │ Операция │ Пользователь         │ Дата                │" << std::endl;
            std::cout << "├──────┼──────────────┼───────┼──────────┼──────────────────────┼─────────────────────┤" << std::endl;

            for (const auto& log : logs) {
                std::string userName = log[4].empty() ? "Система" : log[4];
                std::cout << "│ " << std::setw(4) << std::left << log[0]
                         << " │ " << std::setw(12) << log[1]
                         << " │ " << std::setw(5) << log[2]
                         << " │ " << std::setw(8) << log[3]
                         << " │ " << std::setw(20) << userName
                         << " │ " << std::setw(19) << log[5].substr(0, 19) << " │" << std::endl;
            }
            std::cout << "└──────┴──────────────┴───────┴──────────┴──────────────────────┴─────────────────────┘" << std::endl;

            std::cout << "\nПоказано последних 50 записей" << std::endl;

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }

    void generateReports() {
        std::cout << "\n═══ ГЕНЕРАЦИЯ ОТЧЕТОВ ═══" << std::endl;
        std::cout << "Функция генерации отчетов доступна через основную программу." << std::endl;
        std::cout << "Запустите ./TaskExamShop для создания отчетов в CSV." << std::endl;
        pause();
    }

    void viewStatistics() {
        std::cout << "\n═══ СТАТИСТИКА СИСТЕМЫ ═══" << std::endl;

        try {
            // Общая статистика
            auto totalUsers = db->executeQuery("SELECT COUNT(*) FROM users");
            auto totalProducts = db->executeQuery("SELECT COUNT(*) FROM products");
            auto totalOrders = db->executeQuery("SELECT COUNT(*) FROM orders");
            auto totalRevenue = db->executeQuery(
                "SELECT COALESCE(SUM(total_price), 0) FROM orders WHERE status = 'completed'");

            std::cout << "\n📊 Общая статистика:" << std::endl;
            std::cout << "  Пользователей: " << totalUsers[0][0] << std::endl;
            std::cout << "  Товаров: " << totalProducts[0][0] << std::endl;
            std::cout << "  Заказов: " << totalOrders[0][0] << std::endl;
            std::cout << "  Выручка: $" << totalRevenue[0][0] << std::endl;

            // Статистика по статусам заказов
            auto orderStats = db->executeQuery(
                "SELECT status, COUNT(*), SUM(total_price) FROM orders "
                "GROUP BY status ORDER BY status");

            std::cout << "\n📦 Заказы по статусам:" << std::endl;
            for (const auto& stat : orderStats) {
                std::cout << "  " << std::setw(12) << std::left << stat[0]
                         << ": " << std::setw(4) << stat[1]
                         << " заказов, $" << stat[2] << std::endl;
            }

            // Топ товаров
            auto topProducts = db->executeQuery(
                "SELECT p.name, COUNT(oi.order_item_id) as sales "
                "FROM products p "
                "LEFT JOIN order_items oi ON p.product_id = oi.product_id "
                "GROUP BY p.product_id, p.name "
                "ORDER BY sales DESC LIMIT 5");

            std::cout << "\n🏆 Топ-5 товаров:" << std::endl;
            for (const auto& product : topProducts) {
                std::cout << "  " << product[0] << ": " << product[1] << " продаж" << std::endl;
            }

        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }

        pause();
    }
};

void runAdminMenu(std::shared_ptr<DatabaseConnection> db, int userId) {
    try {
        AdminMenu menu(db, userId);
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
    }
}
