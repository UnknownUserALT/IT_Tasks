#include<iostream>

using namespace std;

int umen()
{
    setlocale(LC_ALL, "ru");

    const int n = 15;
    double numbers[n];

    // ���� �������
    cout << "������� " << n << " ������������ �����:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    // ����� �������� ������������ � ������������� ���������
    int min_index = 0;
    int max_index = 0;

    for (int i = 1; i < n; i++) {
        if (numbers[i] < numbers[min_index]) {
            min_index = i;
        }
        if (numbers[i] > numbers[max_index]) {
            max_index = i;
        }
    }

    // ��������� �������
    if (min_index == 1 && max_index == 0) {
        // ��������� ��� �������� � 2 ����
        for (int i = 0; i < n; i++) {
            numbers[i] = numbers[i] / 2.0;
        }

        cout << "������� �����������! ��� �������� ��������� � 2 ����:" << endl;
    }
    else {
        cout << "������� �� �����������" << endl;
        cout << "����������� ������� �� �����: " << min_index + 1 << endl;
        cout << "������������ ������� �� �����: " << max_index + 1 << endl;
    }

    // ����� �������
    cout << "���������:" << endl;
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }

    return 0;
}