#include <iostream>
using namespace std;

int conv() {
    double celsius;

    // ���� ����������� � ��������
    cout << "������� ����������� � �������� �������: ";
    cin >> celsius;

    // ����������� �� �������
    double fahrenheit = celsius * 9.0 / 5.0 + 32;

    // ����� ����������
    cout << celsius << " �C = " << fahrenheit << " �F" << endl;

    return 0;
}