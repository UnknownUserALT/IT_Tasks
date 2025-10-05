#include<iostream>

using namespace std;

int chpl()
{
    setlocale(LC_ALL, "ru");

    const int n = 15;
    double numbers[n];

    // Ввод последовательности
    cout << "Введите " << n << " вещественных чисел:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    // Поиск индекса наименьшего числа
    int min_index = 0;
    for (int i = 1; i < n; i++) {
        if (numbers[i] < numbers[min_index]) {
            min_index = i;
        }
    }

    // Поиск индекса наибольшего числа
    int max_index = 0;
    for (int i = 1; i < n; i++) {
        if (numbers[i] > numbers[max_index]) {
            max_index = i;
        }
    }

    // Меняем наименьшее с первым
    double temp = numbers[0];
    numbers[0] = numbers[min_index];
    numbers[min_index] = temp;

    // Меняем наибольшее с последним
    temp = numbers[n - 1];
    numbers[n - 1] = numbers[max_index];
    numbers[max_index] = temp;

    // Вывод результата
    cout << "Результат:" << endl;
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }

    return 0;
}