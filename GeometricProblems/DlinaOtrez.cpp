#include <iostream>
#include <cmath>

using namespace std;

int DlinOtr() {
    setlocale(LC_ALL, "ru");

    int points[2][2]; // ��� �����, ������ � ����� ������������

    cout << "������� ���������� ������ ����� (x y): ";
    cin >> points[0][0] >> points[0][1];

    cout << "������� ���������� ������ ����� (x y): ";
    cin >> points[1][0] >> points[1][1];

    // ��������� �������� ���������
    int dx = points[1][0] - points[0][0];
    int dy = points[1][1] - points[0][1];

    // ��������� ����� �� ������� ��������
    double length = sqrt(dx * dx + dy * dy);

    cout << "���������� �����: (" << points[0][0] << "," << points[0][1]
        << ") � (" << points[1][0] << "," << points[1][1] << ")" << endl;
    cout << "����� �������: " << length << endl;

    return 0;
}