# Система интернет-магазина (ExamTaskShop)

## 1. Описание задачи

### 1.1 Цель работы
Разработка полнофункциональной системы интернет-магазина на языке C++ с использованием объектно-ориентированного программирования, базы данных PostgreSQL и современных практик разработки. Система демонстрирует применение принципов ООП, работу с базой данных через хранимые процедуры и триггеры, систему аудита и генерацию отчетов.

### 1.2 Краткое описание системы
Реализована система интернет-магазина с тремя типами пользователей (администратор, менеджер, покупатель), полноценным управлением заказами, товарами, системой истории изменений и аудита действий пользователей. Система поддерживает:
- Управление товарами (добавление, обновление, удаление)
- Создание и обработку заказов
- Систему ролей и прав доступа
- Автоматическое логирование всех операций
- Генерацию отчетов в формате CSV
- Транзакционную обработку с откатом при ошибках
- Систему лояльности покупателей

### 1.3 Используемые технологии
- **Язык программирования**: C++20
- **База данных**: PostgreSQL 13+
- **Библиотека для работы с БД**: libpqxx (C++ PostgreSQL connector)
- **Система сборки**: CMake 3.16+
- **Стандарт**: ISO C++20 с использованием современных возможностей STL

---

## 2. Архитектура проекта

### 2.1 Описание классов и их взаимосвязей

#### Диаграмма классов
```
                    ┌──────────────────┐
                    │   User (ABC)     │
                    │                  │
                    │ - userId         │
                    │ - name           │
                    │ - email          │
                    │ - role           │
                    │ - loyaltyLevel   │
                    └────────┬─────────┘
                             │
             ┌───────────────┼───────────────┐
             │               │               │
       ┌─────▼─────┐  ┌─────▼─────┐  ┌─────▼─────┐
       │   Admin   │  │  Manager  │  │  Customer │
       └───────────┘  └───────────┘  └───────────┘
             │               │               │
             └───────────────┼───────────────┘
                             │
                    ┌────────▼─────────┐
                    │ DatabaseConnection│
                    │   (Template)      │
                    └──────────────────┘
                             │
             ┌───────────────┼───────────────┐
             │               │               │
       ┌─────▼─────┐  ┌─────▼─────┐  ┌─────▼─────┐
       │   Order   │  │OrderItem  │  │  Payment  │
       └───────────┘  └───────────┘  └───────────┘

       ┌──────────────────┐  ┌──────────────────────┐
       │ ReportManager    │  │ HistoryAuditManager  │
       └──────────────────┘  └──────────────────────┘
```

#### Основные классы

**1. DatabaseConnection** (DatabaseConnection.hpp:9-284)
- Шаблонный класс для работы с PostgreSQL
- Управление подключениями, транзакциями и запросами
- Параметризованные запросы для защиты от SQL-инъекций
- Автоматическое управление ресурсами через RAII

**2. User** (User.cpp:8-33)
- Абстрактный базовый класс для всех типов пользователей
- Определяет интерфейс: `createOrder()`, `cancelOrder()`, `viewOrderStatus()`
- Хранит общие атрибуты: userId, name, email, role, loyaltyLevel

**3. Admin** (Admin.cpp:9-272)
- Наследуется от User
- Полный доступ к системе: управление товарами, заказами, пользователями
- Методы: `addProduct()`, `updateProduct()`, `deleteProduct()`, `updateOrderStatus()`

**4. Manager** (Manager.cpp:8-240)
- Наследуется от User
- Управление заказами и складом
- Методы: `approveOrder()`, `updateStock()`
- Ограниченные права по сравнению с администратором

**5. Customer** (Customer.cpp:10-327)
- Наследуется от User
- Создание и управление своими заказами
- Методы: `addToOrder()`, `removeFromOrder()`, `makePayment()`
- Система лояльности с автоматической скидкой

**6. Order** (Order.cpp)
- Класс для управления заказами
- Композиция с OrderItem
- Методы расчета общей стоимости и обработки оплаты

**7. ReportManager** (ReportSystem.cpp:16-189)
- Генерация CSV-отчетов
- Проверка прав доступа
- Использование STL алгоритмов для обработки данных

**8. HistoryAuditManager** (HistoryAuditManager.cpp:14-195)
- Управление доступом к истории и аудиту
- Ролевая модель доступа через лямбда-функции
- Фильтрация данных по правам пользователя

### 2.2 Применение принципов ООП

#### Наследование
- Трехуровневая иерархия: `User` → `Admin/Manager/Customer`
- Виртуальные функции для полиморфизма
- Пример (User.cpp:18-24):
```cpp
virtual std::unique_ptr<Order> createOrder() = 0;
virtual bool cancelOrder(int orderId) = 0;
virtual std::string viewOrderStatus(int orderId) = 0;
```

#### Полиморфизм
- Различная реализация методов в производных классах
- `cancelOrder()` имеет разные права для Admin, Manager, Customer
- Пример: Admin может отменить любой заказ, Customer - только свой

#### Инкапсуляция
- Приватные поля с публичными геттерами/сеттерами
- Защита данных от несанкционированного доступа
- DatabaseConnection скрывает детали работы с libpqxx

#### Композиция
- `Order` содержит вектор `std::vector<std::shared_ptr<OrderItem>>`
- Классы пользователей содержат `std::shared_ptr<DatabaseConnection>`
- Пример (Customer.cpp:13):
```cpp
std::shared_ptr<DatabaseConnection> db;
std::unique_ptr<Order> currentCart;
```

#### Агрегация
- ReportManager и HistoryAuditManager агрегируют DatabaseConnection
- Связь "использует" между классами

### 2.3 Использование шаблонного класса DatabaseConnection<T>

DatabaseConnection реализован как обобщенный класс (DatabaseConnection.hpp:9):
```cpp
class DatabaseConnection {
private:
    std::unique_ptr<pqxx::connection> connection;
    std::unique_ptr<pqxx::work> currentTransaction;

public:
    // Шаблонный метод для параметризованных запросов
    template<typename... Args>
    std::vector<std::vector<std::string>> executeQuery(
        const std::string& query, Args&&... args);

    // Шаблонный метод для выполнения в транзакции
    template<typename Func>
    auto executeInTransaction(Func&& func) -> decltype(...);
}
```

**Преимущества шаблонного подхода:**
- Типобезопасность при передаче параметров
- Автоматическое определение типов через variadic templates
- Использование perfect forwarding для эффективности

### 2.4 Паттерн Стратегия для оплаты

Система реализует паттерн проектирования **Strategy** для обработки различных способов оплаты.

#### Диаграмма классов паттерна Стратегия
```
            ┌─────────────────────────┐
            │   PaymentStrategy       │
            │   <<interface>>         │
            ├─────────────────────────┤
            │ +processPayment()       │
            │ +getPaymentMethodName() │
            └───────────┬─────────────┘
                        │
          ┌─────────────┼─────────────┐
          │             │             │
    ┌─────▼─────┐ ┌────▼────┐ ┌──────▼──────┐
    │ CreditCard│ │ EWallet │ │ SBPPayment  │
    │ Payment   │ │ Payment │ │             │
    └───────────┘ └─────────┘ └─────────────┘
          │             │             │
          └─────────────┼─────────────┘
                        │
               ┌────────▼────────┐
               │ PaymentContext  │
               └─────────────────┘
```

#### Реализация (PaymentStrategy.hpp)

**1. Абстрактная стратегия:**
```cpp
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool processPayment(double amount, const std::string& orderInfo) = 0;
    virtual std::string getPaymentMethodName() const = 0;
};
```

**2. Конкретные стратегии:**

**CreditCardPayment** - Оплата банковской картой
```cpp
class CreditCardPayment : public PaymentStrategy {
private:
    std::string cardNumber;
    std::string cardHolder;
    std::string cvv;
    std::string expiryDate;

public:
    bool processPayment(double amount, const std::string& orderInfo) override {
        // Валидация карты (номер 16 цифр, CVV 3 цифры)
        // Обработка платежа
        return true;
    }

    std::string getPaymentMethodName() const override {
        return "credit_card";
    }
};
```

**EWalletPayment** - Электронный кошелек
```cpp
class EWalletPayment : public PaymentStrategy {
private:
    std::string walletId;
    std::string walletType; // yandex_money, qiwi, webmoney

public:
    bool processPayment(double amount, const std::string& orderInfo) override;
    std::string getPaymentMethodName() const override { return "e_wallet"; }
};
```

**SBPPayment** - Система Быстрых Платежей
```cpp
class SBPPayment : public PaymentStrategy {
private:
    std::string phoneNumber;
    std::string bankName;

public:
    bool processPayment(double amount, const std::string& orderInfo) override;
    std::string getPaymentMethodName() const override { return "sbp"; }
};
```

**3. Контекст использования стратегии:**
```cpp
class PaymentContext {
private:
    std::unique_ptr<PaymentStrategy> strategy;

public:
    void setPaymentStrategy(std::unique_ptr<PaymentStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    bool executePayment(double amount, const std::string& orderInfo) {
        if (!strategy) return false;
        return strategy->processPayment(amount, orderInfo);
    }
};
```

#### Преимущества паттерна Стратегия:
- **Гибкость**: Легко добавить новые способы оплаты без изменения существующего кода
- **Инкапсуляция**: Каждый способ оплаты изолирован в отдельном классе
- **Взаимозаменяемость**: Способы оплаты можно менять во время выполнения
- **Принцип Open/Closed**: Открыт для расширения, закрыт для модификации

---

## 2.5 Интерактивное меню покупателя

Система включает полнофункциональное интерактивное меню для покупателей с поддержкой всех операций.

### Функциональность меню (CustomerMenu.cpp)

#### Главное меню:
```
╔════════════════════════════════════════════════╗
║       ИНТЕРНЕТ-МАГАЗИН - МЕНЮ ПОКУПАТЕЛЯ       ║
╚════════════════════════════════════════════════╝

1. Создать новый заказ
2. Добавить товар в заказ
3. Удалить товар из заказа
4. Просмотр моих заказов
5. Просмотр статуса заказа
6. Оплатить заказ
7. Оформить возврат заказа
8. Просмотр истории статусов заказа
9. Выход
```

#### Реализованные функции:

**1. Создание нового заказа** (CustomerMenu.cpp:132-141)
- Создание корзины для текущей сессии
- Автоматическая проверка отсутствия активных заказов

**2. Добавление товара** (CustomerMenu.cpp:143-192)
- Отображение каталога доступных товаров
- Проверка наличия на складе
- Динамический расчет стоимости корзины

**3. Удаление товара** (CustomerMenu.cpp:194-210)
- Удаление позиций из текущей корзины
- Пересчет общей суммы

**4. Просмотр заказов** (CustomerMenu.cpp:212-246)
- Таблица всех заказов пользователя
- Информация: номер, статус, сумма, дата, количество позиций
- Доступ только к собственным заказам

**5. Статус заказа** (CustomerMenu.cpp:248-276)
- Детальная информация о конкретном заказе
- Проверка прав доступа

**6. Оплата заказа** (CustomerMenu.cpp:278-336)
- Выбор способа оплаты (паттерн Стратегия)
- Интерактивный ввод платежных данных
- Валидация данных
- Автоматическая скидка 10% для PREMIUM пользователей
- Запись в систему аудита

**7. Оформление возврата** (CustomerMenu.cpp:338-413)
- Проверка возможности возврата (функция `can_return_order`)
- Валидация:
  - Статус заказа = 'completed'
  - Не более 30 дней с даты заказа
- Ввод причины возврата
- Подтверждение операции
- Вызов процедуры `process_order_return`
- Автоматический возврат товаров на склад

**8. История статусов** (CustomerMenu.cpp:415-458)
- Отображение всех изменений статуса заказа
- Информация о пользователе, изменившем статус
- Дата и время каждого изменения

#### Ограничения безопасности:
- Покупатель видит только свои заказы
- Проверка прав на каждую операцию
- SQL-инъекции предотвращены параметризованными запросами
- Все действия логируются в систему аудита

### Запуск интерактивного меню:

```bash
# Сборка проекта
cd build
cmake ..
cmake --build .

# Запуск интерактивного меню
./CustomerMenuApp
```

**Пример сеанса работы:**
```
╔══════════════════════════════════════════════════════════╗
║          ДОБРО ПОЖАЛОВАТЬ В ИНТЕРНЕТ-МАГАЗИН!            ║
╚══════════════════════════════════════════════════════════╝

Подключение к базе данных...
✓ Подключение установлено

╔═══════════════════════════════════════╗
║          ВХОД В СИСТЕМУ               ║
╚═══════════════════════════════════════╝

Доступные покупатели:
┌────┬───────────────────────┬──────────────────────┬─────────────┐
│ ID │ Имя                   │ Email                │ Статус      │
├────┼───────────────────────┼──────────────────────┼─────────────┤
│ 1  │ Иван Покупатель       │ ivan@example.com     │ ★ PREMIUM   │
│ 5  │ Тестовый Покупатель   │ test@example.com     │ Обычный     │
└────┴───────────────────────┴──────────────────────┴─────────────┘

Введите ваш ID для входа: 1
✓ Добро пожаловать, Иван Покупатель!
```

---

## 3. Работа с базой данных

### 3.1 Описание структуры базы данных

База данных `online_store` содержит полную информацию о товарах, заказах, пользователях и истории операций.

### 3.2 Список таблиц

#### 1. **users** (database_setup.cpp:12-21)
Хранит информацию о пользователях системы.
```sql
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    role VARCHAR(20) CHECK (role IN ('admin', 'manager', 'customer')) NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    loyalty_level INTEGER CHECK (loyalty_level IN (0, 1)) DEFAULT 0
)
```

#### 2. **products** (database_setup.cpp:22-29)
Каталог товаров магазина.
```sql
CREATE TABLE products (
    product_id SERIAL PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    price DECIMAL(10, 2) CHECK (price > 0) NOT NULL,
    stock_quantity INTEGER CHECK (stock_quantity >= 0) DEFAULT 0 NOT NULL
)
```

#### 3. **orders** (database_setup.cpp:30-42)
Информация о заказах.
```sql
CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    user_id INTEGER NOT NULL,
    status VARCHAR(20) CHECK (status IN ('pending', 'completed', 'canceled', 'returned')) DEFAULT 'pending',
    total_price DECIMAL(10, 2) CHECK (total_price >= 0) DEFAULT 0,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
)
```

#### 4. **order_items** (database_setup.cpp:43-59)
Позиции в заказах (связь многие-ко-многим между orders и products).
```sql
CREATE TABLE order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    quantity INTEGER CHECK (quantity > 0) NOT NULL,
    price DECIMAL(10, 2) CHECK (price >= 0) NOT NULL,
    FOREIGN KEY (order_id) REFERENCES orders(order_id) ON DELETE CASCADE,
    FOREIGN KEY (product_id) REFERENCES products(product_id) ON DELETE RESTRICT
)
```

#### 5. **order_status_history** (database_setup.cpp:60-78)
История изменений статусов заказов для аудита.
```sql
CREATE TABLE order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id INTEGER NOT NULL,
    old_status VARCHAR(20),
    new_status VARCHAR(20) NOT NULL,
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by INTEGER,
    reason TEXT,
    FOREIGN KEY (order_id) REFERENCES orders(order_id) ON DELETE CASCADE,
    FOREIGN KEY (changed_by) REFERENCES users(user_id) ON DELETE SET NULL
)
```

#### 6. **audit_log** (database_setup.cpp:79-92)
Журнал всех операций в системе.
```sql
CREATE TABLE audit_log (
    log_id SERIAL PRIMARY KEY,
    entity_type VARCHAR(50) CHECK (entity_type IN ('order', 'product', 'user', 'orders', 'products', 'users', 'order_items')) NOT NULL,
    entity_id INTEGER NOT NULL,
    operation VARCHAR(20) CHECK (operation IN ('insert', 'update', 'delete')) NOT NULL,
    performed_by INTEGER,
    performed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (performed_by) REFERENCES users(user_id) ON DELETE SET NULL
)
```

### 3.3 Хранимые процедуры, функции и триггеры

#### Хранимые функции

**1. create_order** (stored_procedure.cpp:11-85)
Создает новый заказ с проверкой наличия товаров.
```sql
CREATE OR REPLACE FUNCTION create_order(
    p_user_id INTEGER,
    p_items JSONB,
    p_payment_method VARCHAR DEFAULT 'credit_card'
) RETURNS INTEGER
```
- Проверяет существование пользователя
- Проверяет наличие товаров на складе
- Создает заказ и уменьшает количество товаров
- Записывает в историю статусов

**2. get_order_status** (functions.cpp:64-82)
Возвращает текущий статус заказа.

**3. get_order_status_history** (functions.cpp:13-56)
Возвращает полную историю изменений статуса заказа с информацией о пользователе, изменившем статус.

**4. get_user_order_count** (functions.cpp:86-117)
Возвращает статистику заказов по пользователям (всего, pending, completed, canceled, returned).

**5. get_total_spent_by_user** (functions.cpp:121-154)
Возвращает статистику трат пользователя (общая сумма, средний чек, даты первого и последнего заказа).

**6. can_return_order** (functions.cpp:158-203)
Проверяет возможность возврата заказа (статус, срок 30 дней).

**7. get_audit_log_by_user** (audit_system.cpp:80-118)
Возвращает аудит-лог действий конкретного пользователя.

#### Хранимые процедуры

**1. update_order_status** (stored_procedure.cpp:89-140)
Обновляет статус заказа с проверкой допустимости перехода.
```sql
CREATE OR REPLACE PROCEDURE update_order_status(
    p_order_id INTEGER,
    p_new_status VARCHAR,
    p_changed_by INTEGER DEFAULT NULL
)
```
- Валидация переходов между статусами
- Автоматический возврат товаров при отмене
- Запись в историю

**2. process_order_return** (stored_procedure.cpp:144-192)
Обрабатывает возврат заказа.
- Проверка статуса (только 'completed')
- Проверка срока (не более 30 дней)
- Возврат товаров на склад

#### Триггеры

**1. trigger_log_status_change** (triggers.cpp:56-80)
Автоматически логирует изменения статуса заказа в таблицу `order_status_history`.
```sql
CREATE TRIGGER trigger_log_status_change
AFTER UPDATE OF status ON orders
FOR EACH ROW
EXECUTE FUNCTION log_order_status_change();
```

**2. trg_audit_* (products, orders, users, order_items)** (audit_system.cpp:65-75)
Автоматически записывает все операции INSERT, UPDATE, DELETE в таблицу `audit_log`.
```sql
CREATE TRIGGER trg_audit_[table_name]
AFTER INSERT OR UPDATE OR DELETE ON [table_name]
FOR EACH ROW
EXECUTE FUNCTION audit_trigger_function();
```

### 3.4 Механизм транзакций и отката при ошибках

Система использует транзакции для обеспечения целостности данных.

**Пример 1: Утверждение заказа менеджером** (Manager.cpp:134-183)
```cpp
// Начинаем транзакцию
db->beginTransaction();
try {
    // Обновляем статус заказа
    db->getCurrentTransaction().exec(...);

    // Уменьшаем количество товаров на складе
    for (const auto& item : items) {
        db->getCurrentTransaction().exec(...);
    }

    // Логируем изменения
    db->getCurrentTransaction().exec(...);

    db->commitTransaction(); // Фиксируем транзакцию
} catch (const std::exception& e) {
    db->rollbackTransaction(); // Откат при ошибке
    throw;
}
```

**Пример 2: Оплата заказа покупателем** (Customer.cpp:239-320)
```cpp
db->beginTransaction();
try {
    // Создаем запись заказа
    auto orderResult = db->getCurrentTransaction().exec(...);

    // Добавляем товары и обновляем склад
    for (const auto& item : items) {
        db->getCurrentTransaction().exec(...);
    }

    db->commitTransaction();
} catch (const std::exception& e) {
    db->rollbackTransaction(); // Автоматический откат
    throw;
}
```

**Преимущества:**
- Атомарность операций (все или ничего)
- Защита от частичных обновлений
- Автоматический откат при ошибках

---

## 4. Умные указатели и STL

### 4.1 Использование std::unique_ptr

**Назначение:** Единоличное владение ресурсом, автоматическое освобождение памяти.

**Примеры использования:**

1. **DatabaseConnection** (DatabaseConnection.hpp:11-12)
```cpp
std::unique_ptr<pqxx::connection> connection;
std::unique_ptr<pqxx::work> currentTransaction;
```
Обеспечивает единоличное владение подключением к БД и транзакцией.

2. **Customer** (Customer.cpp:13)
```cpp
std::unique_ptr<Order> currentCart;
```
Корзина принадлежит только одному покупателю.

3. **Возврат из функций** (User.cpp:22)
```cpp
virtual std::unique_ptr<Order> createOrder() = 0;
```
Передача владения созданным заказом вызывающему коду.

### 4.2 Использование std::shared_ptr

**Назначение:** Совместное владение ресурсом несколькими объектами.

**Примеры использования:**

1. **Разделяемое подключение к БД** (Admin.cpp:11, Manager.cpp:10, Customer.cpp:12)
```cpp
std::shared_ptr<DatabaseConnection> db;
```
Несколько пользователей могут использовать одно подключение.

2. **Создание демонстрационного подключения** (main.cpp:92-93)
```cpp
auto demoDb = std::make_shared<DatabaseConnection>(...);
```

3. **Order с OrderItem** (User.cpp:15)
```cpp
std::vector<std::shared_ptr<Order>> orders;
```
Заказы могут быть доступны из разных частей системы.

### 4.3 Примеры использования STL алгоритмов

#### std::find_if
Поиск элемента по условию.

**Пример:** Фильтрация истории заказов (HistoryAuditManager.cpp:103-109)
```cpp
std::copy_if(history.begin(), history.end(),
    std::back_inserter(filteredHistory),
    [userId](const std::vector<std::string>& row) {
        int changedBy = std::stoi(row[5]);
        return changedBy == userId || row[5].empty();
    });
```

#### std::copy_if
Копирование элементов, удовлетворяющих условию.

**Пример:** Фильтрация истории для покупателя (HistoryAuditManager.cpp:104-109)
```cpp
if (userRole == "customer") {
    std::copy_if(history.begin(), history.end(),
        std::back_inserter(filteredHistory),
        [userId](const std::vector<std::string>& row) {
            int changedBy = std::stoi(row[5]);
            return changedBy == userId || row[5].empty();
        });
}
```
Покупатель видит только изменения, сделанные им самим.

#### std::accumulate
Агрегация данных (суммирование, подсчет).

**Пример 1:** Подсчет статистики заказов (HistoryAuditManager.cpp:166-172)
```cpp
auto stats = std::accumulate(orders.begin(), orders.end(),
    std::map<std::string, int>(),
    [](std::map<std::string, int> acc, const std::vector<std::string>& row) {
        acc["total_orders"]++;
        acc["total_amount"] += static_cast<int>(std::stod(row[3]));
        return acc;
    });
```

**Пример 2:** Подсчет общей суммы отчета (ReportSystem.cpp:80-98)
```cpp
double totalOrderAmount = 0.0;
std::for_each(reportData.begin(), reportData.end(),
    [&totalOrderAmount](const std::vector<std::string>& row) {
        if (!row[3].empty()) {
            totalOrderAmount += std::stod(row[3]);
        }
    });
```

#### Лямбда-выражения

**Пример 1: Проверка прав доступа** (HistoryAuditManager.cpp:26-68)
```cpp
PermissionChecker getPermissionChecker() {
    return [this](int userId, const std::string& userRole,
                  const std::string& resourceType, int resourceId) -> bool {
        if (userRole == "admin") return true;
        else if (userRole == "manager") {
            return resourceType == "order" || resourceType == "order_history";
        }
        else if (userRole == "customer") {
            auto result = db->executeQuery(...);
            return !result.empty() && result[0][0] == "t";
        }
        return false;
    };
}
```

**Пример 2: Обработка данных в отчетах** (ReportSystem.cpp:81-98)
```cpp
auto processReportRow = [&](const std::vector<std::string>& row) {
    for (size_t i = 0; i < row.size(); ++i) {
        std::string escapedValue = escapeCSV(row[i]);
        csvFile << escapedValue;
        if (i < row.size() - 1) csvFile << ",";
    }
    csvFile << "\n";
    totalRecords++;
    if (!row[3].empty()) {
        totalOrderAmount += std::stod(row[3]);
    }
};
```

**Пример 3: Форматирование вывода** (HistoryAuditManager.cpp:116-120)
```cpp
auto formatHistoryEntry = [](const std::vector<std::string>& entry) {
    return "ID: " + entry[0] + " | Заказ: " + entry[1] +
           " | Статус: " + entry[2] + " → " + entry[3] +
           " | Дата: " + entry[4] + " | Изменил: " + entry[6];
};
```

**Пример 4: std::for_each с лямбдой** (database_setup.cpp:96-107)
```cpp
std::for_each(tableQueries.begin(), tableQueries.end(),
    [&db](const auto& tableInfo) {
        try {
            db.executeNonQuery(tableInfo.second);
            std::cout << "Таблица '" << tableInfo.first << "' создана успешно" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
            throw;
        }
    });
```

---

## 5. Логика ролей и прав доступа

### 5.1 Возможности ролей

#### Администратор (Admin)
**Полный доступ к системе:**
- ✓ Управление товарами: добавление, обновление, удаление
- ✓ Просмотр всех заказов
- ✓ Изменение статусов любых заказов
- ✓ Доступ ко всем записям аудита
- ✓ Генерация всех типов отчетов
- ✓ Просмотр истории изменений всех заказов

**Методы:** `addProduct()`, `updateProduct()`, `deleteProduct()`, `viewAllOrders()`, `updateOrderStatus()`

#### Менеджер (Manager)
**Управление заказами и складом:**
- ✓ Утверждение заказов (`approveOrder()`)
- ✓ Обновление количества товаров на складе (`updateStock()`)
- ✓ Отмена только pending заказов
- ✓ Просмотр истории заказов
- ✗ Нет доступа к аудит-логу
- ✗ Не может генерировать отчеты

**Ограничения:**
- Может отменять только заказы в статусе "pending" (Manager.cpp:39-48)
```cpp
if (currentStatus != "pending") {
    std::cerr << "Manager can only cancel pending orders" << std::endl;
    return false;
}
```

#### Покупатель (Customer)
**Управление своими заказами:**
- ✓ Создание заказов (`createOrder()`)
- ✓ Добавление товаров в корзину (`addToOrder()`)
- ✓ Оформление и оплата заказов (`makePayment()`)
- ✓ Отмена своих pending заказов
- ✓ Просмотр статуса своих заказов
- ✓ Система лояльности (скидка 10% после 5 заказов)
- ✗ Нет доступа к заказам других пользователей
- ✗ Нет доступа к аудит-логу

### 5.2 Реализация проверки прав доступа

**Проверка через PermissionChecker** (HistoryAuditManager.cpp:26-68)

```cpp
PermissionChecker getPermissionChecker() {
    return [this](int userId, const std::string& userRole,
                  const std::string& resourceType, int resourceId) -> bool {

        // Администратор имеет доступ ко всему
        if (userRole == "admin") return true;

        // Менеджер имеет ограниченный доступ
        else if (userRole == "manager") {
            if (resourceType == "order" || resourceType == "order_history") {
                return true;
            }
            return false;
        }

        // Покупатель может видеть только свои ресурсы
        else if (userRole == "customer") {
            if (resourceType == "order") {
                auto result = db->executeQuery(
                    "SELECT EXISTS(SELECT 1 FROM orders WHERE order_id = $1 AND user_id = $2)",
                    resourceId, userId);
                return !result.empty() && result[0][0] == "t";
            }
            return false;
        }

        return false;
    };
}
```

**Проверка при доступе к истории заказов** (HistoryAuditManager.cpp:76-92)
```cpp
std::vector<std::vector<std::string>> getOrderStatusHistory(int userId, int orderId) {
    auto permissionCheck = getPermissionChecker();
    auto userInfo = getUserInfo(userId);
    std::string userRole = userInfo[0][2];

    // Проверяем права доступа
    if (!permissionCheck(userId, userRole, "order", orderId)) {
        std::cerr << "Доступ запрещен" << std::endl;
        return {};
    }
    // ... получение истории
}
```

### 5.3 Ограничения доступа к ресурсам

**1. История заказов**
- Admin: видит всю историю
- Manager: видит всю историю
- Customer: видит только свои изменения (HistoryAuditManager.cpp:102-113)

**2. Аудит-лог**
- Admin: полный доступ
- Manager: нет доступа
- Customer: нет доступа

**3. Отчеты**
- Только администраторы (ReportSystem.cpp:24-31):
```cpp
bool isAdmin(int userId) {
    try {
        auto result = db->executeQuery(
            "SELECT role FROM users WHERE user_id = $1", userId);
        return !result.empty() && result[0][0] == "admin";
    } catch (...) {
        return false;
    }
}
```

---

## 6. Аудит и история изменений

### 6.1 Описание таблиц

#### order_status_history
Хранит полную историю всех изменений статусов заказов.

**Структура:**
- `history_id` - уникальный идентификатор записи
- `order_id` - ID заказа
- `old_status` - предыдущий статус
- `new_status` - новый статус
- `changed_at` - время изменения
- `changed_by` - ID пользователя, изменившего статус
- `reason` - причина изменения (опционально)

**Примеры записей:**
| history_id | order_id | old_status | new_status | changed_at | changed_by | reason |
|------------|----------|------------|------------|------------|------------|--------|
| 1 | 15 | NULL | pending | 2025-01-14 10:30:00 | 1 | Заказ создан |
| 2 | 15 | pending | completed | 2025-01-14 11:00:00 | 2 | Заказ утвержден менеджером |
| 3 | 15 | completed | returned | 2025-01-20 14:00:00 | 1 | Возврат товара |

#### audit_log
Журнал всех операций (INSERT, UPDATE, DELETE) над сущностями системы.

**Структура:**
- `log_id` - уникальный идентификатор записи
- `entity_type` - тип сущности (order, product, user, order_items)
- `entity_id` - ID сущности
- `operation` - тип операции (insert, update, delete)
- `performed_by` - ID пользователя, выполнившего операцию
- `performed_at` - время выполнения операции

**Примеры записей:**
| log_id | entity_type | entity_id | operation | performed_by | performed_at |
|--------|-------------|-----------|-----------|--------------|--------------|
| 1 | product | 5 | insert | 3 | 2025-01-14 09:00:00 |
| 2 | product | 5 | update | 3 | 2025-01-14 09:15:00 |
| 3 | order | 15 | insert | 1 | 2025-01-14 10:30:00 |
| 4 | order | 15 | update | 2 | 2025-01-14 11:00:00 |

### 6.2 Механизм автоматического логирования

#### Триггер для истории статусов
Автоматически срабатывает при изменении статуса заказа (triggers.cpp:58-73):
```sql
CREATE TRIGGER trigger_log_status_change
AFTER UPDATE OF status ON orders
FOR EACH ROW
EXECUTE FUNCTION log_order_status_change();
```

**Функция триггера:**
```sql
CREATE FUNCTION log_order_status_change() RETURNS TRIGGER AS $$
BEGIN
    IF OLD.status != NEW.status THEN
        INSERT INTO order_status_history (
            order_id, old_status, new_status, changed_by
        ) VALUES (
            NEW.order_id, OLD.status, NEW.status,
            NULLIF(current_setting('app.user_id', TRUE), '')::INTEGER
        );
    END IF;
    RETURN NEW;
END;
$$;
```

#### Триггер для общего аудита
Автоматически логирует все операции над таблицами (audit_system.cpp:11-58):
```sql
CREATE FUNCTION audit_trigger_function() RETURNS TRIGGER AS $$
DECLARE
    v_operation VARCHAR;
    v_entity_id INTEGER;
    v_user_id INTEGER;
BEGIN
    IF TG_OP = 'INSERT' THEN v_operation := 'insert';
    ELSIF TG_OP = 'UPDATE' THEN v_operation := 'update';
    ELSIF TG_OP = 'DELETE' THEN v_operation := 'delete';
    END IF;

    -- Определяем ID в зависимости от таблицы
    IF TG_TABLE_NAME = 'orders' THEN
        v_entity_id := COALESCE(NEW.order_id, OLD.order_id);
    ELSIF TG_TABLE_NAME = 'products' THEN
        v_entity_id := COALESCE(NEW.product_id, OLD.product_id);
    -- ... другие таблицы
    END IF;

    v_user_id := NULLIF(current_setting('app.user_id', TRUE), '')::INTEGER;

    INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
    VALUES (TG_TABLE_NAME, v_entity_id, v_operation, v_user_id);

    RETURN COALESCE(NEW, OLD);
END;
$$;
```

**Применяется к таблицам:** users, products, orders, order_items

### 6.3 Примеры записей аудита

**Сценарий:** Администратор добавил новый товар

1. **Операция в коде** (Admin.cpp:87-90):
```cpp
db->executeQuery(
    "INSERT INTO products (name, price, stock_quantity) "
    "VALUES ($1, $2, $3)", "Ноутбук", 75000.00, 10);
```

2. **Автоматическая запись в audit_log:**
```
log_id: 157
entity_type: products
entity_id: 42
operation: insert
performed_by: 3 (admin)
performed_at: 2025-01-14 15:30:00
```

**Сценарий:** Менеджер утвердил заказ

1. **История статусов:**
```
history_id: 25
order_id: 18
old_status: pending
new_status: completed
changed_at: 2025-01-14 16:00:00
changed_by: 2 (manager)
reason: Статус изменен с pending на completed
```

2. **Запись в аудит-лог:**
```
log_id: 158
entity_type: order
entity_id: 18
operation: update
performed_by: 2
performed_at: 2025-01-14 16:00:00
```

---

## 7. Отчёт в формате CSV

### 7.1 Описание отчёта «История изменений заказов и действий пользователей»

Отчет объединяет данные из таблиц `orders`, `order_status_history`, `audit_log` и `users` для предоставления полной картины активности в системе.

**Назначение:**
- Аудит действий пользователей
- Анализ изменений статусов заказов
- Мониторинг операций с товарами и заказами
- Compliance и отчетность

**Доступ:** Только администраторы (проверка в ReportSystem.cpp:40-43)

### 7.2 SQL-функция для формирования отчёта

**Функция get_order_history_audit_report()** создается в ReportSystem (см. метод `createReportFunction()`):

```sql
CREATE OR REPLACE FUNCTION get_order_history_audit_report()
RETURNS TABLE(
    order_id INTEGER,
    customer_name VARCHAR,
    order_status VARCHAR,
    order_total DECIMAL,
    order_date TIMESTAMP,
    history_type VARCHAR,
    event_description TEXT,
    event_timestamp TIMESTAMP,
    performed_by INTEGER,
    performed_by_name VARCHAR,
    old_value VARCHAR,
    new_value VARCHAR,
    entity_type VARCHAR,
    entity_id INTEGER,
    operation VARCHAR
)
AS $$
BEGIN
    RETURN QUERY
    -- История статусов заказов
    SELECT
        o.order_id,
        u.name as customer_name,
        o.status as order_status,
        o.total_price as order_total,
        o.order_date,
        'status_change'::VARCHAR as history_type,
        ('Статус изменен: ' || COALESCE(h.old_status, 'NULL') || ' → ' || h.new_status) as event_description,
        h.changed_at as event_timestamp,
        h.changed_by as performed_by,
        ub.name as performed_by_name,
        h.old_status as old_value,
        h.new_status as new_value,
        'order_status_history'::VARCHAR as entity_type,
        h.history_id as entity_id,
        'update'::VARCHAR as operation
    FROM orders o
    JOIN users u ON o.user_id = u.user_id
    JOIN order_status_history h ON o.order_id = h.order_id
    LEFT JOIN users ub ON h.changed_by = ub.user_id

    UNION ALL

    -- Общий аудит-лог
    SELECT
        CASE WHEN al.entity_type = 'orders' THEN al.entity_id ELSE 0 END as order_id,
        ''::VARCHAR as customer_name,
        ''::VARCHAR as order_status,
        0::DECIMAL as order_total,
        NULL::TIMESTAMP as order_date,
        'audit'::VARCHAR as history_type,
        (al.operation || ' на ' || al.entity_type || ' #' || al.entity_id::TEXT) as event_description,
        al.performed_at as event_timestamp,
        al.performed_by as performed_by,
        u.name as performed_by_name,
        ''::VARCHAR as old_value,
        ''::VARCHAR as new_value,
        al.entity_type,
        al.entity_id,
        al.operation
    FROM audit_log al
    LEFT JOIN users u ON al.performed_by = u.user_id;
END;
$$ LANGUAGE plpgsql;
```

### 7.3 Генерация CSV-файла

**Код генерации отчета** (ReportSystem.cpp:38-119):

```cpp
bool generateOrderHistoryCSV(int userId, const std::string& outputPath) {
    // Проверка прав администратора
    if (!isAdmin(userId)) {
        std::cerr << "Ошибка: только администратор может генерировать отчеты" << std::endl;
        return false;
    }

    // Получение данных отчета
    auto reportData = db->executeQuery(R"(
        SELECT * FROM get_order_history_audit_report()
        ORDER BY event_timestamp DESC, order_id
    )");

    // Создание CSV файла
    std::ofstream csvFile(outputPath);

    // Заголовок CSV
    csvFile << "order_id,customer_name,order_status,order_total,order_date,"
            << "history_type,event_description,event_timestamp,performed_by,performed_by_name,"
            << "old_value,new_value,entity_type,entity_id,operation\n";

    // Обработка данных с использованием STL
    int totalRecords = 0;
    double totalOrderAmount = 0.0;

    auto processReportRow = [&](const std::vector<std::string>& row) {
        for (size_t i = 0; i < row.size(); ++i) {
            csvFile << escapeCSV(row[i]);
            if (i < row.size() - 1) csvFile << ",";
        }
        csvFile << "\n";
        totalRecords++;
        if (!row[3].empty()) {
            totalOrderAmount += std::stod(row[3]);
        }
    };

    std::for_each(reportData.begin(), reportData.end(), processReportRow);

    csvFile.close();
    return true;
}
```

### 7.4 Пример содержимого CSV-файла

**order_history_report.csv:**
```csv
order_id,customer_name,order_status,order_total,order_date,history_type,event_description,event_timestamp,performed_by,performed_by_name,old_value,new_value,entity_type,entity_id,operation
18,Иван Покупатель,completed,120000.00,2025-01-14 10:30:00,status_change,Статус изменен: pending → completed,2025-01-14 11:00:00,2,Мария Менеджер,pending,completed,order_status_history,25,update
18,Иван Покупатель,completed,120000.00,2025-01-14 10:30:00,audit,update на orders #18,2025-01-14 11:00:00,2,Мария Менеджер,,,orders,18,update
17,Петр Клиент,pending,45000.00,2025-01-14 09:00:00,status_change,Статус изменен: NULL → pending,2025-01-14 09:00:00,5,Петр Клиент,,pending,order_status_history,24,update
17,Петр Клиент,pending,45000.00,2025-01-14 09:00:00,audit,insert на orders #17,2025-01-14 09:00:00,5,Петр Клиент,,,orders,17,insert
0,,,0.00,,audit,insert на products #42,2025-01-14 08:00:00,3,Админ Системы,,,products,42,insert
0,,,0.00,,audit,update на products #42,2025-01-14 08:15:00,3,Админ Системы,,,products,42,update
```

**Дополнительные отчеты:**

1. **user_activity_report.csv** - Активность пользователей (ReportSystem.cpp:126-189)
2. **returns_report.csv** - Отчет по возвратам (ReportSystem.cpp:192+)

---

## 8. Сборка и запуск проекта

### 8.1 Требования к окружению

#### Обязательные компоненты

1. **Компилятор C++20:**
   - GCC 10+ или Clang 12+ (Linux/macOS)
   - MSVC 2019+ или MinGW (Windows)

2. **PostgreSQL 13+:**
   - Сервер PostgreSQL
   - Порт по умолчанию: 5432
   - Права на создание баз данных

3. **libpqxx 7.0+:**
   - C++ клиентская библиотека для PostgreSQL

4. **CMake 3.16+:**
   - Система сборки проектов

#### Опциональные компоненты
- vcpkg (для Windows) - менеджер пакетов для C++
- Homebrew (для macOS) - менеджер пакетов

### 8.2 Установка зависимостей

#### macOS (Homebrew)
```bash
# Установка PostgreSQL
brew install postgresql@14
brew services start postgresql@14

# Установка libpqxx
brew install libpqxx

# Установка CMake
brew install cmake
```

#### Linux (Ubuntu/Debian)
```bash
# Установка PostgreSQL
sudo apt update
sudo apt install postgresql postgresql-contrib

# Установка libpqxx
sudo apt install libpqxx-dev

# Установка CMake
sudo apt install cmake build-essential
```

#### Windows (vcpkg)
```powershell
# Установка vcpkg
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat

# Установка библиотек
.\vcpkg install libpqxx:x64-windows
.\vcpkg integrate install

# Установка PostgreSQL
# Скачать с https://www.postgresql.org/download/windows/
```

### 8.3 Настройка базы данных

```bash
# Запуск PostgreSQL (если не запущен)
# macOS:
brew services start postgresql@14

# Linux:
sudo systemctl start postgresql

# Создание пользователя (если нужно)
psql postgres
CREATE USER postgres WITH PASSWORD '288288';
ALTER USER postgres CREATEDB;
\q
```

**Примечание:** База данных `online_store` создается автоматически при запуске программы.

### 8.4 Инструкции по сборке

#### Шаг 1: Клонирование репозитория
```bash
cd /path/to/project
cd ExamTaskShop
```

#### Шаг 2: Создание директории сборки
```bash
mkdir build
cd build
```

#### Шаг 3: Конфигурация CMake

**macOS/Linux:**
```bash
cmake ..
```

**Windows (с vcpkg):**
```powershell
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

#### Шаг 4: Компиляция
```bash
# Сборка проекта
cmake --build .

# Или с указанием количества потоков
cmake --build . -j 4
```

### 8.5 Инструкция по запуску программы

После сборки создаются два исполняемых файла:
1. **TaskExamShop** - демонстрационная программа (автоматическая настройка и тестирование)
2. **CustomerMenuApp** - интерактивное меню покупателя

#### Вариант 1: Запуск демонстрационной программы (TaskExamShop)

Автоматически создает БД, таблицы, процедуры и демонстрирует все функции системы.

```bash
# Из директории build
./TaskExamShop

# Или полный путь
/Users/worker/IT_Tasks/ExamTaskShop/build/TaskExamShop
```

#### Ожидаемый вывод при успешном запуске:
```
Проверка существования базы данных 'online_store'...
База данных 'online_store' уже существует.
Подключение к базе данных успешно установлено

=== НАСТРОЙКА БАЗЫ ДАННЫХ ===

Создание таблиц базы данных...
Таблица 'users' создана успешно
Таблица 'products' создана успешно
Таблица 'orders' создана успешно
Таблица 'order_items' создана успешно
Таблица 'order_status_history' создана успешно
Таблица 'audit_log' создана успешно

Создание индексов...
Индексы созданы успешно

Создание хранимых процедур...
1. Создание функции create_order...
   Функция create_order создана
2. Создание процедуры update_order_status...
   Процедура update_order_status создана
...

=== СОЗДАНИЕ ТЕСТОВЫХ ДАННЫХ ===
Тестовые пользователи созданы
Тестовые товары созданы
Тестовые заказы созданы

=== ДЕМОНСТРАЦИЯ БИЗНЕС-ЛОГИКИ ===
...

=== СИСТЕМА ИНТЕРНЕТ-МАГАЗИНА УСПЕШНО СОЗДАНА ===
Все компоненты успешно настроены!

Созданные файлы отчетов:
  - demo_order_history.csv
  - demo_order_history.csv.summary.txt
  - demo_user_activity.csv
  - demo_returns.csv
  - test_order_history.csv
```

#### Вариант 2: Запуск интерактивного меню покупателя (CustomerMenuApp) ★ РЕКОМЕНДУЕТСЯ

Интерактивная программа для работы с системой как покупатель.

```bash
# Из директории build
./CustomerMenuApp

# Или полный путь
/Users/worker/IT_Tasks/ExamTaskShop/build/CustomerMenuApp
```

**Примечание:** Перед первым запуском CustomerMenuApp рекомендуется запустить TaskExamShop для инициализации базы данных.

#### Пример работы интерактивного меню:

```
╔══════════════════════════════════════════════════════════╗
║          ДОБРО ПОЖАЛОВАТЬ В ИНТЕРНЕТ-МАГАЗИН!            ║
╚══════════════════════════════════════════════════════════╝

Подключение к базе данных...
✓ Подключение установлено

╔═══════════════════════════════════════╗
║          ВХОД В СИСТЕМУ               ║
╚═══════════════════════════════════════╝

Доступные покупатели:
┌────┬───────────────────────┬──────────────────────┬─────────────┐
│ ID │ Имя                   │ Email                │ Статус      │
├────┼───────────────────────┼──────────────────────┼─────────────┤
│ 1  │ Иван Покупатель       │ ivan@example.com     │ ★ PREMIUM   │
│ 5  │ Тестовый Покупатель   │ test@example.com     │ Обычный     │
└────┴───────────────────────┴──────────────────────┴─────────────┘

Введите ваш ID для входа: 1
✓ Добро пожаловать, Иван Покупатель!

╔════════════════════════════════════════════════╗
║       ИНТЕРНЕТ-МАГАЗИН - МЕНЮ ПОКУПАТЕЛЯ       ║
╚════════════════════════════════════════════════╝
Пользователь: Иван Покупатель (ID: 1)
★ PREMIUM статус - скидка 10% на все заказы ★

┌────────────────────────────────────────────────┐
│  1. Создать новый заказ                        │
│  2. Добавить товар в заказ                     │
│  3. Удалить товар из заказа                    │
│  4. Просмотр моих заказов                      │
│  5. Просмотр статуса заказа                    │
│  6. Оплатить заказ                             │
│  7. Оформить возврат заказа                    │
│  8. Просмотр истории статусов заказа           │
│  9. Выход                                      │
└────────────────────────────────────────────────┘

Выберите действие (1-9): 2

═══ ДОБАВЛЕНИЕ ТОВАРА В ЗАКАЗ ═══

Доступные товары:
┌────┬──────────────────────┬──────────┬──────────┐
│ ID │ Название             │ Цена     │ Остаток  │
├────┼──────────────────────┼──────────┼──────────┤
│  1 │ Ноутбук              │ $  75000 │       10 │
│  2 │ Смартфон             │ $  45000 │       25 │
│  3 │ Наушники             │ $   5000 │       50 │
│  4 │ Клавиатура           │ $   8000 │       30 │
└────┴──────────────────────┴──────────┴──────────┘

Введите ID товара: 1
Введите количество: 2
Added to cart: Ноутбук x2 at $75000 each
Cart total: $150000
✓ Товар добавлен в корзину!

Выберите действие (1-9): 6

═══ ОПЛАТА ЗАКАЗА ═══

Выберите способ оплаты:
1. Банковская карта
2. Электронный кошелек
3. СБП (Система Быстрых Платежей)

Ваш выбор: 1

=== Оплата банковской картой ===
Заказ: #25
Сумма к оплате: $135000 (с учетом скидки 10%)

Введите номер карты (16 цифр): 1234567890123456
Введите имя держателя карты: IVAN IVANOV
Введите CVV (3 цифры): 123
Введите срок действия (MM/YY): 12/26

Обработка платежа...
Карта: **** **** **** 3456
Держатель: IVAN IVANOV
✓ Платеж успешно обработан через банковскую карту

✓ Заказ успешно оплачен!
```

### 8.6 Примеры работы меню для разных ролей

#### Пример: Администратор

**Демонстрация функций администратора** (main.cpp + Admin.cpp):
```
=== ДЕМОНСТРАЦИЯ: АДМИНИСТРАТОР ===
Пользователь: Админ Системы (ID: 3, роль: admin)

1. Добавление нового товара:
   Product added successfully: Игровая консоль
   Цена: $35000, Количество: 15

2. Обновление товара:
   Product updated successfully: ID 5
   Цена изменена: 75000 → 72000

3. Просмотр всех заказов:
   Заказ #1 | Клиент: Иван Покупатель | Статус: pending | Сумма: $80000 | Позиций: 3
   Заказ #2 | Клиент: Петр Клиент | Статус: completed | Сумма: $45000 | Позиций: 1
   ...

4. Изменение статуса заказа:
   Order status updated: 5 from pending to completed
   Products returned to stock for order: 5

5. Доступ к аудит-логу:
   Операции в системе:
   - insert product #42 (Админ Системы) - 2025-01-14 15:30:00
   - update order #5 (Админ Системы) - 2025-01-14 15:45:00
   ...
```

#### Пример: Менеджер

**Демонстрация функций менеджера** (Manager.cpp):
```
=== ДЕМОНСТРАЦИЯ: МЕНЕДЖЕР ===
Пользователь: Мария Менеджер (ID: 2, роль: manager)

1. Утверждение заказа:
   Проверка заказа #8...
   Товары в заказе:
   - Ноутбук x2 (в наличии: 10)
   - Смартфон x1 (в наличии: 25)

   Order approved successfully: 8
   Stock updated for 2 products
   User upgraded to premium loyalty level: 1

2. Обновление количества на складе:
   Stock updated for product: Клавиатура (ID: 12)
   Old quantity: 30, New quantity: 50

3. Попытка отмены completed заказа:
   Manager can only cancel pending orders. Current status: completed
   ERROR: Недостаточно прав

4. Попытка доступа к аудит-логу:
   ERROR: Доступ запрещен
```

#### Пример: Покупатель

**Демонстрация функций покупателя** (Customer.cpp):
```
=== ДЕМОНСТРАЦИЯ: ПОКУПАТЕЛЬ ===
Пользователь: Иван Покупатель (ID: 1, роль: customer)

1. Создание заказа (корзины):
   New cart created for customer: Иван Покупатель

2. Добавление товаров в корзину:
   Added to cart: Ноутбук x2 at $75000 each
   Cart total: $150000

   Added to cart: Наушники x1 at $5000 each
   Cart total: $155000

3. Оформление заказа:
   Payment successful!
   Order #25 created
   Total amount: $155000
   Payment method: credit_card
   Items purchased: 2

4. Просмотр своих заказов:
   Ваши заказы:
   - Заказ #25: completed, сумма: $155000
   - Заказ #18: pending, сумма: $80000

5. Отмена своего pending заказа:
   Order canceled successfully: 18
   Products returned to stock

6. Попытка отмены чужого заказа:
   Order not found or access denied
   ERROR: Недостаточно прав

7. Система лояльности:
   Congratulations! You've been upgraded to Premium loyalty level!
   Следующий заказ: Premium discount applied: 10%
```

### 8.7 Примеры логов и истории изменений заказов

#### История изменений заказа

**Запрос истории для заказа #15:**
```
=== ИСТОРИЯ СТАТУСОВ ЗАКАЗА #15 ===
==========================================
ID: 35 | Заказ: 15 | Статус: completed → returned | Дата: 2025-01-20 14:00:00 | Изменил: Иван Покупатель
ID: 34 | Заказ: 15 | Статус: pending → completed | Дата: 2025-01-14 11:00:00 | Изменил: Мария Менеджер
ID: 33 | Заказ: 15 | Статус: NULL → pending | Дата: 2025-01-14 10:30:00 | Изменил: Иван Покупатель
```

#### Аудит-лог для пользователя

**Запрос аудита для администратора #3:**
```
=== АУДИТ ДЕЙСТВИЙ ПОЛЬЗОВАТЕЛЯ #3 ===
Операции пользователя: Админ Системы

1. insert product #42 (2025-01-14 15:30:00)
   Детали: Товар: Игровая консоль

2. update product #5 (2025-01-14 15:35:00)
   Детали: Товар: Ноутбук

3. update order #5 (2025-01-14 15:45:00)
   Детали: Заказ #5 - completed

4. delete product #38 (2025-01-14 16:00:00)
   Детали: Товар: Устаревший товар

Всего операций: 4
```

#### Журнал системы при транзакции

**Успешная транзакция:**
```
Начало транзакции...
  → Обновление статуса заказа #15: pending → completed
  → Уменьшение количества товара #1: 10 → 8
  → Уменьшение количества товара #3: 50 → 49
  → Запись в order_status_history
  → Запись в audit_log
Транзакция зафиксирована: COMMIT
```

**Откат транзакции при ошибке:**
```
Начало транзакции...
  → Обновление статуса заказа #18: pending → completed
  → Уменьшение количества товара #5: 2 → 0
  → Уменьшение количества товара #7: 0 → -2
  ✗ ОШИБКА: Недостаточно товара на складе
Транзакция отменена: ROLLBACK
Все изменения откачены
```

### 8.8 Структура созданных файлов отчетов

После успешного запуска программы создаются следующие CSV-отчеты:

```
ExamTaskShop/
├── demo_order_history.csv             # Основной отчет по заказам и аудиту
├── demo_order_history.csv.summary.txt # Сводка по отчету
├── demo_user_activity.csv             # Активность пользователей
├── demo_returns.csv                   # Отчет по возвратам
└── test_order_history.csv             # Тестовый отчет
```

**Содержимое summary файла:**
```
=== СВОДКА ОТЧЕТА ===
Дата создания: 2025-01-14 16:30:00
Администратор: Админ Системы (ID: 3)

Статистика:
  Всего записей: 147
  Уникальных заказов: 23
  Общая сумма заказов: $2,345,000.00
  Средний чек: $102,000.00

Операции по типам:
  Изменения статусов: 58
  Записи аудита: 89
    - INSERT: 35
    - UPDATE: 42
    - DELETE: 12

Активность по ролям:
  Администраторы: 45 операций
  Менеджеры: 32 операции
  Покупатели: 70 операций
```

---

## 9. Структура проекта

```
ExamTaskShop/
├── main.cpp                           # Точка входа, инициализация системы
├── main_interactive.cpp               # ★ Интерактивное меню покупателя
├── CMakeLists.txt                     # Конфигурация сборки (2 исполняемых файла)
├── README.md                          # Этот файл
│
├── DatabaseConnection.hpp             # Шаблонный класс для работы с БД
├── DatabaseManager.cpp                # Менеджер подключений
│
├── PaymentStrategy.hpp                # ★ Паттерн Стратегия для оплаты
├── CustomerMenu.cpp                   # ★ Интерактивное меню покупателя
│
├── User.cpp                           # Базовый класс пользователей
├── Admin.cpp                          # Класс администратора
├── Manager.cpp                        # Класс менеджера
├── Customer.cpp                       # Класс покупателя
│
├── Order.cpp                          # Класс заказа
├── OrderItem.cpp                      # Класс позиции заказа
├── Payment.cpp                        # Класс оплаты
│
├── HistoryAuditManager.cpp            # Управление историей и аудитом
├── HistoryAuditManager.hpp
├── ReportSystem.cpp                   # Система генерации отчетов
├── ReportSystem.hpp
│
└── init/                              # Скрипты инициализации БД
    ├── database_setup.cpp             # Создание таблиц и индексов
    ├── database_setup.hpp
    ├── stored_procedure.cpp           # Хранимые процедуры
    ├── stored_procedure.hpp
    ├── functions.cpp                  # Функции БД
    ├── functions.hpp
    ├── triggers.cpp                   # Триггеры
    ├── triggers.hpp
    ├── audit_system.cpp               # Настройка системы аудита
    ├── audit_system.hpp
    ├── business_logic.cpp             # Демонстрация бизнес-логики
    ├── business_logic.hpp
    ├── order_operations.cpp           # Операции с заказами
    └── order_operations.hpp
```

**Исполняемые файлы после сборки:**
- `TaskExamShop` - демонстрационная программа (автоматическая настройка БД и демонстрация функций)
- `CustomerMenuApp` - интерактивное меню покупателя (★ рекомендуется для демонстрации)

---

## 10. Ключевые особенности реализации

### 10.1 Безопасность
- Параметризованные запросы для защиты от SQL-инъекций
- Ролевая модель доступа
- Хеширование паролей (заготовка в структуре БД)
- Валидация данных на уровне БД (CHECK constraints)

### 10.2 Надежность
- Транзакции ACID
- Автоматический откат при ошибках
- Проверка целостности данных
- Каскадное удаление с ограничениями

### 10.3 Производительность
- Индексы на часто запрашиваемых полях
- Использование умных указателей
- Move-семантика C++20
- Эффективные STL алгоритмы

### 10.4 Масштабируемость
- Шаблонные классы для повторного использования
- Модульная архитектура
- Разделение логики и данных
- Возможность расширения ролей

---

## 11. Примеры использования

### Пример 1: Создание и оплата заказа покупателем

```cpp
// Создание покупателя
auto db = std::make_shared<DatabaseConnection>(...);
Customer customer(1, "Иван", "ivan@example.com", db);

// Создание заказа
customer.createOrder();

// Добавление товаров
customer.addToOrder(1, 2);  // Товар #1, количество 2
customer.addToOrder(3, 1);  // Товар #3, количество 1

// Оплата
customer.makePayment("credit_card");
```

### Пример 2: Утверждение заказа менеджером

```cpp
Manager manager(2, "Мария", "maria@example.com", db);

// Утверждение заказа (проверка наличия + обновление склада)
bool approved = manager.approveOrder(15);

if (approved) {
    std::cout << "Заказ утвержден" << std::endl;
}
```

### Пример 3: Генерация отчета администратором

```cpp
ReportManager reportManager(db);

// Генерация CSV отчета
reportManager.generateOrderHistoryCSV(3, "report.csv");
reportManager.generateUserActivityReport(3, "activity.csv");
```

---

## 12. Тестирование

Программа включает встроенные тесты в функции `testExamples()` (main.cpp:188-265):

1. **Создание заказа через процедуру**
2. **Получение статуса заказа**
3. **Статистика заказов пользователей**
4. **Тестирование системы аудита**

Все тесты запускаются автоматически при старте программы.

---

## 13. Возможные проблемы и решения

### Проблема: Ошибка подключения к PostgreSQL
**Решение:**
```bash
# Проверка статуса PostgreSQL
brew services list  # macOS
sudo systemctl status postgresql  # Linux

# Проверка порта
psql -h localhost -p 5432 -U postgres -d postgres

# Изменение пароля (если нужно)
ALTER USER postgres PASSWORD '288288';
```

### Проблема: libpqxx не найдена при сборке
**Решение (macOS):**
```bash
# Переустановка libpqxx
brew uninstall libpqxx
brew install libpqxx

# Указание пути явно
cmake .. -DLIBPQXX_INCLUDE_DIR=/opt/homebrew/include \
         -DLIBPQXX_LIBRARY=/opt/homebrew/lib/libpqxx.dylib
```

### Проблема: Ошибка "database online_store already exists"
**Решение:**
Программа автоматически обрабатывает эту ситуацию. База используется повторно.

При необходимости пересоздать БД:
```sql
psql -U postgres
DROP DATABASE online_store;
\q
```

---

## 14. Заключение

Проект демонстрирует полный цикл разработки системы на C++ с использованием:
- Объектно-ориентированного программирования
- Современных возможностей C++20 (умные указатели, лямбды, STL)
- Профессиональной работы с PostgreSQL (процедуры, триггеры, транзакции)
- Системы аудита и отчетности
- Ролевой модели безопасности

Система полностью функциональна и готова к демонстрации всех заявленных возможностей.

---

## Автор
Проект выполнен в рамках экзамена по информатике

Дата: Январь 2026
