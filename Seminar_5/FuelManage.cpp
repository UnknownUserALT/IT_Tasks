#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>
using namespace std;
int FuelManage() {
    setlocale(LC_ALL, "ru");
    
    // Создание вектора уровня топлива
    std::vector<double> fuel_level(50);
    
    // Заполнение случайными значениями (убывающий тренд)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.5, 1.0); // случайные колебания
    
    double current = 100.0; // начальный уровень 100%
    for (size_t i = 0; i < fuel_level.size(); ++i) {
        fuel_level[i] = current * dis(gen);
        current -= 0.5; // линейное уменьшение
    }
    
    // Вывод исходных значений
    std::cout << "Уровень топлива (50 замеров):" << std::endl;
    for (size_t i = 0; i < fuel_level.size(); ++i) {
        std::cout << fuel_level[i];
        if (i < fuel_level.size() - 1) std::cout << ", ";
        if ((i + 1) % 10 == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // 1. Найдите минимальный уровень топлива
    double min_level = *std::min_element(fuel_level.begin(), fuel_level.end());
    std::cout << "Минимальный уровень: " << min_level << "%" << std::endl;
    
    // 2. Проверьте, не падал ли уровень ниже 5% от максимального
    double max_level = *std::max_element(fuel_level.begin(), fuel_level.end());
    double threshold = max_level * 0.05;
    bool below_threshold = std::any_of(fuel_level.begin(), fuel_level.end(),
        [threshold](double level) { return level < threshold; });
    
    std::cout << "Уровень падал ниже 5% от максимального: " 
              << (below_threshold ? "ДА" : "НЕТ") << std::endl;
    
    // 3. Посчитайте среднее потребление
    double consumption = fuel_level.front() - fuel_level.back();
    std::cout << "Среднее потребление: " << consumption << "% за период" << std::endl;
    
    // 4. Отсортируйте значения и найдите медиану
    std::vector<double> sorted_fuel = fuel_level;
    std::sort(sorted_fuel.begin(), sorted_fuel.end());
    
    // Вывод отсортированных значений
    std::cout << "Отсортированные значения:" << std::endl;
    for (size_t i = 0; i < sorted_fuel.size(); ++i) {
        std::cout << sorted_fuel[i];
        if (i < sorted_fuel.size() - 1) std::cout << ", ";
        if ((i + 1) % 10 == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
    
    double median;
    size_t size = sorted_fuel.size();
    if (size % 2 == 0) {
        median = (sorted_fuel[size/2 - 1] + sorted_fuel[size/2]) / 2.0;
    } else {
        median = sorted_fuel[size/2];
    }
    
    std::cout << "Медиана: " << median << "%" << std::endl;
    
    return 0;
}