#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
int areatriangle() {
    setlocale(LC_ALL, "ru");

    double x1, y1, x2, y2, x3, y3;

    // ���� ��������� ���� �����
    cout << "������� ���������� ����� A (x1 y1): ";
    cin >> x1 >> y1;

    cout << "������� ���������� ����� B (x2 y2): ";
    cin >> x2 >> y2;

    cout << "������� ���������� ����� C (x3 y3): ";
    cin >> x3 >> y3;

    // ������� ������� 3x3 � ��������� ������������
    // ������� ����� ���:
    // | x1  y1  1 |
    // | x2  y2  1 |
    // | x3  y3  1 |

    // ��������� ������������ �� �������:
    // det = x1*(y2 - y3) - y1*(x2 - x3) + 1*(x2*y3 - x3*y2)
    double determinant = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);

    // ������� ����� �������� ������ ������������
    double area = 0.5 * abs(determinant);

    // ����� ����������
    cout << fixed << setprecision(2);
    cout << "������� ������������: " << area << endl;

    return 0;
}