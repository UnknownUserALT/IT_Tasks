
#include <iostream>
using namespace std;

int arearec() {
    double length, width;

    // ���� ����� � ������
    cout << "������� ����� ��������������: ";
    cin >> length;

    cout << "������� ������ ��������������: ";
    cin >> width;

    // �������� �� ������������ �����
    if (length <= 0 || width <= 0) {
        cout << "������: ����� � ������ ������ ���� �������������� �������!" << endl;
        return 1;
    }

    // ���������� �������
    double area = length * width;

    // ����� ����������
    cout << "������� ��������������: " << area << endl;

    return 0;
}