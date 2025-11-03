#include <iostream>

using namespace std;

// Функция для обмена указателей
void swapPointersFunc(int **a, int **b) {
    int *temp = *a;
    *a = *b;
    *b = temp;
}

int SwapPointers() {
    // Создание переменных
    int x = 10;
    int y = 20;
    
    // Объявление и инициализация указателей
    int *p1 = &x;
    int *p2 = &y;
    
    cout << "ДО вызова swapPointers:" << endl;
    cout << "p1: адрес = " << p1 << ", значение = " << *p1 << " (указывает на x)" << endl;
    cout << "p2: адрес = " << p2 << ", значение = " << *p2 << " (указывает на y)" << endl;
    cout << "x = " << x << ", адрес x = " << &x << endl;
    cout << "y = " << y << ", адрес y = " << &y << endl;
    cout << endl;
    
    // Вызов функции для обмена указателей
    swapPointersFunc(&p1, &p2);
    
    cout << "ПОСЛЕ вызова swapPointers:" << endl;
    cout << "p1: адрес = " << p1 << ", значение = " << *p1 << " (теперь указывает на y)" << endl;
    cout << "p2: адрес = " << p2 << ", значение = " << *p2 << " (теперь указывает на x)" << endl;
    cout << "x = " << x << ", адрес x = " << &x << endl;
    cout << "y = " << y << ", адрес y = " << &y << endl;
    
    return 0;
}