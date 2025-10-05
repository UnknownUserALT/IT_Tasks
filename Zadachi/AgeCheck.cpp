#include <iostream>
using namespace std;

int chage() {
    int age;

    // Ввод возраста
    cout << "Введите ваш возраст: ";
    cin >> age;

    // Проверка корректности ввода
    if (age < 0 || age > 150) {
        cout << "Ошибка: возраст должен быть от 0 до 150 лет!" << endl;
        return 1;
    }

    // Проверка возрастной категории
    if (age < 18) {
        cout << "Вам " << age << " лет - вы несовершеннолетний" << endl;
    }
    else if (age >= 18 && age <= 60) {
        cout << "Вам " << age << " лет - вы взрослый человек" << endl;
    }
    else {
        cout << "Вам " << age << " лет - вы пенсионер" << endl;
    }

    return 0;
}