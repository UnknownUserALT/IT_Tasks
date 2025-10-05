#include<iostream>

using namespace std;

int mult()
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

    // ��������� ������� � ��������� ������������
    if (max_index < min_index) {
        double product = numbers[min_index] * numbers[max_index];
        cout << "������������ ������� ������������ ������������" << endl;
        cout << "����������� �������: " << numbers[min_index] << " (������ " << min_index << ")" << endl;
        cout << "������������ �������: " << numbers[max_index] << " (������ " << max_index << ")" << endl;
        cout << "������������: " << numbers[min_index] << " * " << numbers[max_index] << " = " << product << endl;
    }
    else {
        cout << "������������ ������� �� ������������ ������������" << endl;
        cout << "����������� �������: " << numbers[min_index] << " (������ " << min_index << ")" << endl;
        cout << "������������ �������: " << numbers[max_index] << " (������ " << max_index << ")" << endl;
    }

    return 0;
}