#include <iostream>
using namespace std;
int magiccub() {
    setlocale(LC_ALL, "ru");
    int N;
    cout << "������� ������ ������� N: ";
    cin >> N;

    int matrix[50][50];

    cout << "������� ������� " << N << "x" << N << " (���������):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> matrix[i][j];
        }
    }

    // ������� �������� �������
    cout << "\n�������� �������:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // ��������� ��������� ����� (������ ������)
    int magicSum = 0;
    for (int j = 0; j < N; j++) {
        magicSum += matrix[0][j];
    }

    cout << "\n��������� ����� (�� ������ ������): " << magicSum << endl;

    // ��������� ����� ���� �����
    bool isMagic = true;

    cout << "\n�������� �����:" << endl;
    for (int i = 0; i < N && isMagic; i++) {
        int rowSum = 0;
        for (int j = 0; j < N; j++) {
            rowSum += matrix[i][j];
        }
        cout << "������ " << i + 1 << ": ����� = " << rowSum;
        if (rowSum != magicSum) {
            cout << " - �� ���������!" << endl;
            isMagic = false;
        }
        else {
            cout << " - OK" << endl;
        }
    }

    // ��������� ����� ���� ��������
    if (isMagic) {
        cout << "\n�������� ��������:" << endl;
        for (int j = 0; j < N && isMagic; j++) {
            int colSum = 0;
            for (int i = 0; i < N; i++) {
                colSum += matrix[i][j];
            }
            cout << "������� " << j + 1 << ": ����� = " << colSum;
            if (colSum != magicSum) {
                cout << " - �� ���������!" << endl;
                isMagic = false;
            }
            else {
                cout << " - OK" << endl;
            }
        }
    }

    // ��������� ������� ���������
    if (isMagic) {
        cout << "\n�������� ����������:" << endl;
        int mainDiagSum = 0;
        for (int i = 0; i < N; i++) {
            mainDiagSum += matrix[i][i];
        }
        cout << "������� ���������: ����� = " << mainDiagSum;
        if (mainDiagSum != magicSum) {
            cout << " - �� ���������!" << endl;
            isMagic = false;
        }
        else {
            cout << " - OK" << endl;
        }
    }

    // ��������� �������� ���������
    if (isMagic) {
        int secondaryDiagSum = 0;
        for (int i = 0; i < N; i++) {
            secondaryDiagSum += matrix[i][N - 1 - i];
        }
        cout << "�������� ���������: ����� = " << secondaryDiagSum;
        if (secondaryDiagSum != magicSum) {
            cout << " - �� ���������!" << endl;
            isMagic = false;
        }
        else {
            cout << " - OK" << endl;
        }
    }

    // ������� ���������
    cout << "\n=== ��������� ===" << endl;
    if (isMagic) {
        cout << "YES - ��� ���������� �������!" << endl;
        cout << "����� �����: " << magicSum << endl;
    }
    else {
        cout << "NO - ��� �� ���������� �������" << endl;
    }

    return 0;
}