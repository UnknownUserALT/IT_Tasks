#include <iostream>
using namespace std;

int PrToch() {
    setlocale(LC_ALL, "ru");

    // ���������� ������ �������������� (�������)
    int x[] = { 0, 10, 10, 0 };
    int y[] = { 0, 0, 10, 10 };
    int n = 4; // ���������� ������

    // ����������� �����
    int px, py;
    cout << "������� ���������� ����� (x y): ";
    cin >> px >> py;

    bool inside = false;

    // �������� �� ���� �������� ��������������
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n; // ��������� �������

        // ��������� ����������� ���� �� ��������
        if ((y[i] > py) != (y[j] > py)) { // ��� ���������� ������� �� Y
            // ��������� X-���������� �����������
            double intersectX = (x[j] - x[i]) * (py - y[i]) / (double)(y[j] - y[i]) + x[i];

            if (px < intersectX) {
                inside = !inside; // ������ ���������
            }
        }
    }

    if (inside) {
        cout << "����� ������ ��������������" << endl;
    }
    else {
        cout << "����� ������� ��������������" << endl;
    }

    return 0;
}