//
// Created by Worker on 8/12/2025 AD.
//

#include "User.h"

#include <iostream>
#include <string>

class User {
private:
    int id;
    std::string name;

public:
    // Конструктор
    User(int id, const std::string& name) : id(id), name(name) {}

    // Конструктор по умолчанию
    User() : id(0), name("") {}

    // Геттеры
    int getId() const { return id; }
    std::string getName() const { return name; }

    // Сеттеры
    void setId(int new_id) { id = new_id; }
    void setName(const std::string& new_name) { name = new_name; }

    // Вывод информации о пользователе
    void print() const {
        std::cout << "ИД пользователя: " << id << ", Имя: " << name << std::endl;
    }
};
