#include <iostream>
using namespace std;

int div() {
    int number;

    // ���� �����
    cout << "������� ����� �����: ";
    cin >> number;

    // ����� ����������
    cout << "����� " << number;

    if (number % 3 == 0 && number % 5 == 0) {
        cout << " ������� �� 3 � �� 5 ������������" << endl;
    }
    else if (number % 3 == 0) {
        cout << " ������� �� 3" << endl;
    }
    else if (number % 5 == 0) {
        cout << " ������� �� 5" << endl;
    }
    else {
        cout << " �� ������� �� �� 3, �� �� 5" << endl;
    }

    return 0;
}