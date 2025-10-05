// AlgorithmsLab.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    int const size = 10;
    int arr[size];

    // Заполнение массива
    cout << "Введите 10 целых чисел:" << endl;
    for (int i = 0; i < size; i++) {
        cout << "Элемент " << i + 1 << ": ";
        cin >> arr[i];
    }

    // Вывод исходного массива
    cout << "\nИсходный массив: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Сумма элементов
    int sum = 0;   // 22
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    cout << "Сумма элементов: " << sum << endl;

    // Поиск минимального элемента
    int minElement = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < minElement) {
            minElement = arr[i];
        }
    }
    cout << "Минимальный элемент: " << minElement << endl;

    // Сортировка вставками
    for (int i = 1; i < size; i++) {
        // Записываем текущее значение
        int сurrentValue = arr[i];
        // Создаем обратный счетчик
        int j = i - 1;

        // Сдвигаем элементы больше сurrentValue вправо
        while (j >= 0 && arr[j] > сurrentValue) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = сurrentValue;
    }

    // Вывод отсортированного массива
    cout << "Отсортированный массив: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;

}


