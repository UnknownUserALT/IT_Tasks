#include <iostream>
using namespace std;

int prover() {
    double number;
    cout << "������� �����: ";
    cin >> number;

    if (number > 0) {
        cout << "����� �������������." << endl;
    }
    else if (number < 0) {
        cout << "����� �������������." << endl;
    }
    else {
        cout << "����� ����� ����." << endl;
    }

    return 0;
}