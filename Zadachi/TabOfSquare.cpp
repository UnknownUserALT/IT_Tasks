#include <iostream>
using namespace std;

int tabsq() {
    int n;

    // ���� ����� n
    cout << "������� ����� n: ";
    cin >> n;

    // �������� ������������ �����
    if (n <= 0) {
        cout << "������: n ������ ���� ������������� ������!" << endl;
        return 1;
    }

    // ����� ��������� ����� �� 1 �� n
    cout << "�������� ����� �� 1 �� " << n << ":" << endl;
    for (int i = 1; i <= n; i++) {
        cout << i << "^2 = " << i * i << endl;
    }

    return 0;
}
