#include<iostream>
#include <cmath>

using namespace std;

int pmnogoug()
{
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3, x4, y4;

    cout << "������� ���������� 4 ������:" << endl;
    cout << "������� 1 (x y): ";
    cin >> x1 >> y1;
    cout << "������� 2 (x y): ";
    cin >> x2 >> y2;
    cout << "������� 3 (x y): ";
    cin >> x3 >> y3;
    cout << "������� 4 (x y): ";
    cin >> x4 >> y4;

    // ��������� ���������� ����� ����� ��������� ���������
    double d1 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    double d2 = sqrt((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));
    double d3 = sqrt((x4 - x3) * (x4 - x3) + (y4 - y3) * (y4 - y3));
    double d4 = sqrt((x1 - x4) * (x1 - x4) + (y1 - y4) * (y1 - y4));

    double perimeter = d1 + d2 + d3 + d4;

    cout << "��������: " << perimeter << endl;

    return 0;

}