#include <iostream>
using namespace std;

int temp() {
    double temperature;

    cout << "������� ����������� � �������� �������: ";
    cin >> temperature;

    cout << "������������ �� ������: ";

    if (temperature < 0) {
        cout << "�������� ������ ������" << endl;
    }
    else if (temperature >= 0 && temperature <= 10) {
        cout << "�������� ����� ������" << endl;
    }
    else if (temperature >= 11 && temperature <= 20) {
        cout << "�������� ����� ������" << endl;
    }
    else if (temperature > 20) {
        cout << "�������� ������ ������" << endl;
    }

    return 0;
}