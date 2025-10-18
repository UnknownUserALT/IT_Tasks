#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
using namespace std;
int ElectroEngineStats() {
    setlocale(LC_ALL, "ru");

    // Генератор случайных значений мощности (включая возможные ошибки датчика)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-5.0, 100.0); // Диапазон с возможными ошибками <0

    // Создание и заполнение вектора мощности (20 измерений в кВт)
    std::vector<double> power_measurements(20);
    std::generate(power_measurements.begin(), power_measurements.end(), [&]() { return dis(gen); });

    // Вывод исходных данных
    std::cout << "СТАТИСТИКА ПО МОЩНОСТИ ЭЛЕКТРОДВИГАТЕЛЯ" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Исходные измерения мощности (20 значений):" << std::endl;
    for (size_t i = 0; i < power_measurements.size(); ++i) {
        std::cout << std::fixed << std::setprecision(1) << std::setw(6) << power_measurements[i] << " кВт";
        if (power_measurements[i] < 0) std::cout << " ⚠ ОШИБКА";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
        else std::cout << " | ";
    }
    std::cout << std::endl;

    // 1. Удаление всех измерений < 0 (ошибки датчика)
    power_measurements.erase(
        std::remove_if(power_measurements.begin(), power_measurements.end(),
            [](double power) { return power < 0.0; }),
        power_measurements.end()
    );

    std::cout << "После удаления ошибок датчика (< 0 кВт):" << std::endl;
    std::cout << "Осталось измерений: " << power_measurements.size() << std::endl;
    if (!power_measurements.empty()) {
        for (size_t i = 0; i < power_measurements.size(); ++i) {
            std::cout << std::setw(5) << power_measurements[i] << " кВт";
            if ((i + 1) % 5 == 0) std::cout << std::endl;
            else std::cout << " | ";
        }
        std::cout << std::endl << std::endl;
    }
    // Проверка наличия данных после фильтрации
    if (power_measurements.empty()) {
        std::cout << "Нет данных для анализа после фильтрации ошибок!" << std::endl;
        return 0;
    }

    // 2. Нахождение минимальной, максимальной и средней мощности
    auto min_power = std::min_element(power_measurements.begin(), power_measurements.end());
    auto max_power = std::max_element(power_measurements.begin(), power_measurements.end());
    double average_power = std::accumulate(power_measurements.begin(), power_measurements.end(), 0.0) / power_measurements.size();

    std::cout << "СТАТИСТИКА МОЩНОСТИ:" << std::endl;
    std::cout << "Минимальная мощность: " << std::setw(6) << *min_power << " кВт" << std::endl;
    std::cout << "Максимальная мощность: " << std::setw(6) << *max_power << " кВт" << std::endl;
    std::cout << "Средняя мощность: " << std::setw(6) << std::setprecision(2) << average_power << " кВт" << std::endl;
    std::cout << std::endl;

    // 3. Проверка, работал ли двигатель всё время в пределах от 10 до 90 кВт
    bool within_limits = std::all_of(power_measurements.begin(), power_measurements.end(),
        [](double power) { return power >= 10.0 && power <= 90.0; });

    std::cout << "РАБОТА В НОРМАЛЬНОМ РЕЖИМЕ [10-90 кВт]:" << std::endl;
    std::cout << "Все измерения в пределах: " << (within_limits ? "ДА" : "НЕТ") << std::endl;

    if (!within_limits) {
        int below_limit = std::count_if(power_measurements.begin(), power_measurements.end(),
            [](double power) { return power < 10.0; });
        int above_limit = std::count_if(power_measurements.begin(), power_measurements.end(),
            [](double power) { return power > 90.0; });

        std::cout << "Измерений ниже 10 кВт: " << below_limit << std::endl;
        std::cout << "Измерений выше 90 кВт: " << above_limit << std::endl;
        // Найдем конкретные значения вне диапазона
        if (below_limit > 0) {
            std::cout << "Значения < 10 кВт: ";
            for (double power : power_measurements) {
                if (power < 10.0) std::cout << power << " ";
            }
            std::cout << "кВт" << std::endl;
        }
        if (above_limit > 0) {
            std::cout << "Значения > 90 кВт: ";
            for (double power : power_measurements) {
                if (power > 90.0) std::cout << power << " ";
            }
            std::cout << "кВт" << std::endl;
        }
    }
    std::cout << std::endl;

    // 4. Сортировка значений по возрастанию и вычисление медианы
    std::vector<double> sorted_power = power_measurements;
    std::sort(sorted_power.begin(), sorted_power.end());

    double median;
    size_t size = sorted_power.size();
    if (size % 2 == 0) {
        // Четное количество - среднее двух центральных значений
        median = (sorted_power[size/2 - 1] + sorted_power[size/2]) / 2.0;
    } else {
        // Нечетное количество - центральное значение
        median = sorted_power[size/2];
    }

    std::cout << "МЕДИАНА МОЩНОСТИ:" << std::endl;
    std::cout << "Отсортированные значения: ";
    for (size_t i = 0; i < sorted_power.size(); ++i) {
        std::cout << sorted_power[i];
        if (i == size/2 - 1 || (size % 2 != 0 && i == size/2)) std::cout << "*"; // пометка медианы
        if (i < sorted_power.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "Медиана: " << std::setprecision(2) << median << " кВт" << std::endl;

    return 0;
}