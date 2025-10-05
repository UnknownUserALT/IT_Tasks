#include<iostream>
#include <cmath>

using namespace std;

int TriToch()
{
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3;

    // ���� ��������� ���� �����
    cout << "������� ���������� ���� �����:" << endl;
    cout << "����� 1 (x y): ";
    cin >> x1 >> y1;
    cout << "����� 2 (x y): ";
    cin >> x2 >> y2;
    cout << "����� 3 (x y): ";
    cin >> x3 >> y3;

    // ��������� ��������� ������������
    double cross_product = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);

    // ���������, ����� �� ��������� ������������ ���� (� ������ �����������)
    if (abs(cross_product) < 1e-9) {
        cout << "����� ����� �� ����� ������" << endl;
    }
    else {
        cout << "����� �� ����� �� ����� ������" << endl;
    }

    return 0;
}