#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
using namespace std;
int CarSpeedAnalyze() {
    setlocale(LC_ALL, "ru");
    
       std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 180);
    
    // Создание и заполнение вектора скоростей (50 значений)
    std::vector<int> speeds(50);
    std::generate(speeds.begin(), speeds.end(), [&]() { return dis(gen); });
    
    // Вывод исходных данных
    std::cout << "АНАЛИЗ СКОРОСТИ АВТОМОБИЛЯ" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Исходные скорости (50 значений):" << std::endl;
    for (size_t i = 0; i < speeds.size(); ++i) {
        std::cout << std::setw(3) << speeds[i] << " km/h";
        if ((i + 1) % 10 == 0) std::cout << std::endl;
        else std::cout << " | ";
    }
    std::cout << std::endl;
    
    // 1. Отфильтруйте все превышения скорости > 120
    std::vector<int> speeding_violations;
    std::copy_if(speeds.begin(), speeds.end(), std::back_inserter(speeding_violations),
        [](int speed) { return speed > 120; });
    
    std::cout << "ПРЕВЫШЕНИЯ СКОРОСТИ (>120 km/h): " << speeding_violations.size() << " случаев" << std::endl;
    if (!speeding_violations.empty()) {
        std::cout << "Значения: ";
        for (size_t i = 0; i < speeding_violations.size(); ++i) {
            std::cout << speeding_violations[i];
            if (i < speeding_violations.size() - 1) std::cout << ", ";
        }
        std::cout << " km/h" << std::endl;
    }
    std::cout << std::endl;
    
    // 2. Найдите среднюю скорость
    double average_speed = std::accumulate(speeds.begin(), speeds.end(), 0.0) / speeds.size();
    std::cout << "СРЕДНЯЯ СКОРОСТЬ: " << std::fixed << std::setprecision(1) << average_speed << " km/h" << std::endl;
    
    // 3. Определите, были ли все значения больше 0 (двигался ли всегда)
    bool always_moving = std::all_of(speeds.begin(), speeds.end(),
        [](int speed) { return speed > 0; });
    
    std::cout << "АВТОМОБИЛЬ ВСЕГДА ДВИГАЛСЯ: " << (always_moving ? "ДА" : "НЕТ") << std::endl;
    
    if (!always_moving) {
        int zero_speeds = std::count(speeds.begin(), speeds.end(), 0);
        std::cout << "Количество остановок (скорость = 0): " << zero_speeds << std::endl;
        
        // Найдем минимальную ненулевую скорость
        auto min_moving_speed = std::min_element(speeds.begin(), speeds.end(),
            [](int a, int b) {
                if (a == 0) return false; // игнорируем нули
                if (b == 0) return true;
                return a < b;
            });
        if (*min_moving_speed > 0) {
            std::cout << "Минимальная скорость движения: " << *min_moving_speed << " km/h" << std::endl;
        }
    }
    std::cout << std::endl;
    
    // 4. Выведите 10 максимальных значений скорости
    std::vector<int> sorted_speeds = speeds;
    std::sort(sorted_speeds.begin(), sorted_speeds.end(), std::greater<int>());
    
    std::cout << "ТОП-10 МАКСИМАЛЬНЫХ СКОРОСТЕЙ:" << std::endl;
    int count = std::min(10, static_cast<int>(sorted_speeds.size()));
    for (int i = 0; i < count; ++i) {
        std::cout << (i + 1) << ". " << std::setw(3) << sorted_speeds[i] << " km/h";
        if (sorted_speeds[i] > 120) std::cout << " ⚠ ПРЕВЫШЕНИЕ!";
        std::cout << std::endl;
    }
    
    return 0;
}