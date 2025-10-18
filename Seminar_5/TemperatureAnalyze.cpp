#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
using namespace std;
int TempAnalyze() {
    setlocale(LC_ALL, "ru");
    
        std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-60.0, 60.0); // Диапазон шире [-50,50]
    
    // Создание и заполнение вектора температур
    std::vector<double> temperatures(15);
    std::generate(temperatures.begin(), temperatures.end(), [&]() { return dis(gen); });
    
    // Вывод исходных данных
    std::cout << "АНАЛИЗ ТЕМПЕРАТУР" << std::endl;
    std::cout << "=================" << std::endl;
    std::cout << "Исходные температуры: ";
    for (double temp : temperatures) std::cout << std::fixed << std::setprecision(1) << temp << " ";
    std::cout << std::endl << std::endl;
    
    // 1. Удаление значений вне диапазона [-50, 50]
    temperatures.erase(
        std::remove_if(temperatures.begin(), temperatures.end(),
            [](double temp) {
                return temp < -50.0 || temp > 50.0; // Удаляем если вне диапазона
            }),
        temperatures.end()
    );
    
    std::cout << "Температуры в диапазоне [-50, 50]: ";
    for (double temp : temperatures) std::cout << temp << " ";
    std::cout << std::endl;
    std::cout << "Количество температур после фильтрации: " << temperatures.size() << std::endl << std::endl;
    
    // Проверяем, что остались данные для анализа
    if (temperatures.empty()) {
        std::cout << "Нет данных для анализа после фильтрации!" << std::endl;
        return 0;
    }
    
    // 2. Нахождение среднего, минимального и максимального значения
    // Минимальная температура
    auto min_temp = std::min_element(temperatures.begin(), temperatures.end());
    
    // Максимальная температура
    auto max_temp = std::max_element(temperatures.begin(), temperatures.end());
    
    // Средняя температура
    double average_temp = std::accumulate(temperatures.begin(), temperatures.end(), 0.0) / temperatures.size();
    
    std::cout << "СТАТИСТИКА:" << std::endl;
    std::cout << "Минимальная температура: " << *min_temp << "°C" << std::endl;
    std::cout << "Максимальная температура: " << *max_temp << "°C" << std::endl;
    std::cout << "Средняя температура: " << average_temp << "°C" << std::endl << std::endl;
    
    // 3. Проверка, превышают ли все температуры -10 градусов
    bool all_above_minus_10 = std::all_of(temperatures.begin(), temperatures.end(),
        [](double temp) {
            return temp > -10.0; // Проверяем, что все температуры > -10
        });
    
    if (all_above_minus_10) {
        std::cout << "✓ Все температуры превышают -10°C" << std::endl;
    } else {
        std::cout << "✗ Не все температуры превышают -10°C" << std::endl;
        
        // Найдем температуры, которые не превышают -10
        std::cout << "Температуры ≤ -10°C: ";
        std::for_each(temperatures.begin(), temperatures.end(),
            [](double temp) {
                if (temp <= -10.0) std::cout << temp << " ";
            });
        std::cout << std::endl;
    }
    
    return 0;
}