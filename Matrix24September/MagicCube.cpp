#include <iostream>
using namespace std;
int magiccub() {
    setlocale(LC_ALL, "ru");
    int N;
    cout << "Введите размер матрицы N: ";
    cin >> N;

    int matrix[50][50];

    cout << "Введите матрицу " << N << "x" << N << " (построчно):" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> matrix[i][j];
        }
    }

    // Выводим исходную матрицу
    cout << "\nИсходная матрица:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // Вычисляем эталонную сумму (первая строка)
    int magicSum = 0;
    for (int j = 0; j < N; j++) {
        magicSum += matrix[0][j];
    }

    cout << "\nЭталонная сумма (из первой строки): " << magicSum << endl;

    // Проверяем суммы всех строк
    bool isMagic = true;

    cout << "\nПроверка строк:" << endl;
    for (int i = 0; i < N && isMagic; i++) {
        int rowSum = 0;
        for (int j = 0; j < N; j++) {
            rowSum += matrix[i][j];
        }
        cout << "Строка " << i + 1 << ": сумма = " << rowSum;
        if (rowSum != magicSum) {
            cout << " - НЕ СОВПАДАЕТ!" << endl;
            isMagic = false;
        }
        else {
            cout << " - OK" << endl;
        }
    }

    // Проверяем суммы всех столбцов
    if (isMagic) {
        cout << "\nПроверка столбцов:" << endl;
        for (int j = 0; j < N && isMagic; j++) {
            int colSum = 0;
            for (int i = 0; i < N; i++) {
                colSum += matrix[i][j];
            }
            cout << "Столбец " << j + 1 << ": сумма = " << colSum;
            if (colSum != magicSum) {
                cout << " - НЕ СОВПАДАЕТ!" << endl;
                isMagic = false;
            }
            else {
                cout << " - OK" << endl;
            }
        }
    }

    // Проверяем главную диагональ
    if (isMagic) {
        cout << "\nПроверка диагоналей:" << endl;
        int mainDiagSum = 0;
        for (int i = 0; i < N; i++) {
            mainDiagSum += matrix[i][i];
        }
        cout << "Главная диагональ: сумма = " << mainDiagSum;
        if (mainDiagSum != magicSum) {
            cout << " - НЕ СОВПАДАЕТ!" << endl;
            isMagic = false;
        }
        else {
            cout << " - OK" << endl;
        }
    }

    // Проверяем побочную диагональ
    if (isMagic) {
        int secondaryDiagSum = 0;
        for (int i = 0; i < N; i++) {
            secondaryDiagSum += matrix[i][N - 1 - i];
        }
        cout << "Побочная диагональ: сумма = " << secondaryDiagSum;
        if (secondaryDiagSum != magicSum) {
            cout << " - НЕ СОВПАДАЕТ!" << endl;
            isMagic = false;
        }
        else {
            cout << " - OK" << endl;
        }
    }

    // Выводим результат
    cout << "\n=== РЕЗУЛЬТАТ ===" << endl;
    if (isMagic) {
        cout << "YES - это магический квадрат!" << endl;
        cout << "Общая сумма: " << magicSum << endl;
    }
    else {
        cout << "NO - это не магический квадрат" << endl;
    }

    return 0;
}