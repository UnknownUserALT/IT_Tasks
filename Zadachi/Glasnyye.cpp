#include <iostream>
using namespace std;

int glasn() {
    string input;
    int vowelCount = 0;

    // ���� ������
    cout << "������� ������: ";
    cin >> input;

    // ������� ������� ����
    for (char c : input) {
        // �������� ������ � ������� �������� ��� �������� ��������
        char lowerC = tolower(c);

        // ���������, �������� �� ������ ������� ������
        if (lowerC == 'a' || lowerC == 'e' || lowerC == 'i' ||
            lowerC == 'o' || lowerC == 'u' || lowerC == 'y') {
            vowelCount++;
        }
    }

    // ����� ����������
    cout << "���������� ������� ���� � ������: " << vowelCount << endl;

    return 0;
}