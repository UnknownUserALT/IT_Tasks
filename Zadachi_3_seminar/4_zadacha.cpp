#include<iostream>

using namespace std;

int mult()
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

    // Проверяем условие и вычисляем произведение
    if (max_index < min_index) {
        double product = numbers[min_index] * numbers[max_index];
        cout << "Максимальный элемент предшествует минимальному" << endl;
        cout << "Минимальный элемент: " << numbers[min_index] << " (индекс " << min_index << ")" << endl;
        cout << "Максимальный элемент: " << numbers[max_index] << " (индекс " << max_index << ")" << endl;
        cout << "Произведение: " << numbers[min_index] << " * " << numbers[max_index] << " = " << product << endl;
    }
    else {
        cout << "Максимальный элемент НЕ предшествует минимальному" << endl;
        cout << "Минимальный элемент: " << numbers[min_index] << " (индекс " << min_index << ")" << endl;
        cout << "Максимальный элемент: " << numbers[max_index] << " (индекс " << max_index << ")" << endl;
    }

    return 0;
}