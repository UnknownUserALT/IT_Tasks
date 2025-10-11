#include <iostream>
using namespace std;
int saddlepoints() {
    setlocale(LC_ALL, "ru");
    int N, M;
    cout << "������� ���������� ����� N � �������� M: ";
    cin >> N >> M;

    int matrix[100][100];

    cout << "������� ������� " << N << "x" << M << " (���������):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> matrix[i][j];
        }
    }

    // ������� �������� �������
    cout << "\n�������� �������:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // ������� ������� ��� �������� min/max �� ������� � ��������
    int rowMin[100], rowMax[100];
    int colMin[100], colMax[100];

    // �������������� �������
    for (int i = 0; i < N; i++) {
        rowMin[i] = matrix[i][0];
        rowMax[i] = matrix[i][0];
    }

    for (int j = 0; j < M; j++) {
        colMin[j] = matrix[0][j];
        colMax[j] = matrix[0][j];
    }

    // ��������� min � max ��� ������ ������
    cout << "\n��������� �������� � ���������:" << endl;
    cout << "�� �������:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] < rowMin[i]) {
                rowMin[i] = matrix[i][j];
            }
            if (matrix[i][j] > rowMax[i]) {
                rowMax[i] = matrix[i][j];
            }
        }
        cout << "������ " << i + 1 << ": min = " << rowMin[i] << ", max = " << rowMax[i] << endl;
    }

    // ��������� min � max ��� ������� �������
    cout << "\n�� ��������:" << endl;
    for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {
            if (matrix[i][j] < colMin[j]) {
                colMin[j] = matrix[i][j];
            }
            if (matrix[i][j] > colMax[j]) {
                colMax[j] = matrix[i][j];
            }
        }
        cout << "������� " << j + 1 << ": min = " << colMin[j] << ", max = " << colMax[j] << endl;
    }

    // ���� �������� �����
    bool foundAny = false;

    cout << "\n=== ����� �������� ����� ===" << endl;

    // ����� ����� ���� A: ���������� � ������ � ����������� � �������
    cout << "\n��� A (min � ������, max � �������):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] == rowMin[i] && matrix[i][j] == colMax[j]) {
                cout << "A " << matrix[i][j] << " " << i + 1 << " " << j + 1 << endl;
                foundAny = true;
            }
        }
    }

    // ����� ����� ���� B: ����������� � ������ � ���������� � �������
    cout << "\n��� B (max � ������, min � �������):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] == rowMax[i] && matrix[i][j] == colMin[j]) {
                cout << "B " << matrix[i][j] << " " << i + 1 << " " << j + 1 << endl;
                foundAny = true;
            }
        }
    }

    if (!foundAny) {
        cout << "NONE" << endl;
    }

    return 0;
}