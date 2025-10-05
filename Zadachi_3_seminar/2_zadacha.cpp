#include <iostream>
using namespace std;

int naib()
{
    setlocale(LC_ALL, "Russian");
    const int n = 3;

    int numbers[n];
    int even_count = 0;
    int max_odd = 0;
    bool has_odd = false;

    // ������ �����
    cout << "������� " << n << " �����:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    // ��������� ������ �����
    for (int i = 0; i < n; i++) {
        // ���� ����� ������
        if (numbers[i] % 2 == 0) {
            even_count++;
        }
        // ���� ����� ��������
        else {
            // ���� ��� ������ �������� ����� ��� ��� ������ ����������� ���������
            if (!has_odd || numbers[i] > max_odd) {
                max_odd = numbers[i];
                has_odd = true;
            }
        }
    }

    // ������� ����������
    cout << "���������� ������ �����: " << even_count << endl;
    if (has_odd) {
        cout << "���������� �������� �����: " << max_odd << endl;
    }
    else {
        cout << "�������� ����� ���" << endl;
    }

    return 0;
}