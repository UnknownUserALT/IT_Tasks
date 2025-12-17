#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <libpq-fe.h>
#include <algorithm>

// Структура для хранения данных о продажах (CSV)
struct RawSale {
    int sale_id;
    std::string sale_date_str;
    int product_id;
    int customer_id;
    int quantity;
    double amount;
};

// Функция для подключения к БД
PGconn* connectDB(const std::string& conninfo) {
    PGconn* conn = PQconnectdb(conninfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Ошибка подключения: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    }
    std::cout << "Успешно подключились к PostgreSQL." << std::endl;
    return conn;
}

// Функция для выполнения SQL-запроса
void executeSQL(PGconn* conn, const std::string& sql) {
    PGresult* res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "SQL ошибка: " << PQerrorMessage(conn) << std::endl;
    }
    PQclear(res);
}

// Чтение CSV файла sales.csv
std::vector<RawSale> readSalesCSV(const std::string& filename) {
    std::vector<RawSale> sales;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Невозможно открыть файл: " << filename << std::endl;
        return sales;
    }

    std::string line;
    std::getline(file, line); // Пропуск заголовка
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        RawSale sale;

        std::getline(ss, token, ',');
        sale.sale_id = std::stoi(token);
        std::getline(ss, sale.sale_date_str, ',');
        std::getline(ss, token, ',');
        sale.product_id = std::stoi(token);
        std::getline(ss, token, ',');
        sale.customer_id = std::stoi(token);
        std::getline(ss, token, ',');
        sale.quantity = std::stoi(token);
        std::getline(ss, token, ',');
        sale.amount = std::stod(token);

        sales.push_back(sale);
    }
    file.close();
    std::cout << "Прочитано " << sales.size() << " продаж из CSV." << std::endl;
    return sales;
}

// Трансформация данных: проверка даты и агрегация
void transformAndLoadSales(PGconn* conn, const std::vector<RawSale>& rawSales) {

    // Шаг 1: Заполнение time_dim на основе уникальных дат
    std::vector<std::string> uniqueDates;
    for (const auto& sale : rawSales) {
        if (std::find(uniqueDates.begin(), uniqueDates.end(), sale.sale_date_str) == uniqueDates.end()) {
            uniqueDates.push_back(sale.sale_date_str);
        }
    }

    // Заполняем time_dim (если еще нет)
    for (const auto& dateStr : uniqueDates) {
        // Парсим дату
        int year, month, day;
        sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);

        // Определяем квартал
        int quarter = (month - 1) / 3 + 1;

        const char* days[] = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
        // Простой хэш для примера
        int dayOfWeekIndex = (year + month + day) % 7;
        std::string dayOfWeek = days[dayOfWeekIndex];

        // Генерируем time_id (например, YYYYMMDD)
        int time_id = year * 10000 + month * 100 + day;

        // Проверяем, существует ли уже запись
        std::string checkSql = "SELECT time_id FROM time_dim WHERE time_id = " + std::to_string(time_id) + ";";
        PGresult* res = PQexec(conn, checkSql.c_str());
        if (PQntuples(res) == 0) {
            // Вставляем новую запись
            std::string insertTimeSql = "INSERT INTO time_dim (time_id, year, quarter, month, day, day_of_week) "
                                        "VALUES (" + std::to_string(time_id) + ", " +
                                        std::to_string(year) + ", " +
                                        std::to_string(quarter) + ", " +
                                        std::to_string(month) + ", " +
                                        std::to_string(day) + ", '" +
                                        dayOfWeek + "');";
            executeSQL(conn, insertTimeSql);
            std::cout << "Вставлено time_id: " << time_id << std::endl;
        }
        PQclear(res);
    }

    // Шаг 2: Загрузка sales_fact с трансформацией даты и связью с time_dim
    for (const auto& sale : rawSales) {
        // Преобразуем строку даты в формат DATE PostgreSQL (уже в правильном формате YYYY-MM-DD)
        // Но нужно также получить time_id
        int year, month, day;
        sscanf(sale.sale_date_str.c_str(), "%d-%d-%d", &year, &month, &day);
        int time_id = year * 10000 + month * 100 + day;

        // SQL для вставки
        std::string sql = "INSERT INTO sales_fact (sale_id, sale_date, product_id, customer_id, time_id, quantity, amount) "
                          "VALUES (" + std::to_string(sale.sale_id) + ", '" +
                          sale.sale_date_str + "', " +
                          std::to_string(sale.product_id) + ", " +
                          std::to_string(sale.customer_id) + ", " +
                          std::to_string(time_id) + ", " +
                          std::to_string(sale.quantity) + ", " +
                          std::to_string(sale.amount) + ");";

        executeSQL(conn, sql);
    }

    std::cout << "Загружено " << rawSales.size() << " продаж в sales_fact." << std::endl;
}

// Загрузка измерений из CSV
void loadDimension(PGconn* conn, const std::string& filename, const std::string& tableName) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Невозможно открыть файл: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Пропуск заголовка

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;

        // Парсинг CSV строки
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 3) continue;

        // Экранирование кавычек
        for (auto& t : tokens) {
            size_t pos = 0;
            while ((pos = t.find("'", pos)) != std::string::npos) {
                t.replace(pos, 1, "''");
                pos += 2;
            }
        }

        // Формирование SQL в зависимости от таблицы
        std::string sql;
        if (tableName == "customers_dim") {
            sql = "INSERT INTO customers_dim VALUES (" +
                  tokens[0] + ", '" +
                  tokens[1] + "', '" +
                  tokens[2] + "');";
        }
        else if (tableName == "products_dim") {
            sql = "INSERT INTO products_dim VALUES (" +
                  tokens[0] + ", '" +
                  tokens[1] + "', '" +
                  tokens[2] + "', " +
                  tokens[3] + ");";
        }

        executeSQL(conn, sql);
    }
    file.close();
    std::cout << "Загружено: " << tableName << std::endl;
}
// Агрегация данных: пример - сумма продаж по месяцам
void aggregateSales(PGconn* conn) {
    std::string sql =
        "SELECT t.year, t.month, SUM(s.amount) as total_amount, SUM(s.quantity) as total_quantity "
        "FROM sales_fact s "
        "JOIN time_dim t ON s.time_id = t.time_id "
        "GROUP BY t.year, t.month "
        "ORDER BY t.year, t.month;";

    PGresult* res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        std::cout << "\nАгрегируем продажи по месяцам:" << std::endl;
        std::cout << "Год | Месяц | Общая сумма | Общее количество" << std::endl;
        for (int i = 0; i < rows; i++) {
            std::cout << PQgetvalue(res, i, 0) << " | "
                      << PQgetvalue(res, i, 1) << " | "
                      << PQgetvalue(res, i, 2) << " | "
                      << PQgetvalue(res, i, 3) << std::endl;
        }
    } else {
        std::cerr << "Ошибка агрегации: " << PQerrorMessage(conn) << std::endl;
    }
    PQclear(res);
}

void executeAnalyticsQueries(PGconn* conn) {
    std::vector<std::pair<std::string, std::string>> queries = {
        {"1. Объем продаж по категориям товаров",
         "SELECT p.category, SUM(s.amount), SUM(s.quantity), COUNT(s.sale_id) "
         "FROM sales_fact s JOIN products_dim p ON s.product_id = p.product_id "
         "GROUP BY p.category ORDER BY SUM(s.amount) DESC"},

        {"2. Количество покупок по регионам",
         "SELECT c.region, COUNT(s.sale_id), COUNT(DISTINCT c.customer_id), SUM(s.amount) "
         "FROM sales_fact s JOIN customers_dim c ON s.customer_id = c.customer_id "
         "GROUP BY c.region ORDER BY COUNT(s.sale_id) DESC"},

        {"3. Средний чек за месяц",
         "SELECT t.year, t.month, COUNT(s.sale_id), SUM(s.amount), ROUND(AVG(s.amount), 2) "
         "FROM sales_fact s JOIN time_dim t ON s.time_id = t.time_id "
         "GROUP BY t.year, t.month ORDER BY t.year, t.month"}
    };

    for (const auto& [title, sql] : queries) {
        std::cout << "\n=== " << title << " ===" << std::endl;

        PGresult* res = PQexec(conn, sql.c_str());
        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            int rows = PQntuples(res);
            int cols = PQnfields(res);

            // Вывод заголовков
            for (int i = 0; i < cols; i++) {
                std::cout << PQfname(res, i) << "\t";
            }
            std::cout << std::endl;

            // Вывод данных
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    std::cout << PQgetvalue(res, i, j) << "\t";
                }
                std::cout << std::endl;
            }
        } else {
            std::cerr << "Ошибка запроса: " << PQerrorMessage(conn) << std::endl;
        }
        PQclear(res);
    }
}

void createTables(PGconn* conn) {
    const char* createTablesSQL =
        "CREATE TABLE IF NOT EXISTS customers_dim ("
        "    customer_id INTEGER PRIMARY KEY,"
        "    customer_name VARCHAR(255) NOT NULL,"
        "    region VARCHAR(100)"
        ");"
        "CREATE TABLE IF NOT EXISTS products_dim ("
        "    product_id INTEGER PRIMARY KEY,"
        "    product_name VARCHAR(255) NOT NULL,"
        "    category VARCHAR(100),"
        "    price NUMERIC(10, 2)"
        ");"
        "CREATE TABLE IF NOT EXISTS time_dim ("
        "    time_id INTEGER PRIMARY KEY,"
        "    year INTEGER NOT NULL,"
        "    quarter INTEGER CHECK (quarter BETWEEN 1 AND 4),"
        "    month INTEGER CHECK (month BETWEEN 1 AND 12),"
        "    day INTEGER CHECK (day BETWEEN 1 AND 31),"
        "    day_of_week VARCHAR(20)"
        ");"
        "CREATE TABLE IF NOT EXISTS sales_fact ("
        "    sale_id SERIAL PRIMARY KEY,"
        "    sale_date DATE NOT NULL,"
        "    product_id INTEGER REFERENCES products_dim(product_id),"
        "    customer_id INTEGER REFERENCES customers_dim(customer_id),"
        "    time_id INTEGER REFERENCES time_dim(time_id),"
        "    quantity INTEGER CHECK (quantity > 0),"
        "    amount NUMERIC(12, 2) NOT NULL"
        ");";

    PGresult* res = PQexec(conn, createTablesSQL);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Ошибка создания таблиц: " << PQerrorMessage(conn) << std::endl;
    } else {
        std::cout << "Таблицы успешно созданы." << std::endl;
    }
    PQclear(res);
}

void createOptimizationIndexes(PGconn* conn) {
    std::vector<std::string> indexQueries = {
        // Основные индексы
        "CREATE INDEX IF NOT EXISTS idx_sales_fact_product_id ON sales_fact(product_id)",
        "CREATE INDEX IF NOT EXISTS idx_sales_fact_customer_id ON sales_fact(customer_id)",
        "CREATE INDEX IF NOT EXISTS idx_sales_fact_time_id ON sales_fact(time_id)",
        "CREATE INDEX IF NOT EXISTS idx_sales_fact_sale_date ON sales_fact(sale_date)",

        // Оптимизация для аналитических запросов
        "CREATE INDEX IF NOT EXISTS idx_products_category ON products_dim(category, product_id)",
        "CREATE INDEX IF NOT EXISTS idx_customers_region ON customers_dim(region, customer_id)",
        "CREATE INDEX IF NOT EXISTS idx_time_dim_year_month ON time_dim(year, month, time_id)",

        // Составные индексы для частых JOIN
        "CREATE INDEX IF NOT EXISTS idx_sales_products_join ON sales_fact(product_id, amount, quantity)",
        "CREATE INDEX IF NOT EXISTS idx_sales_customers_join ON sales_fact(customer_id, sale_id)",
        "CREATE INDEX IF NOT EXISTS idx_sales_time_join ON sales_fact(time_id, amount)"
    };

    std::cout << "Создаем индекс для оптимизации запросов..." << std::endl;
    for (const auto& query : indexQueries) {
        PGresult* res = PQexec(conn, query.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Ошибка создания индексов: " << PQerrorMessage(conn) << std::endl;
        } else {
            std::cout << "Индексы успешно созданы: " << query.substr(0, 50) << "..." << std::endl;
        }
        PQclear(res);
    }
}

int main() {
    // Параметры подключения
    std::string conninfo = "host=localhost port=5432 dbname=DWH user=postgres password=288288";

    // Подключение к БД
    PGconn* conn = connectDB(conninfo);
    if (!conn) return 1;

    createTables(conn);
    createOptimizationIndexes(conn);

    // 1. Загрузка измерений
    loadDimension(conn, "../customers.csv", "customers_dim");
    loadDimension(conn, "../products.csv", "products_dim");

    // 2. Чтение и трансформация фактов
    std::vector<RawSale> rawSales = readSalesCSV("../sales.csv");

    // 3. Трансформация и загрузка фактов
    transformAndLoadSales(conn, rawSales);

    // 4. Агрегация данных
    aggregateSales(conn);

    executeAnalyticsQueries(conn);

    // Закрытие подключения
    PQfinish(conn);

    return 0;
}