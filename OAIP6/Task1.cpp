#include <iostream>
#include <queue>
#include <string>

using namespace std;

// Функция для добавления человека в очередь
void addToQueue(queue<string>& queue, const string& person) {
    queue.push(person);
    cout << person << " добавлен в очередь" << endl;
}

// Функция для обработки человека из очереди
void processQueue(queue<string>& queue) {
    if (queue.empty()) {
        cout << "Очередь пуста! Никого нет для обработки." << endl;
    } else {
        string person = queue.front();
        queue.pop();
        cout << person << " обработан и вышел из очереди" << endl;
    }
}

int Task1() {
    queue<string> shopQueue;
    
    // Добавляем людей в очередь
    addToQueue(shopQueue, "Иван");
    addToQueue(shopQueue, "Мария");
    addToQueue(shopQueue, "Петр");
    
    cout << "\nНачинаем обработку очереди:\n" << endl;
    
    // Обрабатываем людей из очереди
    processQueue(shopQueue);
    processQueue(shopQueue);
    processQueue(shopQueue);
    
    // Пытаемся обработать пустую очередь
    processQueue(shopQueue);
    
    cout << "\nДобавим ещё людей:\n" << endl;
    
    addToQueue(shopQueue, "Анна");
    addToQueue(shopQueue, "Сергей");
    
    cout << "\nОбработаем их:\n" << endl;
    
    processQueue(shopQueue);
    processQueue(shopQueue);
    processQueue(shopQueue);
    
    return 0;
}