#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
};

vector<Product> readProductsFromCSV(const string& filename) {
    vector<Product> products;
    ifstream file(filename);
    string line;
    
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Product product;
        
        // Читаем ID
        getline(ss, token, ',');
        product.id = stoi(token);
        
        // Читаем название
        getline(ss, token, ',');
        product.name = token;
        
        // Читаем категорию
        getline(ss, token, ',');
        product.category = token;
        
        // Читаем цену
        getline(ss, token, ',');
        product.price = stod(token);
        
        // Читаем количество
        getline(ss, token, ',');
        product.quantity = stoi(token);
        
        products.push_back(product);
    }
    
    file.close();
    return products;
}

void writeProductsToCSV(const vector<Product>& products, const string& filename) {
    ofstream file(filename);
    
    file << "ID,Name,Category,Price,Quantity\n";
    
    for (const auto& product : products) {
        file << product.id << ","
             << product.name << ","
             << product.category << ","
             << fixed << setprecision(2) << product.price << ","
             << product.quantity << "\n";
    }
    
    file.close();
}

int showSortMenu() {
    cout << "Выберите критерий сортировки:\n";
    cout << "1. По цене (от дешевых к дорогим)\n";
    cout << "2. По цене (от дорогих к дешевым)\n";
    cout << "3. По названию (алфавитный порядок)\n";
    cout << "4. По количеству (от большего к меньшему)\n";
    cout << "Ваш выбор: ";
    
    int choice;
    cin >> choice;
    return choice;
}

int Task9() {
    try {
        vector<Product> products = readProductsFromCSV("../products.csv");
        cout << "Прочитано " << products.size() << " товаров\n";
        
        // Получаем выбор пользователя
        int choice = showSortMenu();
        
        // Сортируем в зависимости от выбора
        switch (choice) {
            case 1: {
                // По цене (от дешевых к дорогим)
                auto price_asc = [](const Product& a, const Product& b) {
                    return a.price < b.price;
                };
                sort(products.begin(), products.end(), price_asc);
                cout << "Отсортировано по цене (возрастание)\n";
                break;
            }
            case 2: {
                // По цене (от дорогих к дешевым)
                auto price_desc = [](const Product& a, const Product& b) {
                    return a.price > b.price;
                };
                sort(products.begin(), products.end(), price_desc);
                cout << "Отсортировано по цене (убывание)\n";
                break;
            }
            case 3: {
                // По названию (алфавитный порядок)
                auto name_asc = [](const Product& a, const Product& b) {
                    return a.name < b.name;
                };
                sort(products.begin(), products.end(), name_asc);
                cout << "Отсортировано по названию (алфавитный порядок)\n";
                break;
            }
            case 4: {
                // По количеству (от большего к меньшему)
                auto quantity_desc = [](const Product& a, const Product& b) {
                    return a.quantity > b.quantity;
                };
                sort(products.begin(), products.end(), quantity_desc);
                cout << "Отсортировано по количеству (убывание)\n";
                break;
            }
            default:
                cout << "Неверный выбор! Данные не отсортированы.\n";
                break;
        }
        
        writeProductsToCSV(products, "../sorted_products.csv");
        cout << "Отсортированные данные сохранены в sorted_products.csv\n";
        
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}