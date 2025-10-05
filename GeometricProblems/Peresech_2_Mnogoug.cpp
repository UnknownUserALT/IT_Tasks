#include <iostream>
#include <cmath>
using namespace std;
int PerMnogoug() {
    setlocale(LC_ALL, "ru");
    // ������ �������������
    int n1;
    double x1[10]; // ���������� x
    double y1[10]; // ���������� y

    cout << "������� ���������� ������ ������� �������������� (3-10): ";
    cin >> n1;

    cout << "������� ���������� ������ ������� ��������������:" << endl;
    for (int i = 0; i < n1; i++) {
        cout << "������� " << i + 1 << " (x y): ";
        cin >> x1[i] >> y1[i];
    }

    // ������ �������������
    int n2;
    double x2[10];
    double y2[10];

    cout << "\n������� ���������� ������ ������� �������������� (3-10): ";
    cin >> n2;

    cout << "������� ���������� ������ ������� ��������������:" << endl;
    for (int i = 0; i < n2; i++) {
        cout << "������� " << i + 1 << " (x y): ";
        cin >> x2[i] >> y2[i];
    }

    // ���� �����������
    bool peresekayutsya = false;

    // �������� �� ���� �������� ������� ��������������
    for (int i = 0; i < n1; i++) {
        // ���������� ������� ������� AB (������� ������� ��������������)
        double ax = x1[i];
        double ay = y1[i];
        double bx = x1[(i + 1) % n1]; // ��������� ������� (�������� �� ������)
        double by = y1[(i + 1) % n1];

        // �������� �� ���� �������� ������� ��������������
        for (int j = 0; j < n2; j++) {
            // ���������� ������� ������� CD
            double cx = x2[j];
            double cy = y2[j];
            double dx = x2[(j + 1) % n2];
            double dy = y2[(j + 1) % n2];

            // ��������� ���������� ��� �������� ����������� �������� AB � CD
            // ���������� ����������, ��� ��������� ����� ������������ �������

            // ���������� ������ (A, B, C)
            double o1 = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);

            // ���������� ������ (A, B, D)
            double o2 = (bx - ax) * (dy - ay) - (by - ay) * (dx - ax);

            // ���������� ������ (C, D, A)
            double o3 = (dx - cx) * (ay - cy) - (dy - cy) * (ax - cx);

            // ���������� ������ (C, D, B)
            double o4 = (dx - cx) * (by - cy) - (dy - cy) * (bx - cx);

            // ����� ������: ������� ������������, ���� ����� C � D
            // ��������� �� ������ ������� �� AB, � A � B �� ������ ������� �� CD
            if (o1 * o2 < 0 && o3 * o4 < 0) {
                peresekayutsya = true;
                break;
            }

            // ����������� ������: ����� ����� �� ����� ������
            // ���������, ����� �� ����� �� �������
            if (o1 == 0) {
                // C ����� �� AB, ���������, ������ �� �������
                if (cx >= min(ax, bx) && cx <= max(ax, bx) &&
                    cy >= min(ay, by) && cy <= max(ay, by)) {
                    peresekayutsya = true;
                    break;
                }
            }
            if (o2 == 0) {
                // D ����� �� AB
                if (dx >= min(ax, bx) && dx <= max(ax, bx) &&
                    dy >= min(ay, by) && dy <= max(ay, by)) {
                    peresekayutsya = true;
                    break;
                }
            }
            if (o3 == 0) {
                // A ����� �� CD
                if (ax >= min(cx, dx) && ax <= max(cx, dx) &&
                    ay >= min(cy, dy) && ay <= max(cy, dy)) {
                    peresekayutsya = true;
                    break;
                }
            }
            if (o4 == 0) {
                // B ����� �� CD
                if (bx >= min(cx, dx) && bx <= max(cx, dx) &&
                    by >= min(cy, dy) && by <= max(cy, dy)) {
                    peresekayutsya = true;
                    break;
                }
            }
        }
        if (peresekayutsya) {
            break;
        }
    }

    // ������� ���������
    cout << "\n";
    if (peresekayutsya) {
        cout << "�������������� ������������!" << endl;
    }
    else {
        cout << "�������������� �� ������������." << endl;
    }

    return 0;
}