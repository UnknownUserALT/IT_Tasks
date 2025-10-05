#include<iostream>

using namespace std;

int ariph()
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

    // ��������� ������� � ��������� ������� ��������������
    if (min_index == 4 && max_index == 9) { // ������� � 0, ������� 4 = 5-� �����, 9 = 10-� �����
        double average = (numbers[min_index] + numbers[max_index]) / 2.0;
        cout << "������� �����������!" << endl;
        cout << "����������� �������: " << numbers[min_index] << " �� 5 �����" << endl;
        cout << "������������ �������: " << numbers[max_index] << " �� 10 �����" << endl;
        cout << "������� ��������������: (" << numbers[min_index] << " + " << numbers[max_index]
            << ") / 2 = " << average << endl;
    }
    else {
        cout << "������� �� �����������" << endl;
        cout << "����������� ������� �� �����: " << min_index + 1 << endl;
        cout << "������������ ������� �� �����: " << max_index + 1 << endl;
    }

    return 0;
}