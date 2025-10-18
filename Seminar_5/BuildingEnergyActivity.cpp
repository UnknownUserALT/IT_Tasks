#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
using namespace std;
int BuildingEnergyActivity() {
    setlocale(LC_ALL, "ru");
    
        // Генератор случайных значений энергопотребления
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(50.0, 200.0); // кВт·ч в день
    
    // Создание и заполнение вектора потребления энергии за 30 дней
    std::vector<double> energy_consumption(30);
    std::generate(energy_consumption.begin(), energy_consumption.end(), [&]() { return dis(gen); });
    
    // Вывод исходных данных
    std::cout << "АНАЛИЗ ЭНЕРГОПОТРЕБЛЕНИЯ ЗДАНИЯ" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Суточное потребление энергии за 30 дней (кВт·ч):" << std::endl;
    for (size_t i = 0; i < energy_consumption.size(); ++i) {
        std::cout << "День " << std::setw(2) << (i + 1) << ": " 
                  << std::fixed << std::setprecision(1) << std::setw(6) 
                  << energy_consumption[i] << " кВт·ч";
        if ((i + 1) % 5 == 0) std::cout << std::endl;
        else std::cout << " | ";
    }
    std::cout << std::endl;
    
    // 1. Найдите общее потребление за месяц
    double total_consumption = std::accumulate(energy_consumption.begin(), energy_consumption.end(), 0.0);
    std::cout << "ОБЩЕЕ ПОТРЕБЛЕНИЕ ЗА МЕСЯЦ: " << std::setprecision(1) << total_consumption << " кВт·ч" << std::endl;
    
    // 2. Определите среднесуточное значение
    double average_daily = total_consumption / energy_consumption.size();
    std::cout << "СРЕДНЕСУТОЧНОЕ ПОТРЕБЛЕНИЕ: " << std::setprecision(2) << average_daily << " кВт·ч" << std::endl;
    std::cout << std::endl;
    
    // 3. Найдите дни с максимальным и минимальным потреблением
    auto [min_day_it, max_day_it] = std::minmax_element(energy_consumption.begin(), energy_consumption.end());
    int min_day_index = std::distance(energy_consumption.begin(), min_day_it) + 1;
    int max_day_index = std::distance(energy_consumption.begin(), max_day_it) + 1;
    
    std::cout << "ЭКСТРЕМАЛЬНЫЕ ЗНАЧЕНИЯ:" << std::endl;
    std::cout << "Минимальное потребление: День " << min_day_index 
              << " - " << std::setprecision(1) << *min_day_it << " кВт·ч" << std::endl;
    std::cout << "Максимальное потребление: День " << max_day_index 
              << " - " << *max_day_it << " кВт·ч" << std::endl;
    std::cout << "Разница: " << (*max_day_it - *min_day_it) << " кВт·ч" << std::endl;
    std::cout << std::endl;
    
    // 4. Отфильтруйте «пиковые» дни (значение > среднего на 20%)
    double peak_threshold = average_daily * 1.2; // На 20% выше среднего
    std::vector<std::pair<int, double>> peak_days; // день, потребление
    
    for (size_t i = 0; i < energy_consumption.size(); ++i) {
        if (energy_consumption[i] > peak_threshold) {
            peak_days.push_back({i + 1, energy_consumption[i]});
        }
    }
    
    std::cout << "ПИКОВЫЕ ДНИ (потребление > " << std::setprecision(1) << peak_threshold 
              << " кВт·ч - на 20% выше среднего):" << std::endl;
    std::cout << "Количество пиковых дней: " << peak_days.size() << std::endl;
    
    if (!peak_days.empty()) {
        // Сортируем пиковые дни по убыванию потребления
        std::sort(peak_days.begin(), peak_days.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
        
        for (const auto& [day, consumption] : peak_days) {
            double above_average = ((consumption - average_daily) / average_daily) * 100;
            std::cout << "День " << std::setw(2) << day << ": " << std::setw(6) << consumption 
                      << " кВт·ч (+" << std::setprecision(1) << above_average << "%)" << std::endl;
        }
    } else {
        std::cout << "Пиковые дни не обнаружены" << std::endl;
    }
    std::cout << std::endl;
    
    return 0;
}