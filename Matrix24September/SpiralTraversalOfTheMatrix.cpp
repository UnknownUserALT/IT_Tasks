#include <iostream>
#include <iomanip>
using namespace std;
int spiraltraversal() {
    setlocale(LC_ALL, "ru");
    int N, M;
    cout << "������� ������� ������� N M: ";
    cin >> N >> M;

    int matrix[100][100]; // ������������ ������ 100x100

    cout << "������� �������� �������:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> matrix[i][j];
        }
    }

    int top = 0, bottom = N - 1;
    int left = 0, right = M - 1;

    cout << "����� �� �������: ";

    while (top <= bottom && left <= right) {
        // ������
        for (int j = left; j <= right; j++) {
            cout << matrix[top][j] << " ";
        }
        top++;

        // ����
        for (int i = top; i <= bottom; i++) {
            cout << matrix[i][right] << " ";
        }
        right--;

        // �����
        if (top <= bottom) {
            for (int j = right; j >= left; j--) {
                cout << matrix[bottom][j] << " ";
            }
            bottom--;
        }

        // �����
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                cout << matrix[i][left] << " ";
            }
            left++;
        }
    }

    cout << endl;

    return 0;
}