#include <iostream>
using namespace std;

int stopcount()
{
    int stops;
    int total�ost = 0;
    const int pricePerStop = 5;

    cout << "������� ���������� ���������: ";
    cin >> stops;


    for (int i = 0; i < stops; i++) {
        total�ost += pricePerStop;
    }

    cout << "����� ��������� �������: " << total�ost << " ������" << endl;

    return 0;
}