#include <iostream>
using namespace std;

int check() {
    int number;

    // ���� �����
    cout << "������� ����� �����: ";
    cin >> number;

    // �������� ��������
    if (number % 2 == 0) {
        cout << number << " - ������ �����" << endl;
    }
    else {
        cout << number << " - �������� �����" << endl;
    }

    return 0;
}