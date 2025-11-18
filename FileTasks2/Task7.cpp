#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
};

vector<Product> readCSV(const string& filename) {
    vector<Product> products;
    ifstream file(filename);
    
    if (!file) {
        cerr << "Ошибка открытия файла!" << endl;
        return products;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 5) {
            Product product;
            product.id = stoi(tokens[0]);
            product.name = tokens[1];
            product.category = tokens[2];
            product.price = stod(tokens[3]);
            product.quantity = stoi(tokens[4]);
            
            products.push_back(product);
        }
    }
    
    file.close();
    return products;
}

int Task7() {
    vector<Product> products = readCSV("../products.csv");
    
    if (products.empty()) {
        cout << "Нет данных для анализа!" << endl;
        return 1;
    }
    
    cout << "СПИСОК ТОВАРОВ:" << endl;
    cout << "==================================================" << endl;
    for (const auto& p : products) {
        cout << "ID: " << p.id << " | " << p.name << " | " << p.category 
             << " | " << fixed << setprecision(2) << p.price << " руб. | " 
             << p.quantity << " шт." << endl;
    }
    cout << "==================================================" << endl;
    
    double totalValue = 0;
    int totalQuantity = 0;
    Product mostExpensive = products[0];
    Product cheapest = products[0];
    
    for (const auto& p : products) {
        totalValue += p.price * p.quantity;
        totalQuantity += p.quantity;
        
        if (p.price > mostExpensive.price) mostExpensive = p;
        if (p.price < cheapest.price) cheapest = p;
    }
    
    cout << "\nСТАТИСТИКА:" << endl;
    cout << "Количество товаров: " << products.size() << endl;
    cout << "Общее количество на складе: " << totalQuantity << " шт." << endl;
    cout << "Суммарная стоимость: " << totalValue << " руб." << endl;
    cout << "Самый дорогой товар: " << mostExpensive.name << " (" << mostExpensive.price << " руб.)" << endl;
    cout << "Самый дешевый товар: " << cheapest.name << " (" << cheapest.price << " руб.)" << endl;
    
    return 0;
}