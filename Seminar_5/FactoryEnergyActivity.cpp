#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>
using namespace std;
int FactoryEnergyActivity() {
    setlocale(LC_ALL, "ru");
    
        // Создание вектора потребления (24 часа)
    std::vector<double> consumption(24);
    
    // Заполнение случайными значениями (100-500 кВт·ч)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(100.0, 500.0);
    
    std::for_each(consumption.begin(), consumption.end(), [&](double& c) {
        c = dis(gen);
    });
    
    // Вывод исходных значений
    std::cout << "Исходное потребление по часам (кВт·ч):" << std::endl;
    for (int i = 0; i < 24; ++i) {
        std::cout << "Час " << i << ": " << consumption[i] << " кВт·ч" << std::endl;
    }
    std::cout << std::endl;
    
    // Вычисление суммарного и среднего потребления
    double total_consumption = std::accumulate(consumption.begin(), consumption.end(), 0.0);
    double average_consumption = total_consumption / consumption.size();
    
    std::cout << "Суммарное потребление: " << total_consumption << " кВт·ч" << std::endl;
    std::cout << "Среднее потребление: " << average_consumption << " кВт·ч" << std::endl << std::endl;
    
    // Нахождение часа пикового потребления
    auto peak_hour_it = std::max_element(consumption.begin(), consumption.end());
    int peak_hour = std::distance(consumption.begin(), peak_hour_it);
    
    std::cout << "Час пикового потребления: " << peak_hour << " (" << *peak_hour_it << " кВт·ч)" << std::endl << std::endl;
    
    // Расчёт отклонения каждого часа от среднего
    std::vector<double> deviations(24);
    std::transform(consumption.begin(), consumption.end(), deviations.begin(),
        [average_consumption](double c) { return c - average_consumption; });
    
    std::cout << "Отклонения от среднего по часам:" << std::endl;
    for (int i = 0; i < 24; ++i) {
        std::cout << "Час " << i << ": " << deviations[i] << " кВт·ч" << std::endl;
    }
    std::cout << std::endl;
    
    // Сортировка и вывод топ-5 минимальных часов потребления
    std::vector<double> sorted_consumption = consumption;
    std::sort(sorted_consumption.begin(), sorted_consumption.end());
    
    std::cout << "Отсортированные значения потребления:" << std::endl;
    for (double c : sorted_consumption) {
        std::cout << c << " ";
    }
    std::cout << std::endl << std::endl;
    
    std::cout << "Топ-5 минимальных часов потребления: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << sorted_consumption[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}