#include<iostream>

using namespace std;

int ariph()
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

    // Проверяем условие и вычисляем среднее арифметическое
    if (min_index == 4 && max_index == 9) { // Индексы с 0, поэтому 4 = 5-е место, 9 = 10-е место
        double average = (numbers[min_index] + numbers[max_index]) / 2.0;
        cout << "Условие выполняется!" << endl;
        cout << "Минимальный элемент: " << numbers[min_index] << " на 5 месте" << endl;
        cout << "Максимальный элемент: " << numbers[max_index] << " на 10 месте" << endl;
        cout << "Среднее арифметическое: (" << numbers[min_index] << " + " << numbers[max_index]
            << ") / 2 = " << average << endl;
    }
    else {
        cout << "Условие НЕ выполняется" << endl;
        cout << "Минимальный элемент на месте: " << min_index + 1 << endl;
        cout << "Максимальный элемент на месте: " << max_index + 1 << endl;
    }

    return 0;
}