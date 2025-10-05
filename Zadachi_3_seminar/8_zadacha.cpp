#include<iostream>

using namespace std;

int max()
{
    setlocale(LC_ALL, "ru");
    const int n = 10;
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
    if (max_index == 1 && min_index == 4) {
        // Заменяем элементы от минимального до конца на максимальный
        int max_value = numbers[max_index];
        for (int i = min_index; i < n; i++) {
            numbers[i] = max_value;
        }

        cout << "Условие выполняется!" << endl;
        cout << "Максимальный элемент: " << max_value << " на 2 месте" << endl;
        cout << "Минимальный элемент был: " << numbers[min_index] << " на 5 месте" << endl;
    }
    else {
        cout << "Условие НЕ выполняется" << endl;
        cout << "Максимальный элемент на месте: " << max_index + 1 << endl;
        cout << "Минимальный элемент на месте: " << min_index + 1 << endl;
    }

    // Вывод массива
    cout << "Результат:" << endl;
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }

    return 0;
}