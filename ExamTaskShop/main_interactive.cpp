// main_interactive.cpp
// Интерактивная версия программы с меню для разных ролей
#include <iostream>
#include "DatabaseConnection.hpp"
#include "CustomerMenu.cpp"
#include "AdminMenu.cpp"
#include "ManagerMenu.cpp"

void displayWelcome() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                                          ║" << std::endl;
    std::cout << "║          ДОБРО ПОЖАЛОВАТЬ В ИНТЕРНЕТ-МАГАЗИН!            ║" << std::endl;
    std::cout << "║                                                          ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
}

void displayRoleSelection() {
    std::cout << "╔═══════════════════════════════════════╗" << std::endl;
    std::cout << "║          ВЫБЕРИТЕ РОЛЬ                ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════╝" << std::endl;
    std::cout << "\n1. Покупатель (Customer)" << std::endl;
    std::cout << "2. Администратор (Admin)" << std::endl;
    std::cout << "3. Менеджер (Manager)" << std::endl;
    std::cout << "\nВаш выбор: ";
}

void showUsersByRole(std::shared_ptr<DatabaseConnection> db, const std::string& role, const std::string& roleDisplayName) {
    auto users = db->executeQuery(
        "SELECT user_id, name, email FROM users WHERE role = $1 ORDER BY user_id",
        role);

    if (users.empty()) {
        std::cout << "\n⚠ В системе нет " << roleDisplayName << "." << std::endl;

        // Создаем тестового пользователя если нет
        if (role == "customer") {
            std::cout << "Создание тестового покупателя..." << std::endl;
            db->executeNonQuery(R"(
                INSERT INTO users (name, email, role, password_hash, loyalty_level)
                VALUES ('Тестовый Покупатель', 'test@example.com', 'customer', 'hash', 0)
                ON CONFLICT (email) DO NOTHING
            )");
        } else if (role == "admin") {
            std::cout << "Создание тестового администратора..." << std::endl;
            db->executeNonQuery(R"(
                INSERT INTO users (name, email, role, password_hash, loyalty_level)
                VALUES ('Администратор', 'admin@example.com', 'admin', 'hash', 0)
                ON CONFLICT (email) DO NOTHING
            )");
        } else if (role == "manager") {
            std::cout << "Создание тестового менеджера..." << std::endl;
            db->executeNonQuery(R"(
                INSERT INTO users (name, email, role, password_hash, loyalty_level)
                VALUES ('Менеджер', 'manager@example.com', 'manager', 'hash', 0)
                ON CONFLICT (email) DO NOTHING
            )");
        }

        users = db->executeQuery(
            "SELECT user_id, name, email FROM users WHERE role = $1",
            role);
    }

    if (!users.empty()) {
        std::cout << "\n┌────┬───────────────────────┬──────────────────────┐" << std::endl;
        std::cout << "│ ID │ Имя                   │ Email                │" << std::endl;
        std::cout << "├────┼───────────────────────┼──────────────────────┤" << std::endl;

        for (const auto& user : users) {
            std::cout << "│ " << std::setw(2) << user[0]
                     << " │ " << std::setw(21) << std::left << user[1]
                     << " │ " << std::setw(20) << user[2] << " │" << std::endl;
        }
        std::cout << "└────┴───────────────────────┴──────────────────────┘" << std::endl;
    }
}

int main() {
    try {
        displayWelcome();

        // Подключение к базе данных
        std::cout << "Подключение к базе данных..." << std::endl;
        auto db = std::make_shared<DatabaseConnection>(
            "host=localhost port=5432 dbname=online_store user=postgres password=288288");

        if (!db->isConnected()) {
            std::cerr << "❌ Не удалось подключиться к базе данных" << std::endl;
            return 1;
        }

        std::cout << "✓ Подключение установлено" << std::endl << std::endl;

        // Выбор роли
        displayRoleSelection();

        int roleChoice;
        std::cin >> roleChoice;

        if (std::cin.fail() || roleChoice < 1 || roleChoice > 3) {
            std::cerr << "❌ Некорректный выбор роли" << std::endl;
            return 1;
        }

        std::cin.ignore();

        std::string selectedRole;
        std::string roleDisplayName;

        switch (roleChoice) {
            case 1:
                selectedRole = "customer";
                roleDisplayName = "покупателей";
                break;
            case 2:
                selectedRole = "admin";
                roleDisplayName = "администраторов";
                break;
            case 3:
                selectedRole = "manager";
                roleDisplayName = "менеджеров";
                break;
        }

        // Показываем пользователей выбранной роли
        std::cout << "\n╔═══════════════════════════════════════╗" << std::endl;
        std::cout << "║          ВХОД В СИСТЕМУ               ║" << std::endl;
        std::cout << "╚═══════════════════════════════════════╝" << std::endl;
        std::cout << "\nДоступные " << roleDisplayName << ":" << std::endl;

        showUsersByRole(db, selectedRole, roleDisplayName);

        int userId;
        std::cout << "\nВведите ваш ID для входа: ";
        std::cin >> userId;

        if (std::cin.fail()) {
            std::cerr << "❌ Некорректный ID" << std::endl;
            return 1;
        }

        std::cin.ignore();

        // Проверка существования пользователя и его роли
        auto userCheck = db->executeQuery(
            "SELECT name, role FROM users WHERE user_id = $1", userId);

        if (userCheck.empty()) {
            std::cerr << "❌ Пользователь с ID " << userId << " не найден" << std::endl;
            return 1;
        }

        if (userCheck[0][1] != selectedRole) {
            std::cerr << "❌ Этот пользователь не является " << roleDisplayName.substr(0, roleDisplayName.length() - 1) << std::endl;
            std::cout << "Роль пользователя: " << userCheck[0][1] << std::endl;
            return 1;
        }

        std::cout << "\n✓ Добро пожаловать, " << userCheck[0][0] << "!" << std::endl;
        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.get();

        // Запуск соответствующего меню
        if (selectedRole == "customer") {
            runCustomerMenu(db, userId);
        } else if (selectedRole == "admin") {
            runAdminMenu(db, userId);
        } else if (selectedRole == "manager") {
            runManagerMenu(db, userId);
        }

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
