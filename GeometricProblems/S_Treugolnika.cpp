#include<iostream>
#include <cmath>

using namespace std;

int streug()
{
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3;

    cout << "������� ���������� ���� ������ ������������:" << endl;
    cout << "������� 1 (x y): ";
    cin >> x1 >> y1;
    cout << "������� 2 (x y): ";
    cin >> x2 >> y2;
    cout << "������� 3 (x y): ";
    cin >> x3 >> y3;

    double area = 0.5 * abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

    cout << "������� ������������: " << area << endl;

    return 0;

}