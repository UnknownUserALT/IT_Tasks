#include <iostream>
using namespace std;

int chage() {
    int age;

    // ���� ��������
    cout << "������� ��� �������: ";
    cin >> age;

    // �������� ������������ �����
    if (age < 0 || age > 150) {
        cout << "������: ������� ������ ���� �� 0 �� 150 ���!" << endl;
        return 1;
    }

    // �������� ���������� ���������
    if (age < 18) {
        cout << "��� " << age << " ��� - �� ������������������" << endl;
    }
    else if (age >= 18 && age <= 60) {
        cout << "��� " << age << " ��� - �� �������� �������" << endl;
    }
    else {
        cout << "��� " << age << " ��� - �� ���������" << endl;
    }

    return 0;
}