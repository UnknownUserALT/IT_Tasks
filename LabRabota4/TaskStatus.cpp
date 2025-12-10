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
