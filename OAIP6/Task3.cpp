#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Структура для представления заказа
struct Order {
    string customerName;
    vector<string> items;
};

// Функция для добавления заказа в очередь
void addOrder(queue<Order>& orderQueue, const Order& newOrder) {
    orderQueue.push(newOrder);
    cout << "Заказ от " << newOrder.customerName << " добавлен в очередь" << endl;
}

// Функция для обработки заказа из очереди
void processOrder(queue<Order>& orderQueue) {
    if (orderQueue.empty()) {
        cout << "Очередь заказов пуста!" << endl;
    } else {
        Order currentOrder = orderQueue.front();
        orderQueue.pop();
        
        cout << "\n--- Обработка заказа ---" << endl;
        cout << "Клиент: " << currentOrder.customerName << endl;
        cout << "Товары: ";
        
        // Выводим все товары из вектора
        for (int i = 0; i < currentOrder.items.size(); i++) {
            cout << currentOrder.items[i];
            if (i < currentOrder.items.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
        cout << "Заказ успешно обработан!\n" << endl;
    }
}

int Task3() {
    queue<Order> orderQueue;
    
    cout << "=== Интернет-магазин начал работу ===\n" << endl;
    
    // Создаём заказы с векторами товаров
    Order order1;
    order1.customerName = "Иван Петров";
    order1.items = {"Ноутбук", "Мышка"};
    
    Order order2;
    order2.customerName = "Мария Сидорова";
    order2.items = {"Телефон", "Чехол", "Наушники"};
    
    Order order3;
    order3.customerName = "Алексей Смирнов";
    order3.items = {"Клавиатура"};
    
    // Добавляем заказы в очередь
    cout << "Поступили новые заказы:\n" << endl;
    addOrder(orderQueue, order1);
    addOrder(orderQueue, order2);
    addOrder(orderQueue, order3);
    
    cout << "\n=== Начинаем обработку заказов ===\n" << endl;
    
    // Обрабатываем заказы
    processOrder(orderQueue);
    processOrder(orderQueue);
    
    // Добавляем ещё один заказ
    cout << "\nПоступил новый заказ:\n" << endl;
    Order order4;
    order4.customerName = "Елена Козлова";
    order4.items = {"Монитор", "HDMI кабель"};
    addOrder(orderQueue, order4);
    
    cout << "\n=== Продолжаем обработку ===\n" << endl;
    
    // Обрабатываем оставшиеся заказы
    processOrder(orderQueue);
    processOrder(orderQueue);
    
    // Проверяем пустую очередь
    cout << "Проверяем очередь:" << endl;
    processOrder(orderQueue);
    
    return 0;
}