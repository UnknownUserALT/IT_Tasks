#include <iostream>
#include <memory>
#include <string>

// Базовый класс для статусов задач
class TaskStatus {
public:
    virtual ~TaskStatus() = default;
    virtual std::string getStatus() const = 0;
    virtual std::string getColor() const = 0;
    virtual bool canChangeTo(const std::string& newStatus) const = 0;
};

// Выполненная задача
class CompletedTask : public TaskStatus {
public:
    std::string getStatus() const override {
        return "Выполнена";
    }
    
    std::string getColor() const override {
        return "green";
    }
    
    bool canChangeTo(const std::string& newStatus) const override {
        return newStatus == "В процессе";
    }
};

// Задача в процессе
class InProgressTask : public TaskStatus {
public:
    std::string getStatus() const override {
        return "В процессе";
    }
    
    std::string getColor() const override {
        return "orange";
    }
    
    bool canChangeTo(const std::string& newStatus) const override {
        return newStatus == "Выполнена" || newStatus == "Отложена";
    }
};

// Отложенная задача
class DeferredTask : public TaskStatus {
public:
    std::string getStatus() const override {
        return "Отложена";
    }
    
    std::string getColor() const override {
        return "gray";
    }
    
    bool canChangeTo(const std::string& newStatus) const override {
        return newStatus == "В процессе";
    }
};

// Класс задачи
class Task {
private:
    int id;
    std::string title;
    std::string priority;
    std::string dueDate;
    std::unique_ptr<TaskStatus> status;
    
    std::unique_ptr<TaskStatus> createStatus(const std::string& statusStr) {
        if (statusStr == "Выполнена") {
            return std::make_unique<CompletedTask>();
        } else if (statusStr == "В процессе") {
            return std::make_unique<InProgressTask>();
        } else if (statusStr == "Отложена") {
            return std::make_unique<DeferredTask>();
        }
        // По умолчанию - "В процессе"
        return std::make_unique<InProgressTask>();
    }
    
public:
    Task(int id, const std::string& title, const std::string& priority, 
         const std::string& dueDate, const std::string& statusStr)
        : id(id), title(title), priority(priority), dueDate(dueDate) {
        status = createStatus(statusStr);
    }
    
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getPriority() const { return priority; }
    std::string getDueDate() const { return dueDate; }
    std::string getStatus() const { return status->getStatus(); }
    std::string getStatusColor() const { return status->getColor(); }
    
    bool changeStatus(const std::string& newStatus) {
        if (status->canChangeTo(newStatus)) {
            status = createStatus(newStatus);
            return true;
        }
        std::cout << "Невозможно изменить статус с '" << status->getStatus()
                  << "' на '" << newStatus << "'" << std::endl;
        return false;
    }
    
    void display() const {
        std::cout << "ID: " << id 
                  << " | Задача: " << title
                  << " | Приоритет: " << priority
                  << " | Срок: " << dueDate
                  << " | Статус: " << status->getStatus()
                  << " [" << status->getColor() << "]" << std::endl;
    }
};
