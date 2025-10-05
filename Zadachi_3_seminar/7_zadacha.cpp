#include<iostream>

using namespace std;

int obnul()
{
    setlocale(LC_ALL, "ru");

    const int n = 15;
    int numbers[n];

    // Ввод массива
    cout << "Введите " << n << " целых чисел:" << endl;
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
    if (numbers[min_index] == 0 || numbers[max_index] == 0) {
        // Обнуляем элементы, предшествующие минимальному
        for (int i = 0; i < min_index; i++) {
            numbers[i] = 0;
        }

        cout << "Условие выполняется! Обнулены элементы перед минимальным:" << endl;
        cout << "Минимальный элемент: " << numbers[min_index] << " на месте " << min_index + 1 << endl;
        cout << "Максимальный элемент: " << numbers[max_index] << " на месте " << max_index + 1 << endl;
    }
    else {
        cout << "Условие НЕ выполняется" << endl;
        cout << "Минимальный элемент: " << numbers[min_index] << endl;
        cout << "Максимальный элемент: " << numbers[max_index] << endl;
    }

    // Вывод массива
    cout << "Результат:" << endl;
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }

    return 0;
}