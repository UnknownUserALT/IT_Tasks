#include <iostream>
using namespace std;

int CmToM() {
    int cm;

    // ���� ����� � �����������
    cout << "������� ����� � �����������: ";
    cin >> cm;

    // �������� �� ������������� ��������
    if (cm < 0) {
        cout << "������: ����� �� ����� ���� �������������!" << endl;
        return 1;
    }

    // ���������� ������ � �����������
    int meters = cm / 100;      // ������ �����
    int centimeters = cm % 100; // ���������� ����������

    // ����� ����������
    cout << cm << " �� = " << meters << " � " << centimeters << " ��" << endl;

    return 0;
}