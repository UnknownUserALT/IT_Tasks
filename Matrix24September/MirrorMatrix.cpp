#include <iostream>
using namespace std;

int mirrorm() {
    setlocale(LC_ALL, "ru");

    int N;
    cout << "������� ������ ������� N: ";
    cin >> N;

    int A[100][100];

    // ������ �������� �������
    cout << "������� ������� " << N << "x" << N << " (���������):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> A[i][j];
        }
    }

    // ������� �������� �������
    cout << "\n�������� �������:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    // ������� 4 ������� ��� ��������������
    int m1[100][100], m2[100][100], m3[100][100], m4[100][100];

    // 1. ��������� ������������ ������� ��������� (����������������)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m1[i][j] = A[j][i];
        }
    }

    // 2. ��������� ������������ �������� ���������
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m2[i][j] = A[N - 1 - j][N - 1 - i];
        }
    }

    // 3. ���������� ��������� ������������ ������������ ���
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m3[i][j] = A[i][N - 1 - j];
        }
    }

    // 4. ���������� ��������� ������������ �������������� ���
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m4[i][j] = A[N - 1 - i][j];
        }
    }

    // ������� ��� 4 �������
    cout << "\n=== ���������� �������������� ===" << endl;

    cout << "\n1. ��������� ������������ ������� ���������:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << m1[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\n2. ��������� ������������ �������� ���������:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << m2[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\n3. ���������� ��������� ������������ ������������ ���:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << m3[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\n4. ���������� ��������� ������������ �������������� ���:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << m4[i][j] << " ";
        }
        cout << endl;
    }

    // ��������� ���������� ������
    cout << "\n=== ����������� ������� ===" << endl;
    bool found = false;

    // ��������� ��� ���� (i,j) ��� i < j
    for (int i = 1; i <= 4; i++) {
        for (int j = i + 1; j <= 4; j++) {
            bool equal = true;

            // ���������� ��������������� �������
            int (*mat1)[100], (*mat2)[100];

            if (i == 1) mat1 = m1;
            else if (i == 2) mat1 = m2;
            else if (i == 3) mat1 = m3;
            else mat1 = m4;

            if (j == 1) mat2 = m1;
            else if (j == 2) mat2 = m2;
            else if (j == 3) mat2 = m3;
            else mat2 = m4;

            // ������������ ���������
            for (int x = 0; x < N && equal; x++) {
                for (int y = 0; y < N && equal; y++) {
                    if (mat1[x][y] != mat2[x][y]) {
                        equal = false;
                    }
                }
            }

            if (equal) {
                cout << "������� " << i << " � " << j << " ���������" << endl;
                found = true;
            }
        }
    }

    if (!found) {
        cout << "NONE (����������� ������ ���)" << endl;
    }

    return 0;
}