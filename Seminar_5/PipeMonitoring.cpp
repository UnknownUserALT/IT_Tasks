#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>
using namespace std;
int PipeMonitoring() {
    setlocale(LC_ALL, "ru");
    
        // Создание вектора давления в трубопроводе
    std::vector<double> pressure(50);
    
    // Заполнение случайными значениями (1-10 бар + возможные ошибки)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.5, 11.5); // включая возможные ошибки
    
    std::for_each(pressure.begin(), pressure.end(), [&](double& p) {
        p = dis(gen);
    });
    
    // Вывод исходных значений
    std::cout << "Исходные измерения давления (50 значений):" << std::endl;
    std::for_each(pressure.begin(), pressure.end(), [](double p) {
        std::cout << p << " ";
    });
    std::cout << std::endl << std::endl;
    
    // Удаление ошибочных значений (< 0 или > 12)
    pressure.erase(
        std::remove_if(pressure.begin(), pressure.end(),
            [](double p) { return p < 0 || p > 12; }),
        pressure.end()
    );
    
    // Вывод отфильтрованных значений
    std::cout << "После удаления ошибочных значений (< 0 или > 12):" << std::endl;
    std::for_each(pressure.begin(), pressure.end(), [](double p) {
        std::cout << p << " ";
    });
    std::cout << std::endl << std::endl;
    
    // Вычисление среднего давления
    double average_pressure = std::accumulate(pressure.begin(), pressure.end(), 0.0) / pressure.size();
    std::cout << "Среднее давление: " << average_pressure << " бар" << std::endl;
    
    // Нахождение минимального и максимального давления
    auto [min_it, max_it] = std::minmax_element(pressure.begin(), pressure.end());
    std::cout << "Минимальное давление: " << *min_it << " бар" << std::endl;
    std::cout << "Максимальное давление: " << *max_it << " бар" << std::endl;
    
    // Вывод значений, превышающих 8 бар
    std::cout << "Значения, превышающие 8 бар: ";
    std::for_each(pressure.begin(), pressure.end(), [](double p) {
        if (p > 8) {
            std::cout << p << " ";
        }
    });
    std::cout << std::endl;
    
    return 0;
}