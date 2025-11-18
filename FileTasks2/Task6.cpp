#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
};

int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

double randomPrice(double min, double max) {
    return min + (rand() % static_cast<int>((max - min) * 100 + 1)) / 100.0;
}

void writeProductToCSV(ofstream& file, const Product& product) {
    file << product.id << ","
         << product.name << ","
         << product.category << ","
         << fixed << setprecision(2) << product.price << ","
         << product.quantity << "\n";
}

int Task6() {
    srand(time(nullptr));

    vector<string> categories = {"Электроника", "Аксессуары", "Одежда", "Книги",
                                "Спорт", "Красота", "Дом", "Игрушки"};

    vector<string> productNames = {
        "Ноутбук Dell", "Мышь Bluetooth", "Клавиатура механическая", "Наушники Sony",
        "Смартфон Samsung", "Планшет iPad", "Монитор LG", "Принтер HP",
        "Футболка хлопковая", "Джинсы классические", "Кроссовки Nike", "Куртка зимняя",
        "Книга 'Мастер и Маргарита'", "Английский словарь", "Детская энциклопедия",
        "Мяч футбольный", "Гантели 5кг", "Йога-мат", "Шампунь увлажняющий",
        "Крем для лица", "Зубная паста", "Сковорода антипригарная", "Чайник электрический",
        "Конструктор LEGO", "Кукла Barbie", "Пазл 1000 деталей"
    };

    ofstream csvFile("../products.csv");
    if (!csvFile) {
        cerr << "Ошибка создания файла products.csv!" << endl;
        return 1;
    }

    csvFile << "id,name,category,price,quantity\n";

    cout << "Генерация 20 товаров..." << endl;

    for (int i = 0; i < 20; ++i) {
        Product product;
        product.id = i + 1;
        product.name = productNames[randomInt(0, productNames.size() - 1)];
        product.category = categories[randomInt(0, categories.size() - 1)];
        product.price = randomPrice(100.0, 10000.0);
        product.quantity = randomInt(1, 100);

        writeProductToCSV(csvFile, product);

        cout << "Добавлен товар: " << product.name << " - " << product.price << " руб." << endl;
    }

    csvFile.close();
    cout << "\nФайл products.csv успешно создан!" << endl;

    cout << "\nПроверка созданного файла..." << endl;

    ifstream checkFile("../products.csv");
    if (!checkFile) {
        cerr << "Ошибка открытия файла для проверки!" << endl;
        return 1;
    }

    string line;
    int lineCount = 0;

    while (getline(checkFile, line)) {
        lineCount++;
        if (lineCount <= 3) {
            cout << lineCount << ": " << line << endl;
        }
    }

    checkFile.close();

    cout << "\nРезультаты проверки:" << endl;
    cout << "Общее количество строк в файле: " << lineCount << endl;
    cout << "Количество заголовков: 1" << endl;
    cout << "Количество записей о товарах: " << (lineCount - 1) << endl;

    if (lineCount - 1 == 20) {
        cout << "✓ Файл содержит правильное количество записей!" << endl;
    } else {
        cout << "✗ Ошибка: неверное количество записей!" << endl;
    }

    return 0;
}