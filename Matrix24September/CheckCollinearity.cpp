#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
int chekcollinearity() {
    setlocale(LC_ALL, "ru");
    double x1, y1, x2, y2, x3, y3;

    // ���� ��������� ���� �����
    cout << "������� ���������� ����� A (x1 y1): ";
    cin >> x1 >> y1;

    cout << "������� ���������� ����� B (x2 y2): ";
    cin >> x2 >> y2;

    cout << "������� ���������� ����� C (x3 y3): ";
    cin >> x3 >> y3;

    // ���������� ������������ ������� 3x3
    // | x1  y1  1 |
    // | x2  y2  1 |
    // | x3  y3  1 |
    double determinant = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);

    // �������� ��������������
    // ���������� ��������� ������� ��� ����� ����������� ���������� � ��������� ������
    const double epsilon = 1e-10;

    cout << fixed << setprecision(10);
    cout << "������������ �������: " << determinant << endl;

    if (abs(determinant) < epsilon) {
        cout << "����� ����������� (����� �� ����� ������)" << endl;
    }
    else {
        cout << "����� �� �����������" << endl;
    }

    return 0;
}