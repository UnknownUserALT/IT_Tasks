#include<iostream>

using namespace std;

int umen()
{
    setlocale(LC_ALL, "ru");

    const int n = 15;
    double numbers[n];

    // Ввод массива
    cout << "Введите " << n << " вещественных чисел:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    // Поиск индексов минимального и максимального элементов
    int min_index = 0;
    int max_index = 0;

    for (int i = 1; i < n; i++) {
        if (numbers[i] < numbers[min_index]) {
            min_index = i;
        }
        if (numbers[i] > numbers[max_index]) {
            max_index = i;
        }
    }

    // Проверяем условие
    if (min_index == 1 && max_index == 0) {
        // Уменьшаем все элементы в 2 раза
        for (int i = 0; i < n; i++) {
            numbers[i] = numbers[i] / 2.0;
        }

        cout << "Условие выполняется! Все элементы уменьшены в 2 раза:" << endl;
    }
    else {
        cout << "Условие НЕ выполняется" << endl;
        cout << "Минимальный элемент на месте: " << min_index + 1 << endl;
        cout << "Максимальный элемент на месте: " << max_index + 1 << endl;
    }

    // Вывод массива
    cout << "Результат:" << endl;
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }

    return 0;
}