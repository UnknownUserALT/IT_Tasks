#include <iostream>
using namespace std;

int tabascii() {
    cout << "Таблица печатаемых символов ASCII (32–126):\n\n";
    for (int i = 32; i <= 126; i++) {
        cout << i << " : " << static_cast<char>(i) << endl;
    }

    return 0;
}