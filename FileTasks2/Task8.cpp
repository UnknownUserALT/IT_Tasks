#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <functional>

using namespace std;

struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
};

vector<Product> readCSV2(const string& filename) {
    vector<Product> products;
    ifstream file(filename);
    
    if (!file.is_open()) {
        throw runtime_error("Ошибка: не удалось открыть файл " + filename);
    }
    
    string line;
    int lineNumber = 0;
    
    while (getline(file, line)) {
        lineNumber++;
        if (lineNumber == 1 || line.empty()) continue;
        
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

void writeFilteredCSV(const vector<Product>& products, const string& filename) {
    ofstream file(filename);
    
    if (!file.is_open()) {
        throw runtime_error("Ошибка: не удалось создать файл " + filename);
    }
    
    file << "id,name,category,price,quantity\n";
    
    for (const auto& product : products) {
        file << product.id << ","
             << product.name << ","
             << product.category << ","
             << fixed << setprecision(2) << product.price << ","
             << product.quantity << "\n";
    }
    
    file.close();
}

void printProducts(const vector<Product>& products, const string& title) {
    cout << "\n" << title << " (" << products.size() << " товаров):" << endl;
    cout << "==================================================" << endl;
    
    if (products.empty()) {
        cout << "Нет товаров для отображения" << endl;
        return;
    }
    
    for (const auto& product : products) {
        cout << "ID: " << product.id 
             << " | " << setw(25) << left << product.name.substr(0, 25)
             << " | " << setw(15) << left << product.category
             << " | " << setw(8) << right << fixed << setprecision(2) << product.price << " руб."
             << " | " << setw(3) << product.quantity << " шт." << endl;
    }
}

int Task8() {
    try {
        cout << "📁 Чтение файла products.csv..." << endl;
        vector<Product> products = readCSV2("../products.csv");
        
        if (products.empty()) {
            cout << "Файл пуст или не содержит данных!" << endl;
            return 1;
        }
        
        cout << "✅ Успешно прочитано " << products.size() << " товаров." << endl;
        
        printProducts(products, "📦 ВСЕ ТОВАРЫ");
        

        // Фильтр по цене (дороже указанной)
        auto priceGreaterThan = [](double minPrice) {
            return [minPrice](const Product& p) { return p.price > minPrice; };
        };

        // Фильтр по цене (дешевле указанной)
        auto priceLessThan = [](double maxPrice) {
            return [maxPrice](const Product& p) { return p.price < maxPrice; };
        };

        // Фильтр по категории
        auto categoryFilter = [](const string& category) {
            return [category](const Product& p) { return p.category == category; };
        };
        
        // Фильтр по количеству (больше указанного)
        auto quantityGreaterThan = [](int minQuantity) {
            return [minQuantity](const Product& p) { return p.quantity > minQuantity; };
        };

        // Фильтр по количеству (меньше указанного)
        auto quantityLessThan = [](int maxQuantity) {
            return [maxQuantity](const Product& p) { return p.quantity < maxQuantity; };
        };
        
        // Комбинированный фильтр (И)
        auto combinedAnd = [](auto filter1, auto filter2) {
            return [filter1, filter2](const Product& p) { 
                return filter1(p) && filter2(p); 
            };
        };
        
        vector<Product> filteredProducts;
        
        int choice;
        do {
            cout << "\n🎯 ВЫБЕРИТЕ ТИП ФИЛЬТРА:" << endl;
            cout << "1️⃣  Товары дороже указанной цены" << endl;
            cout << "2️⃣  Товары дешевле указанной цены" << endl;
            cout << "3️⃣  Товары из определенной категории" << endl;
            cout << "4️⃣  Товары с количеством больше указанного" << endl;
            cout << "5️⃣  Товары с количеством меньше указанного" << endl;
            cout << "6️⃣  Комбинированный фильтр (цена + категория)" << endl;
            cout << "7️⃣  Комбинированный фильтр (цена + количество)" << endl;
            cout << "0️⃣  Выход" << endl;
            cout << "Ваш выбор: ";
            cin >> choice;
            
            filteredProducts.clear();
            
            switch (choice) {
                case 1: {
                    double minPrice;
                    cout << "Введите минимальную цену: ";
                    cin >> minPrice;
                    
                    copy_if(products.begin(), products.end(), 
                            back_inserter(filteredProducts), 
                            priceGreaterThan(minPrice));
                    
                    printProducts(filteredProducts, "💰 ТОВАРЫ ДОРОЖЕ " + to_string((int)minPrice) + " РУБ.");
                    break;
                }
                
                case 2: {
                    double maxPrice;
                    cout << "Введите максимальную цену: ";
                    cin >> maxPrice;
                    
                    copy_if(products.begin(), products.end(), 
                            back_inserter(filteredProducts), 
                            priceLessThan(maxPrice));
                    
                    printProducts(filteredProducts, "💸 ТОВАРЫ ДЕШЕВЛЕ " + to_string((int)maxPrice) + " РУБ.");
                    break;
                }
                
                case 3: {
                    string category;
                    cout << "Введите категорию: ";
                    cin.ignore();
                    getline(cin, category);
                    
                    copy_if(products.begin(), products.end(), 
                            back_inserter(filteredProducts), 
                            categoryFilter(category));
                    
                    printProducts(filteredProducts, "🏷️  ТОВАРЫ ИЗ КАТЕГОРИИ '" + category + "'");
                    break;
                }
                
                case 4: {
                    int minQuantity;
                    cout << "Введите минимальное количество: ";
                    cin >> minQuantity;
                    
                    copy_if(products.begin(), products.end(), 
                            back_inserter(filteredProducts), 
                            quantityGreaterThan(minQuantity));
                    
                    printProducts(filteredProducts, "📦 ТОВАРЫ С КОЛИЧЕСТВОМ БОЛЬШЕ " + to_string(minQuantity));
                    break;
                }
                
                case 5: {
                    int maxQuantity;
                    cout << "Введите максимальное количество: ";
                    cin >> maxQuantity;
                    
                    copy_if(products.begin(), products.end(), 
                            back_inserter(filteredProducts), 
                            quantityLessThan(maxQuantity));
                    
                    printProducts(filteredProducts, "📥 ТОВАРЫ С КОЛИЧЕСТВОМ МЕНЬШЕ " + to_string(maxQuantity));
                    break;
                }
                
                case 6: {
                    double minPrice;
                    string category;
                    cout << "Введите минимальную цену: ";
                    cin >> minPrice;
                    cout << "Введите категорию: ";
                    cin.ignore();
                    getline(cin, category);
                    
                    auto combinedFilter = combinedAnd(priceGreaterThan(minPrice), categoryFilter(category));
                    copy_if(products.begin(), products.end(), 
                            back_inserter(filteredProducts), 
                            combinedFilter);
                    
                    printProducts(filteredProducts, "🔍 ТОВАРЫ ДОРОЖЕ " + to_string((int)minPrice) + " РУБ. ИЗ КАТЕГОРИИ '" + category + "'");
                    break;
                }
                
                case 7: {
                    double minPrice;
                    int minQuantity;
                    cout << "Введите минимальную цену: ";
                    cin >> minPrice;
                    cout << "Введите минимальное количество: ";
                    cin >> minQuantity;
                    
                    auto combinedFilter = combinedAnd(priceGreaterThan(minPrice), quantityGreaterThan(minQuantity));
                    copy_if(products.begin(), products.end(), 
                            back_inserter(filteredProducts), 
                            combinedFilter);
                    
                    printProducts(filteredProducts, "🔍 ТОВАРЫ ДОРОЖЕ " + to_string((int)minPrice) + " РУБ. И КОЛИЧЕСТВОМ БОЛЬШЕ " + to_string(minQuantity));
                    break;
                }
                
                case 0:
                    cout << "Выход из программы." << endl;
                    break;
                    
                default:
                    cout << "Неверный выбор!" << endl;
                    break;
            }
            
            // Сохранение отфильтрованных данных
            if (!filteredProducts.empty() && choice != 0) {
                writeFilteredCSV(filteredProducts, "filtered_products.csv");
                cout << "\n💾 Отфильтрованные данные сохранены в файл: filtered_products.csv" << endl;
                cout << "📊 Найдено товаров: " << filteredProducts.size() << endl;
            }
            
        } while (choice != 0);
        
    } catch (const exception& e) {
        cerr << "❌ Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}