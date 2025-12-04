#include <iostream>
#include <queue>
#include <string>

using namespace std;

// Функция для добавления события в очередь
void addEvent(queue<string>& eventQueue, const string& event) {
    eventQueue.push(event);
    cout << "Событие добавлено: " << event << endl;
}

// Функция для обработки событий из очереди
void processEvents(queue<string>& eventQueue) {
    if (eventQueue.empty()) {
        cout << "Очередь событий пуста!" << endl;
    } else {
        string event = eventQueue.front();
        eventQueue.pop();
        cout << "Обрабатывается событие: " << event << endl;
    }
}

int Task2() {
    queue<string> eventQueue;
    
    cout << "=== Сетевое приложение запущено ===" << endl;
    cout << "\nПоступают запросы от клиентов:\n" << endl;
    
    // Добавляем события (запросы от клиентов)
    addEvent(eventQueue, "Клиент 1: Запрос на авторизацию");
    addEvent(eventQueue, "Клиент 2: Запрос данных пользователя");
    addEvent(eventQueue, "Клиент 3: Отправка сообщения");
    addEvent(eventQueue, "Клиент 1: Обновление профиля");
    
    cout << "\n--- Начинаем обработку событий ---\n" << endl;
    
    // Обрабатываем все события
    processEvents(eventQueue);
    processEvents(eventQueue);
    
    cout << "\nПоступили новые запросы:\n" << endl;
    
    addEvent(eventQueue, "Клиент 4: Загрузка файла");
    addEvent(eventQueue, "Клиент 2: Выход из системы");
    
    cout << "\nПродолжаем обработку:\n" << endl;
    
    // Обрабатываем оставшиеся события
    processEvents(eventQueue);
    processEvents(eventQueue);
    processEvents(eventQueue);
    processEvents(eventQueue);
    
    // Пытаемся обработать когда очередь пуста
    cout << "\nПроверяем пустую очередь:\n" << endl;
    processEvents(eventQueue);
    
    return 0;
}