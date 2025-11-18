#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

int Task11() {
    std::ifstream inputFile("../sensor_data.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл sensor_data.txt" << std::endl;
        return 1;
    }

    std::vector<double> temperatures;
    std::string line;

    while (std::getline(inputFile, line)) {
        try {
            double temp = std::stod(line);
            temperatures.push_back(temp);
        } catch (const std::invalid_argument& e) {
        } catch (const std::out_of_range& e) {
        }
    }
    inputFile.close();

    std::cout << "Исходные данные: " << temperatures.size() << " значений" << std::endl;

    auto it = std::remove_if(temperatures.begin(), temperatures.end(),
        [](double temp) {
            return temp < -50.0 || temp > 50.0;
        });

    temperatures.erase(it, temperatures.end());

    std::cout << "После фильтрации: " << temperatures.size() << " значений" << std::endl;

    if (!temperatures.empty()) {
        double sum = std::accumulate(temperatures.begin(), temperatures.end(), 0.0);
        double average = sum / temperatures.size();
        
        std::cout << "Средняя температура: " << average << "°C" << std::endl;
    } else {
        std::cout << "Нет корректных данных для вычисления средней температуры" << std::endl;
    }

    std::ofstream outputFile("../filtered_sensor_data.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл filtered_sensor_data.txt" << std::endl;
        return 1;
    }

    for (double temp : temperatures) {
        outputFile << temp << std::endl;
    }
    outputFile.close();

    std::cout << "Отфильтрованные данные сохранены в файл filtered_sensor_data.txt" << std::endl;

    return 0;
}