#include<iostream>

using namespace std;

int chpl()
{
    setlocale(LC_ALL, "ru");

    const int n = 15;
    double numbers[n];

    // ���� ������������������
    cout << "������� " << n << " ������������ �����:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    // ����� ������� ����������� �����
    int min_index = 0;
    for (int i = 1; i < n; i++) {
        if (numbers[i] < numbers[min_index]) {
            min_index = i;
        }
    }

    // ����� ������� ����������� �����
    int max_index = 0;
    for (int i = 1; i < n; i++) {
        if (numbers[i] > numbers[max_index]) {
            max_index = i;
        }
    }

    // ������ ���������� � ������
    double temp = numbers[0];
    numbers[0] = numbers[min_index];
    numbers[min_index] = temp;

    // ������ ���������� � ���������
    temp = numbers[n - 1];
    numbers[n - 1] = numbers[max_index];
    numbers[max_index] = temp;

    // ����� ����������
    cout << "���������:" << endl;
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }

    return 0;
}