#include<iostream>
#include <cmath>

using namespace std;

int DiagPr()
{
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3, x4, y4;

    cout << "������� ���������� 4 ������ ��������������:" << endl;
    cout << "������� 1 (x y): ";
    cin >> x1 >> y1;
    cout << "������� 2 (x y): ";
    cin >> x2 >> y2;
    cout << "������� 3 (x y): ";
    cin >> x3 >> y3;
    cout << "������� 4 (x y): ";
    cin >> x4 >> y4;

    // ��������� ����� ��������� 1 � 3 (���������������)
    double dx = x3 - x1;
    double dy = y3 - y1;
    double diagonal = sqrt(dx * dx + dy * dy);

    cout << "����� ���������: " << diagonal << endl;

    return 0;
}