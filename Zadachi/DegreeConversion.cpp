#include <iostream>
using namespace std;

int conv() {
    double celsius;

    // Ввод температуры в Цельсиях
    cout << "Введите температуру в градусах Цельсия: ";
    cin >> celsius;

    // Конвертация по формуле
    double fahrenheit = celsius * 9.0 / 5.0 + 32;

    // Вывод результата
    cout << celsius << " °C = " << fahrenheit << " °F" << endl;

    return 0;
}