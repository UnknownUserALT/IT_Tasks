#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <functional>
std::ofstream logFile;

// Функция для получения текущего времени в формате строки
std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now = *std::localtime(&time_t_now);

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Функция для записи в лог
void writeToLog(const std::string& message) {
    if (!logFile.is_open()) {
        logFile.open("log.txt", std::ios::app);
        if (!logFile) {
            std::cerr << "Не удалось открыть файл логов!" << std::endl;
            return;
        }
    }

    std::string timestamp = getCurrentTime();
    logFile << "[" << timestamp << "] " << message << std::endl;
    logFile.flush(); // Немедленно записываем на диск
}


// Функция для безопасного выполнения операций с откатом при ошибке
void executeInTransaction(pqxx::connection& conn, const std::string& operation_name,
                          std::function<void(pqxx::work&)> operation) {
    writeToLog("Начало транзакции: " + operation_name);

    try {
        pqxx::work txn(conn);
        std::cout << "\n[Транзакция начата: " << operation_name << "]" << std::endl;

        operation(txn);

        txn.commit();
        std::cout << "[Транзакция закоммичена: " << operation_name << "]" << std::endl;
        writeToLog("Транзакция успешно завершена: " + operation_name);
    } catch (const std::exception& e) {
        std::cerr << "[Транзакция откачена назад: " << operation_name << "]" << std::endl;
        std::cerr << "Ошибка: " << e.what() << std::endl;

        std::string error_msg = "Ошибка в транзакции " + operation_name + ": " + e.what();
        writeToLog(error_msg);

        throw;
    }
}

// Функции меню
void showAllTasks(pqxx::connection& conn) {
    writeToLog("Запрос: показать все задачи");

    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(
            "SELECT id, title, priority, due_date, status FROM tasks ORDER BY id"
        );

        std::cout << "\n=== Все задачи ===" << std::endl;
        if (result.empty()) {
            std::cout << "Задач нет." << std::endl;
            writeToLog("Результат: задачи не найдены");
        } else {
            std::cout << "Найдено задач: " << result.size() << std::endl;
            writeToLog("Результат: найдено " + std::to_string(result.size()) + " задач");

            for (const auto& row : result) {
                std::cout << "ID: " << row["id"].as<int>()
                          << " | Задача: " << row["title"].as<std::string>()
                          << " | Приоритет: " << row["priority"].as<std::string>()
                          << " | Срок: " << row["due_date"].as<std::string>()
                          << " | Статус: " << row["status"].as<std::string>() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        writeToLog("Ошибка при получении задач: " + std::string(e.what()));
        std::cerr << "Ошибка при получении задач: " << e.what() << std::endl;
    }
}

void showAllUsers(pqxx::connection& conn) {
    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(
            "SELECT id, name FROM users ORDER BY id"
        );

        std::cout << "\n=== Все пользователи ===" << std::endl;
        if (result.empty()) {
            std::cout << "Пользователей нет." << std::endl;
        } else {
            for (const auto& row : result) {
                std::cout << "ID: " << row["id"].as<int>()
                          << " | Имя: " << row["name"].as<std::string>() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при получении пользователей: " << e.what() << std::endl;
    }
}

void showTaskAssignments(pqxx::connection& conn) {
    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(R"(
            SELECT ta.assignment_id, t.title, u.name, ta.assigned_date
            FROM task_assignments ta
            JOIN tasks t ON ta.task_id = t.id
            JOIN users u ON ta.user_id = u.id
            ORDER BY ta.assignment_id
        )");

        std::cout << "\n=== Назначения задач ===" << std::endl;
        if (result.empty()) {
            std::cout << "Назначений нет." << std::endl;
        } else {
            for (const auto& row : result) {
                std::cout << "Назначение ID: " << row["assignment_id"].as<int>()
                          << " | Задача: " << row["title"].as<std::string>()
                          << " | Пользователь: " << row["name"].as<std::string>()
                          << " | Дата назначения: " << row["assigned_date"].as<std::string>() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при получении назначений: " << e.what() << std::endl;
    }
}

void addTask(pqxx::connection& conn) {
    std::string title, priority, due_date, status;

    std::cout << "\n=== Добавление задачи ===" << std::endl;
    writeToLog("Операция: добавление новой задачи");

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Название задачи: ";
    std::getline(std::cin, title);

    std::cout << "Приоритет (Высокий/Средний/Низкий): ";
    std::getline(std::cin, priority);

    std::cout << "Срок выполнения (ГГГГ-ММ-ДД): ";
    std::getline(std::cin, due_date);

    std::cout << "Статус (Выполнена/В процессе/Отложена): ";
    std::getline(std::cin, status);

    writeToLog("Добавление задачи: \"" + title + "\", приоритет: " + priority +
           ", срок: " + due_date + ", статус: " + status);
    executeInTransaction(conn, "Добавление задачи", [&](pqxx::work& txn) {
        txn.exec_params(
            "INSERT INTO tasks (title, priority, due_date, status) VALUES ($1, $2, $3, $4)",
            title, priority, due_date, status
        );
        std::cout << "Задача добавлена: " << title << std::endl;
    });
}

void addUser(pqxx::connection& conn) {
    std::string name;

    std::cout << "\n=== Добавление пользователя ===" << std::endl;
    writeToLog("Операция: добавление нового пользователя");

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Имя пользователя: ";
    std::getline(std::cin, name);
    writeToLog("Добавление пользователя: \"" + name + "\"");

    executeInTransaction(conn, "Добавление пользователя", [&](pqxx::work& txn) {
        txn.exec_params(
            "INSERT INTO users (name) VALUES ($1)",
            name
        );
        std::cout << "Пользователь добавлен: " << name << std::endl;
    });
}

void assignTaskToUser(pqxx::connection& conn) {
    int task_id, user_id;

    std::cout << "\n=== Назначение задачи пользователю ===" << std::endl;
    writeToLog("Операция: назначение задачи пользователю");

    // Показать существующие задачи и пользователей
    showAllTasks(conn);
    showAllUsers(conn);

    std::cout << "\nВведите ID задачи: ";
    std::cin >> task_id;
    std::cout << "Введите ID пользователя: ";
    std::cin >> user_id;

    writeToLog("Назначение: задача " + std::to_string(task_id) +
               " -> пользователь " + std::to_string(user_id));

    executeInTransaction(conn, "Назначение задачи", [&](pqxx::work& txn) {
        txn.exec_params(
            "INSERT INTO task_assignments (task_id, user_id, assigned_date) VALUES ($1, $2, CURRENT_DATE)",
            task_id, user_id
        );
        std::cout << "Задача ID " << task_id << " назначена пользователю ID " << user_id << std::endl;
    });
}

void changeTaskStatus(pqxx::connection& conn) {
    int task_id;
    std::string new_status;

    std::cout << "\n=== Изменение статуса задачи ===" << std::endl;
    writeToLog("Операция: изменение статуса задачи");

    showAllTasks(conn);

    std::cout << "\nВведите ID задачи: ";
    std::cin >> task_id;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Введите новый статус (Выполнена/В процессе/Отложена): ";
    std::getline(std::cin, new_status);

    writeToLog("Изменение статуса задачи " + std::to_string(task_id) +
               " на: " + new_status);

    executeInTransaction(conn, "Изменение статуса задачи", [&](pqxx::work& txn) {
        // Сначала проверим, существует ли задача
        pqxx::result check = txn.exec_params(
            "SELECT 1 FROM tasks WHERE id = $1",
            task_id
        );

        if (check.empty()) {
            std::cout << "Задача с ID " << task_id << " не найдена." << std::endl;
            return;
        }

        // Выполняем обновление
        txn.exec_params(
            "UPDATE tasks SET status = $1 WHERE id = $2",
            new_status, task_id
        );

        std::cout << "Статус задачи ID " << task_id << " изменен на: " << new_status << std::endl;
    });
}

void showHighPriorityTasks(pqxx::connection& conn) {
    writeToLog("Аналитический запрос: задачи с высоким приоритетом");

    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(R"(
            SELECT id, title, priority, due_date, status
            FROM tasks
            WHERE priority = 'Высокий'
            ORDER BY due_date
        )");

        std::cout << "\n=== Задачи с высоким приоритетом ===" << std::endl;
        if (result.empty()) {
            std::cout << "Задач с высоким приоритетом нет." << std::endl;
        } else {
            for (const auto& row : result) {
                std::cout << "ID: " << row["id"].as<int>()
                          << " | Задача: " << row["title"].as<std::string>()
                          << " | Приоритет: " << row["priority"].as<std::string>()
                          << " | Срок: " << row["due_date"].as<std::string>()
                          << " | Статус: " << row["status"].as<std::string>() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при получении задач: " << e.what() << std::endl;
    }
}

void countUsersWithRecentAssignments(pqxx::connection& conn) {
    writeToLog("Аналитический запрос: пользователи с назначениями за 30 дней");

    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(R"(
            SELECT COUNT(DISTINCT user_id) as user_count
            FROM task_assignments
            WHERE assigned_date >= CURRENT_DATE - INTERVAL '30 days'
        )");

        std::cout << "\n=== Пользователи с назначениями за последние 30 дней ===" << std::endl;
        if (!result.empty()) {
            int user_count = result[0]["user_count"].as<int>();
            std::cout << "Количество пользователей: " << user_count << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при подсчете пользователей: " << e.what() << std::endl;
    }
}

void showTasksDueNextWeek(pqxx::connection& conn) {
    writeToLog("Аналитический запрос: задачи на следующую неделю");

    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(R"(
            SELECT id, title, priority, due_date, status
            FROM tasks
            WHERE due_date >= CURRENT_DATE
               AND due_date <= CURRENT_DATE + INTERVAL '7 days'
            ORDER BY due_date
        )");

        std::cout << "\n=== Задачи со сроком выполнения в течение следующей недели ===" << std::endl;
        if (result.empty()) {
            std::cout << "Задач на следующую неделю нет." << std::endl;
        } else {
            for (const auto& row : result) {
                std::cout << "ID: " << row["id"].as<int>()
                          << " | Задача: " << row["title"].as<std::string>()
                          << " | Приоритет: " << row["priority"].as<std::string>()
                          << " | Срок: " << row["due_date"].as<std::string>()
                          << " | Статус: " << row["status"].as<std::string>() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при получении задач: " << e.what() << std::endl;
    }
}

void showTopUsersByCompletedTasks(pqxx::connection& conn) {
    writeToLog("Аналитический запрос: топ-3 пользователей по выполненным задачам");

    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(R"(
            SELECT u.id, u.name, COUNT(t.id) as completed_count
            FROM users u
            JOIN task_assignments ta ON u.id = ta.user_id
            JOIN tasks t ON ta.task_id = t.id
            WHERE t.status = 'Выполнена'
            GROUP BY u.id, u.name
            ORDER BY completed_count DESC
            LIMIT 3
        )");

        std::cout << "\n=== Топ-3 пользователей по количеству выполненных задач ===" << std::endl;
        if (result.empty()) {
            std::cout << "Нет выполненных задач." << std::endl;
        } else {
            int rank = 1;
            for (const auto& row : result) {
                std::cout << rank++ << ". " << row["name"].as<std::string>()
                          << " (ID: " << row["id"].as<int>()
                          << ") - выполнено задач: " << row["completed_count"].as<int>() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при получении топ-пользователей: " << e.what() << std::endl;
    }
}

void displayMenu() {
    std::cout << "\n=== МЕНЮ УПРАВЛЕНИЯ ЗАДАЧАМИ ===" << std::endl;
    std::cout << "===== Основные операции =====" << std::endl;
    std::cout << "1. Показать все задачи" << std::endl;
    std::cout << "2. Показать всех пользователей" << std::endl;
    std::cout << "3. Показать назначения задач" << std::endl;
    std::cout << "4. Добавить задачу" << std::endl;
    std::cout << "5. Добавить пользователя" << std::endl;
    std::cout << "6. Назначить задачу пользователю" << std::endl;
    std::cout << "7. Изменить статус задачи" << std::endl;
    std::cout << "===== Аналитические запросы =====" << std::endl;
    std::cout << "8. Задачи с высоким приоритетом" << std::endl;
    std::cout << "9. Пользователи с назначениями за 30 дней" << std::endl;
    std::cout << "10. Задачи на следующую неделю" << std::endl;
    std::cout << "11. Топ-3 пользователей по выполненным задачам" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите опцию: ";
}



int main() {
    try {
        // Открываем файл логов
        logFile.open("log.txt", std::ios::app);
        if (!logFile) {
            std::cerr << "Предупреждение: не удалось открыть файл логов log.txt" << std::endl;
        } else {
            writeToLog("=== Запуск программы управления задачами ===");
        }


        // Строка подключения к базе данных postgres для создания новой БД
        std::string conninfo = "host=localhost port=5432 dbname=postgres user=postgres password=288288";

        // Устанавливаем соединение с базой данных postgres
        pqxx::connection conn(conninfo);

        if (conn.is_open()) {
            std::cout << "Подключение к БД: " << conn.dbname() << std::endl;
        } else {
            std::cerr << "Ошибка подключения к БД" << std::endl;
            return 1;
        }

        // Создаем транзакцию для создания базы данных
        pqxx::nontransaction ntxn(conn);

        // Проверяем, существует ли база данных task_management_db
        pqxx::result check_db = ntxn.exec(
            "SELECT 1 FROM pg_database WHERE datname = 'task_management_db'"
        );

        if (check_db.empty()) {
            std::cout << "Создание БД task_management_db..." << std::endl;
            ntxn.exec("CREATE DATABASE task_management_db");
            std::cout << "БД создана успешно!" << std::endl;
        } else {
            std::cout << "БД task_management_db уже существует." << std::endl;
        }

        // ВАЖНО: Закрываем соединение с postgres перед подключением к новой БД
        conn.close();

        // Подключаемся к новой базе данных
        std::string new_conninfo = "host=localhost port=5432 dbname=task_management_db user=postgres password=288288";
        pqxx::connection new_conn(new_conninfo);

        if (new_conn.is_open()) {
            std::cout << "Подключение к БД: " << new_conn.dbname() << std::endl;
        } else {
            std::cerr << "Неудалось подключиться к task_management_db." << std::endl;
            return 1;
        }

        // Создаем транзакцию для создания таблиц
        pqxx::work txn(new_conn);

        std::cout << "\nСоздание таблиц..." << std::endl;

        // Создаем таблицу users
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS users (
                id SERIAL PRIMARY KEY,
                name VARCHAR(255) NOT NULL
            )
        )");
        std::cout << "Таблица 'users' создана." << std::endl;

        // Создаем таблицу tasks
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS tasks (
                id SERIAL PRIMARY KEY,
                title VARCHAR(255) NOT NULL,
                priority VARCHAR(50) CHECK (priority IN ('Высокий', 'Средний', 'Низкий')),
                due_date DATE,
                status VARCHAR(50) CHECK (status IN ('Выполнена', 'В процессе', 'Отложена'))
            )
        )");
        std::cout << "Таблица 'tasks' создана." << std::endl;

        // Создаем таблицу task_assignments
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS task_assignments (
                assignment_id SERIAL PRIMARY KEY,
                task_id INTEGER NOT NULL,
                user_id INTEGER NOT NULL,
                FOREIGN KEY (task_id) REFERENCES tasks(id) ON DELETE CASCADE,
                FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
            )
        )");
        std::cout << "Таблица 'task_assignments' создана." << std::endl;

        // Фиксируем транзакцию
        txn.commit();

        std::cout << "\nВсе таблицы успешно созданы!" << std::endl;

        // Добавим немного тестовых данных для проверки
        pqxx::work insert_txn(new_conn);

        std::cout << "\nВставляем данные..." << std::endl;

        // Добавляем пользователей
        insert_txn.exec("INSERT INTO users (name) VALUES ('Иван Иванов'), ('Мария Петрова'), ('Алексей Сидоров')");

        // Добавляем задачи
        insert_txn.exec(R"(
            INSERT INTO tasks (title, priority, due_date, status) VALUES
            ('Разработать модуль авторизации', 'Высокий', '2025-12-15', 'В процессе'),
            ('Написать документацию', 'Средний', '2025-12-20', 'Отложена'),
            ('Провести code review', 'Высокий', '2025-12-10', 'Выполнена'),
            ('Исправить баги', 'Низкий', '2025-12-25', 'В процессе')
        )");

        // Назначаем задачи пользователям
        insert_txn.exec(R"(
            INSERT INTO task_assignments (task_id, user_id) VALUES
            (1, 1),
            (2, 2),
            (3, 1),
            (4, 3),
            (1, 3)
        )");

        insert_txn.commit();

        std::cout << "Данные вставлены успешно!" << std::endl;

        // ============================================
        // Часть 2.1: Создание индексов
        // ============================================

        executeInTransaction(new_conn, "Создание индексов", [](pqxx::work& txn) {
            std::cout << "Создание индексов...." << std::endl;

            // Индекс на поле due_date в таблице tasks
            txn.exec(R"(
                CREATE INDEX IF NOT EXISTS idx_tasks_due_date
                ON tasks(due_date)
            )");
            std::cout << "  - Индекс 'idx_tasks_due_date' создан для tasks(due_date)" << std::endl;

            // Индекс на поле status в таблице tasks
            txn.exec(R"(
                CREATE INDEX IF NOT EXISTS idx_tasks_status
                ON tasks(status)
            )");
            std::cout << "  - Индекс 'idx_tasks_status' создан для tasks(status)" << std::endl;

            // Добавляем поле assigned_date в таблицу task_assignments (если его нет)
            txn.exec(R"(
                ALTER TABLE task_assignments
                ADD COLUMN IF NOT EXISTS assigned_date DATE DEFAULT CURRENT_DATE
            )");
            std::cout << "  - Колонка 'assigned_date' добавлена к task_assignments" << std::endl;

            // Индекс на поле assigned_date в таблице task_assignments
            txn.exec(R"(
                CREATE INDEX IF NOT EXISTS idx_task_assignments_assigned_date
                ON task_assignments(assigned_date)
            )");
            std::cout << "  - Индекс 'idx_task_assignments_assigned_date' создан" << std::endl;
        });

        // ============================================
        // Часть 2.2: Транзакции - Добавление данных
        // ============================================

        executeInTransaction(new_conn, "Добавление нового пользователя", [](pqxx::work& txn) {
            txn.exec("INSERT INTO users (name) VALUES ('Дмитрий Козлов')");
            std::cout << "  - New user added: Дмитрий Козлов" << std::endl;
        });

        executeInTransaction(new_conn, "Добавление новой задачи", [](pqxx::work& txn) {
            txn.exec(R"(
                INSERT INTO tasks (title, priority, due_date, status)
                VALUES ('Оптимизация базы данных', 'Высокий', '2024-12-18', 'В процессе')
            )");
            std::cout << "  - Новая задача добавлена: Оптимизация базы данных" << std::endl;
        });

        executeInTransaction(new_conn, "Назначем задачу юзеру", [](pqxx::work& txn) {
            // Назначаем задачу #5 пользователю #4
            txn.exec(R"(
                INSERT INTO task_assignments (task_id, user_id, assigned_date)
                VALUES (5, 4, CURRENT_DATE)
            )");
            std::cout << "  - Задача #5 назнача на юзера #4" << std::endl;
        });

        int choice;
        bool running = true;

        while (running) {
            displayMenu();
            std::cin >> choice;

            switch (choice) {
                case 1:
                    showAllTasks(new_conn);
                    break;
                case 2:
                    showAllUsers(new_conn);
                    break;
                case 3:
                    showTaskAssignments(new_conn);
                    break;
                case 4:
                    addTask(new_conn);
                    break;
                case 5:
                    addUser(new_conn);
                    break;
                case 6:
                    assignTaskToUser(new_conn);
                    break;
                case 7:
                    changeTaskStatus(new_conn);
                    break;
                case 8:
                    showHighPriorityTasks(new_conn);
                    break;
                case 9:
                    countUsersWithRecentAssignments(new_conn);
                    break;
                case 10:
                    showTasksDueNextWeek(new_conn);
                    break;
                case 11:
                    showTopUsersByCompletedTasks(new_conn);
                    break;
                case 0:
                    std::cout << "Выход из программы..." << std::endl;
                    running = false;
                    break;
                default:
                    std::cout << "Неверный выбор! Попробуйте снова." << std::endl;
            }
        }

        new_conn.close();
        std::cout << "\n=== Программа успешно завершена ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "\nФатальная ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}