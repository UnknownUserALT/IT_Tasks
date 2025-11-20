#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>
#include <iomanip>
#include <ctime>

using namespace std;

// Структура для хранения задачи
struct Task {
    string name;
    int priority;
    string deadline;
};

// Функция для получения текущей даты и времени
string getCurrentDateTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localTime);
    return string(buffer);
}

// Функция для записи заголовка в output.txt
void writeHeaderToOutput(ofstream& outputFile, const string& operation) {
    outputFile << "=== Результаты операции: " << operation << " ===" << endl;
    outputFile << "Время выполнения: " << getCurrentDateTime() << endl;
    outputFile << "================================================" << endl;
}

// Функция для записи задач в output.txt
void writeTasksToOutput(ofstream& outputFile, const vector<Task>& tasks, const string& title = "") {
    if (!title.empty()) {
        outputFile << title << endl;
    }

    outputFile << "================================================" << endl;
    outputFile << setw(20) << left << "Название"
               << setw(12) << "Приоритет"
               << setw(15) << "Срок" << endl;
    outputFile << "------------------------------------------------" << endl;

    for (const auto& task : tasks) {
        outputFile << setw(20) << left << task.name
                   << setw(12) << task.priority
                   << setw(15) << task.deadline << endl;
    }
    outputFile << "================================================" << endl;
    outputFile << "Всего задач: " << tasks.size() << endl << endl;
}

// Функция для чтения задач из файла
vector<Task> readTasksFromFile() {
    vector<Task> tasks;
    ifstream file("tasks.txt");

    if (!file.is_open()) {
        return tasks;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        Task task;
        stringstream ss(line);
        string token;

        // Разбиваем строку по разделителю '|'
        getline(ss, task.name, '|');
        getline(ss, token, '|');
        task.priority = stoi(token);
        getline(ss, task.deadline, '|');

        tasks.push_back(task);
    }

    file.close();
    return tasks;
}

// Функция для поиска задачи по названию
void searchTaskByName() {
    string searchName;
    cout << "\nВведите название задачи для поиска: ";
    cin.ignore();
    getline(cin, searchName);

    vector<Task> tasks = readTasksFromFile();
    vector<Task> foundTasks;

    // Поиск задач
    for (const auto& task : tasks) {
        if (task.name.find(searchName) != string::npos) {
            foundTasks.push_back(task);
        }
    }

    // Запись результатов в файл
    ofstream outputFile("../output.txt", ios::app);
    if (outputFile.is_open()) {
        writeHeaderToOutput(outputFile, "Поиск по названию: '" + searchName + "'");

        if (foundTasks.empty()) {
            outputFile << "Задачи с названием '" << searchName << "' не найдены." << endl << endl;
        } else {
            writeTasksToOutput(outputFile, foundTasks, "Найденные задачи:");
        }
        outputFile.close();
    }

    // Вывод в консоль
    if (foundTasks.empty()) {
        cout << "Задачи с названием '" << searchName << "' не найдены." << endl;
        cout << "Результаты записаны в output.txt" << endl;
    } else {
        cout << "Найдено " << foundTasks.size() << " задач. Результаты записаны в output.txt" << endl;

        // Краткий вывод в консоль для подтверждения
        cout << "\nКраткий просмотр:" << endl;
        for (const auto& task : foundTasks) {
            cout << "- " << task.name << " (приоритет: " << task.priority << ")" << endl;
        }
    }
}

// Функции-компараторы для сортировки
bool compareByPriority(const Task& a, const Task& b) {
    return a.priority < b.priority;
}

bool compareByDeadline(const Task& a, const Task& b) {
    return a.deadline < b.deadline;
}

// Функция для сортировки задач
void sortTasks() {
    vector<Task> tasks = readTasksFromFile();

    if (tasks.empty()) {
        cout << "Нет задач для сортировки." << endl;
        return;
    }

    cout << "\nВыберите критерий сортировки:" << endl;
    cout << "1 - По приоритету" << endl;
    cout << "2 - По сроку выполнения" << endl;
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;

    string sortType;
    switch (choice) {
        case 1:
            sort(tasks.begin(), tasks.end(), compareByPriority);
            sortType = "по приоритету";
            cout << "\nЗадачи отсортированы по приоритету:" << endl;
            break;
        case 2:
            sort(tasks.begin(), tasks.end(), compareByDeadline);
            sortType = "по сроку выполнения";
            cout << "\nЗадачи отсортированы по сроку выполнения:" << endl;
            break;
        default:
            cout << "Неверный выбор!" << endl;
            return;
    }

    // Запись результатов в файл
    ofstream outputFile("../output.txt", ios::app);
    if (outputFile.is_open()) {
        writeHeaderToOutput(outputFile, "Сортировка " + sortType);
        writeTasksToOutput(outputFile, tasks, "Отсортированные задачи:");
        outputFile.close();
    }

    // Краткий вывод в консоль
    cout << "Результаты сортировки записаны в output.txt" << endl;
    cout << "Всего задач: " << tasks.size() << endl;
}

// Функция для фильтрации задач по приоритету
void filterTasksByPriority() {
    int maxPriority;
    cout << "\nВведите максимальный приоритет: ";

    while (!(cin >> maxPriority)) {
        cout << "Ошибка! Введите целое число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<Task> tasks = readTasksFromFile();
    vector<Task> filteredTasks;

    // Фильтруем задачи
    for (const auto& task : tasks) {
        if (task.priority <= maxPriority) {
            filteredTasks.push_back(task);
        }
    }

    // Сортируем отфильтрованные задачи по приоритету для лучшего отображения
    sort(filteredTasks.begin(), filteredTasks.end(), compareByPriority);

    // Запись результатов в файл
    ofstream outputFile("../output.txt", ios::app);
    if (outputFile.is_open()) {
        writeHeaderToOutput(outputFile, "Фильтр по приоритету (<= " + to_string(maxPriority) + ")");

        if (filteredTasks.empty()) {
            outputFile << "Задачи с приоритетом <= " << maxPriority << " не найдены." << endl << endl;
        } else {
            writeTasksToOutput(outputFile, filteredTasks, "Отфильтрованные задачи:");
        }
        outputFile.close();
    }

    // Вывод в консоль
    if (filteredTasks.empty()) {
        cout << "Задачи с приоритетом <= " << maxPriority << " не найдены." << endl;
        cout << "Результаты записаны в output.txt" << endl;
    } else {
        cout << "Найдено " << filteredTasks.size() << " задач. Результаты записаны в output.txt" << endl;

        // Краткий вывод в консоль для подтверждения
        cout << "\nКраткий просмотр:" << endl;
        for (const auto& task : filteredTasks) {
            cout << "- " << task.name << " (приоритет: " << task.priority << ")" << endl;
        }
    }
}

// Функция для добавления новых задач
void addTasks() {
    ofstream file("tasks.txt", ios::app);

    if (!file.is_open()) {
        cout << "Ошибка: не удалось открыть файл tasks.txt" << endl;
        return;
    }

    cout << "\n=== Добавление новых задач ===" << endl;
    cout << "Для завершения ввода введите пустую строку в названии задачи" << endl;

    while (true) {
        string taskName;
        int priority;
        string deadline;

        cout << "\nВведите название задачи: ";
        getline(cin, taskName);

        if (taskName.empty()) {
            cout << "Ввод завершен." << endl;
            break;
        }

        cout << "Введите приоритет (целое число): ";
        while (!(cin >> priority)) {
            cout << "Ошибка! Введите целое число для приоритета: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Введите срок выполнения: ";
        getline(cin, deadline);

        file << taskName << "|" << priority << "|" << deadline << endl;
        cout << "Задача добавлена успешно!" << endl;
    }

    file.close();
}

// Функция для просмотра содержимого output.txt
void viewOutputFile() {
    ifstream outputFile("../output.txt");
    if (!outputFile.is_open()) {
        cout << "Файл output.txt не существует или пуст." << endl;
        return;
    }

    cout << "\n=== Содержимое output.txt ===" << endl;
    string line;
    while (getline(outputFile, line)) {
        cout << line << endl;
    }
    outputFile.close();
}

// Главное меню
void showMenu() {
    cout << "\n=== Программа для управления задачами ===" << endl;
    cout << "1 - Добавить новые задачи" << endl;
    cout << "2 - Поиск задачи по названию" << endl;
    cout << "3 - Сортировка задач" << endl;
    cout << "4 - Фильтр по приоритету" << endl;
    cout << "5 - Просмотреть output.txt" << endl;
    cout << "6 - Очистить output.txt" << endl;
    cout << "0 - Выход" << endl;
    cout << "Выберите действие: ";
}

int main() {
    int choice;

    do {
        showMenu();
        cin >> choice;
        cin.ignore(); // Очистка буфера

        switch (choice) {
            case 0: {
                cout << "Программа завершена." << endl;
                break;
            }
            case 1: {
                addTasks();
                break;
            }
            case 2: {
                searchTaskByName();
                break;
            }

            case 3: {
                sortTasks();
                break;
            }

            case 4: {
                filterTasksByPriority();
                break;
            }

            case 5: {
                viewOutputFile();
                break;
            }
            case 6: {
                ofstream clearFile("../output.txt", ios::trunc);
                clearFile.close();
                cout << "Файл output.txt очищен." << endl;
                break;
            }
            default: {
                cout << "Неверный выбор! Попробуйте снова." << endl;
                break;
            }
        }

    } while (choice != 0);

    return 0;
}