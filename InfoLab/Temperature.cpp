#include <iostream>
using namespace std;

int temp() {
    double temperature;

    cout << "Введите температуру в градусах Цельсия: ";
    cin >> temperature;

    cout << "Рекомендация по одежде: ";

    if (temperature < 0) {
        cout << "наденьте зимнюю одежду" << endl;
    }
    else if (temperature >= 0 && temperature <= 10) {
        cout << "наденьте тёплую одежду" << endl;
    }
    else if (temperature >= 11 && temperature <= 20) {
        cout << "наденьте лёгкую одежду" << endl;
    }
    else if (temperature > 20) {
        cout << "наденьте летнюю одежду" << endl;
    }

    return 0;
}