#include <iostream>
using namespace std;

int stopcount()
{
    int stops;
    int totalСost = 0;
    const int pricePerStop = 5;

    cout << "Введите количество остановок: ";
    cin >> stops;


    for (int i = 0; i < stops; i++) {
        totalСost += pricePerStop;
    }

    cout << "Общая стоимость проезда: " << totalСost << " рублей" << endl;

    return 0;
}