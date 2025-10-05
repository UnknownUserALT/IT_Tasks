#include<iostream>

using namespace std;

int obnul()
{
    setlocale(LC_ALL, "ru");

    const int n = 15;
    int numbers[n];

    // ���� �������
    cout << "������� " << n << " ����� �����:" << endl;
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
    if (numbers[min_index] == 0 || numbers[max_index] == 0) {
        // �������� ��������, �������������� ������������
        for (int i = 0; i < min_index; i++) {
            numbers[i] = 0;
        }

        cout << "������� �����������! �������� �������� ����� �����������:" << endl;
        cout << "����������� �������: " << numbers[min_index] << " �� ����� " << min_index + 1 << endl;
        cout << "������������ �������: " << numbers[max_index] << " �� ����� " << max_index + 1 << endl;
    }
    else {
        cout << "������� �� �����������" << endl;
        cout << "����������� �������: " << numbers[min_index] << endl;
        cout << "������������ �������: " << numbers[max_index] << endl;
    }

    // ����� �������
    cout << "���������:" << endl;
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }

    return 0;
}