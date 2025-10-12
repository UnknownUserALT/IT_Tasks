#include <iostream>
#include <iomanip>
using namespace std;
int systemofequations7() {
    setlocale(LC_ALL, "ru");
    // ������� ���������:
  // x + y + z = 6
  // 2x - y + 3z = 14
  // -x + 4y - z = 2

  // ������� ������������� � ��������� ������
    double a[3][4] = {
        {1,  1,  1, 6},
        {2, -1,  3, 14},
        {-1, 4, -1, 2}
    };

    // ������ ��� ������ ������
    for (int i = 0; i < 3; i++) {
        // ����� ������������� �������� � ������� (��� ������������)
        int maxRow = i;
        for (int k = i + 1; k < 3; k++) {
            if (abs(a[k][i]) > abs(a[maxRow][i])) {
                maxRow = k;
            }
        }

        // ������������ �����
        for (int k = i; k < 4; k++) {
            double tmp = a[maxRow][k];
            a[maxRow][k] = a[i][k];
            a[i][k] = tmp;
        }

        // ���������� � ������������ ����
        for (int k = i + 1; k < 3; k++) {
            double coef = a[k][i] / a[i][i];
            for (int j = i; j < 4; j++) {
                a[k][j] -= coef * a[i][j];
            }
        }
    }

    // �������� ���
    double x[3];
    for (int i = 2; i >= 0; i--) {
        x[i] = a[i][3];
        for (int j = i + 1; j < 3; j++) {
            x[i] -= a[i][j] * x[j];
        }
        x[i] /= a[i][i];
    }

    cout << "������� ������� ���������:" << endl;
    cout << "x + y + z = 6" << endl;
    cout << "2x - y + 3z = 14" << endl;
    cout << "-x + 4y - z = 2" << endl;
    cout << endl;

    cout << fixed << setprecision(2);
    cout << "x = " << x[0] << endl;
    cout << "y = " << x[1] << endl;
    cout << "z = " << x[2] << endl;

    // �������� �������
    cout << endl << "��������:" << endl;
    cout << x[0] << " + " << x[1] << " + " << x[2] << " = "
        << (x[0] + x[1] + x[2]) << endl;
    cout << "2*" << x[0] << " - " << x[1] << " + 3*" << x[2] << " = "
        << (2 * x[0] - x[1] + 3 * x[2]) << endl;
    cout << "-" << x[0] << " + 4*" << x[1] << " - " << x[2] << " = "
        << (-x[0] + 4 * x[1] - x[2]) << endl;

    return 0;
}