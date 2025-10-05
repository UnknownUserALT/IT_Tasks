#include<iostream>

using namespace std;

int max()
{
    setlocale(LC_ALL, "ru");
    const int n = 10;
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
    if (max_index == 1 && min_index == 4) {
        // �������� �������� �� ������������ �� ����� �� ������������
        int max_value = numbers[max_index];
        for (int i = min_index; i < n; i++) {
            numbers[i] = max_value;
        }

        cout << "������� �����������!" << endl;
        cout << "������������ �������: " << max_value << " �� 2 �����" << endl;
        cout << "����������� ������� ���: " << numbers[min_index] << " �� 5 �����" << endl;
    }
    else {
        cout << "������� �� �����������" << endl;
        cout << "������������ ������� �� �����: " << max_index + 1 << endl;
        cout << "����������� ������� �� �����: " << min_index + 1 << endl;
    }

    // ����� �������
    cout << "���������:" << endl;
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }

    return 0;
}