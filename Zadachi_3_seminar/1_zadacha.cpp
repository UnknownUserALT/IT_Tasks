#include <iostream>
using namespace std;

int kol()
{
    setlocale(LC_ALL, "Russian");
    const int n = 5; // ���������� �����
    int A[n];
    int count = 0;

    // ���� ������������������
    cout << "������� " << n << " ����������� �����:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    // �������� ������� Ak > 2^k
    for (int k = 0; k < n; k++) {
        int power = pow(2, k + 1); // 2^(k+1)
        if (A[k] > power) {
            count++;
            cout << "A" << (k + 1) << " = " << A[k] << " > 2^" << (k + 1)
                << " = " << power << endl;
        }
    }

    cout << "���������� ������, ��������������� �������: " << count << endl;

    return 0;
}