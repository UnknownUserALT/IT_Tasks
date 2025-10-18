#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>
using namespace std;
int TemperatureAndPressureMonitoring() {
    setlocale(LC_ALL, "ru");

    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> temp_dis(20.0, 100.0);
    std::uniform_real_distribution<double> pressure_dis(1.0, 10.0);

    // Создание данных температуры (5 каналов по 30 измерений)
    std::vector<std::vector<double>> temp(5, std::vector<double>(30));
    std::for_each(temp.begin(), temp.end(), [&](std::vector<double>& channel) {
        std::for_each(channel.begin(), channel.end(), [&](double& t) {
            t = temp_dis(gen);
        });
    });

    // Создание данных давления (5 каналов по 100 измерений)
    std::vector<std::vector<double>> pressure(5, std::vector<double>(100));
    std::for_each(pressure.begin(), pressure.end(), [&](std::vector<double>& channel) {
        std::for_each(channel.begin(), channel.end(), [&](double& p) {
            p = pressure_dis(gen);
        });
    });

    // Вывод значений температуры по каналам
    std::cout << "Температуры по каналам:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "Канал " << i << ": ";
        for (int j = 0; j < 30; ++j) {
            std::cout << temp[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    // Вывод значений давления по каналам (первые 20 значений каждого канала)
    std::cout << "Давление по каналам (первые 20 значений):" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "Канал " << i << ": ";
        for (int j = 0; j < 20; ++j) {
            std::cout << pressure[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 1. Для каждого канала температуры рассчитать среднюю температуру
    std::vector<double> avg_temperatures(5);
    std::transform(temp.begin(), temp.end(), avg_temperatures.begin(),
        [](const std::vector<double>& channel) {
            return std::accumulate(channel.begin(), channel.end(), 0.0) / channel.size();
        });

    std::cout << "Средние температуры по каналам:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "Канал " << i << ": " << avg_temperatures[i] << "°C" << std::endl;
    }
    std::cout << std::endl;

    // 2. Для каждого канала давления найти минимальное и максимальное давление
    std::vector<std::pair<double, double>> pressure_minmax(5);
    std::transform(pressure.begin(), pressure.end(), pressure_minmax.begin(),
        [](const std::vector<double>& channel) {
            auto [min_it, max_it] = std::minmax_element(channel.begin(), channel.end());
            return std::make_pair(*min_it, *max_it);
        });

    std::cout << "Минимальное и максимальное давление по каналам:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "Канал " << i << ": min=" << pressure_minmax[i].first
                  << ", max=" << pressure_minmax[i].second << " бар" << std::endl;
    }
    std::cout << std::endl;
    // 3. Рассчитать разности между соседними измерениями для температуры и давления
    std::vector<std::vector<double>> temp_differences(5, std::vector<double>(29));
    for (int i = 0; i < 5; ++i) {
        std::transform(temp[i].begin() + 1, temp[i].end(), temp[i].begin(),
                      temp_differences[i].begin(),
                      [](double a, double b) { return a - b; });
    }

    std::vector<std::vector<double>> pressure_differences(5, std::vector<double>(99));
    for (int i = 0; i < 5; ++i) {
        std::transform(pressure[i].begin() + 1, pressure[i].end(), pressure[i].begin(),
                      pressure_differences[i].begin(),
                      [](double a, double b) { return a - b; });
    }

    std::cout << "Первые 5 разностей температуры для каждого канала:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "Канал " << i << ": ";
        for (int j = 0; j < 5; ++j) {
            std::cout << temp_differences[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 4. Определить канал с наибольшей вариацией температуры (разность max-min)
    std::vector<double> temp_variations(5);
    std::transform(temp.begin(), temp.end(), temp_variations.begin(),
        [](const std::vector<double>& channel) {
            auto [min_it, max_it] = std::minmax_element(channel.begin(), channel.end());
            return *max_it - *min_it;
        });

    auto max_var_it = std::max_element(temp_variations.begin(), temp_variations.end());
    int max_var_channel = std::distance(temp_variations.begin(), max_var_it);

    std::cout << "Канал с наибольшей вариацией температуры: " << max_var_channel
              << " (вариация: " << *max_var_it << "°C)" << std::endl << std::endl;
    // 5. Для каждого канала давления нормализовать значения: (value - min)/(max - min)
    std::vector<std::vector<double>> normalized_pressure(5, std::vector<double>(100));
    for (int i = 0; i < 5; ++i) {
        double min_p = pressure_minmax[i].first;
        double max_p = pressure_minmax[i].second;
        double range = max_p - min_p;

        std::transform(pressure[i].begin(), pressure[i].end(), normalized_pressure[i].begin(),
            [min_p, range](double p) { return (p - min_p) / range; });
    }

    // Вывод нормализованных значений давления (первые 10 значений каждого канала)
    std::cout << "Нормализованные значения давления (первые 10 значений):" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "Канал " << i << ": ";
        for (int j = 0; j < 10; ++j) {
            std::cout << normalized_pressure[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 6. Вывести все значения давления после нормализации, которые превышают 0.9 (пики)
    std::cout << "Пики давления (нормализованные значения > 0.9):" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "Канал " << i << ": ";
        std::for_each(normalized_pressure[i].begin(), normalized_pressure[i].end(),
            [](double p) {
                if (p > 0.9) std::cout << p << " ";
            });
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 7. Отсортировать средние температуры каналов и вывести канал с максимальной средней температурой
    std::vector<std::pair<double, int>> temp_with_index(5);
    for (int i = 0; i < 5; ++i) {
        temp_with_index[i] = {avg_temperatures[i], i};
    }

    std::sort(temp_with_index.begin(), temp_with_index.end(),
        [](const auto& a, const auto& b) { return a.first > b.first; });

    std::cout << "Рейтинг каналов по средней температуре:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << i+1 << ". Канал " << temp_with_index[i].second
                  << ": " << temp_with_index[i].first << "°C" << std::endl;
    }

    std::cout << std::endl << "Канал с максимальной средней температурой: " << temp_with_index[0].second
              << " (" << temp_with_index[0].first << "°C)" << std::endl;

    return 0;
}