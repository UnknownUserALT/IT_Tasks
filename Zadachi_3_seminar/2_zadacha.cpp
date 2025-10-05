#include <iostream>
using namespace std;

int naib()
{
    setlocale(LC_ALL, "Russian");
    const int n = 3;

    int numbers[n];
    int even_count = 0;
    int max_odd = 0;
    bool has_odd = false;

    // Вводим числа
    cout << "Введите " << n << " чисел:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    // Проверяем каждое число
    for (int i = 0; i < n; i++) {
        // Если число четное
        if (numbers[i] % 2 == 0) {
            even_count++;
        }
        // Если число нечетное
        else {
            // Если это первое нечетное число или оно больше предыдущего максимума
            if (!has_odd || numbers[i] > max_odd) {
                max_odd = numbers[i];
                has_odd = true;
            }
        }
    }

    // Выводим результаты
    cout << "Количество четных чисел: " << even_count << endl;
    if (has_odd) {
        cout << "Наибольшее нечетное число: " << max_odd << endl;
    }
    else {
        cout << "Нечетных чисел нет" << endl;
    }

    return 0;
}