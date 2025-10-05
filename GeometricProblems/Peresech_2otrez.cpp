#include<iostream>
#include <cmath>

using namespace std;

int DvaOtrez()
{
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3, x4, y4;

    cout << "������� ���������� ���� ��������:" << endl;
    cout << "������� 1 - A(x1 y1) B(x2 y2): ";
    cin >> x1 >> y1 >> x2 >> y2;
    cout << "������� 2 - C(x3 y3) D(x4 y4): ";
    cin >> x3 >> y3 >> x4 >> y4;

    // ������� ��� ���������� ������������
    auto cross = [](double x1, double y1, double x2, double y2, double x3, double y3) {
        return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
        };

    double cp1 = cross(x1, y1, x2, y2, x3, y3);
    double cp2 = cross(x1, y1, x2, y2, x4, y4);
    double cp3 = cross(x3, y3, x4, y4, x1, y1);
    double cp4 = cross(x3, y3, x4, y4, x2, y2);

    // �������� �����������
    if (((cp1 * cp2) < 0) && ((cp3 * cp4) < 0)) {
        cout << "������� ������������" << endl;
    }
    else {
        cout << "������� �� ������������" << endl;
    }

    return 0;
}