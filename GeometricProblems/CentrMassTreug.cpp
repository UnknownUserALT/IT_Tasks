#include<iostream>
//#include <cmath>

using namespace std;

int CentrM()
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

    double center_x = (x1 + x2 + x3) / 3;
    double center_y = (y1 + y2 + y3) / 3;

    cout << "����� ���� ������������: (" << center_x << ", " << center_y << ")" << endl;

    return 0;

}