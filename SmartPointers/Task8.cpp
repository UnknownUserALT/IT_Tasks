#include <iostream>
#include <memory>
#include <iomanip>

// Функция для вычисления среднего значения массива
double calculateAverage(const std::unique_ptr<double[]>& arr, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
    }
    return sum / size;
}

int Task8() {
    const int DATA_SIZE = 5;
    
    // Создаем три unique_ptr для разных типов данных
    std::unique_ptr<double[]> temperatures = std::make_unique<double[]>(DATA_SIZE);
    std::unique_ptr<double[]> pressures = std::make_unique<double[]>(DATA_SIZE);
    std::unique_ptr<double[]> humidities = std::make_unique<double[]>(DATA_SIZE);
    
    // Заполняем массив температур вручную
    temperatures[0] = 20.5;
    temperatures[1] = 22.3;
    temperatures[2] = 19.8;
    temperatures[3] = 21.7;
    temperatures[4] = 23.1;
    
    // Заполняем массив давлений вручную
    pressures[0] = 1013.2;
    pressures[1] = 1012.8;
    pressures[2] = 1014.1;
    pressures[3] = 1013.5;
    pressures[4] = 1012.9;
    
    // Заполняем массив влажности вручную
    humidities[0] = 65.0;
    humidities[1] = 68.5;
    humidities[2] = 62.3;
    humidities[3] = 70.1;
    humidities[4] = 66.8;
    
    // Вычисляем средние значения
    double avgTemp = calculateAverage(temperatures, DATA_SIZE);
    double avgPressure = calculateAverage(pressures, DATA_SIZE);
    double avgHumidity = calculateAverage(humidities, DATA_SIZE);
    
    // Выводим исходные данные
    std::cout << "Исходные данные:" << std::endl;
    std::cout << std::setw(4) << "№" << std::setw(12) << "Температура" 
              << std::setw(10) << "Давление" << std::setw(12) << "Влажность" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    for (int i = 0; i < DATA_SIZE; ++i) {
        std::cout << std::setw(4) << i + 1 
                  << std::setw(12) << std::fixed << std::setprecision(1) << temperatures[i]
                  << std::setw(10) << std::setprecision(1) << pressures[i]
                  << std::setw(12) << std::setprecision(1) << humidities[i] << std::endl;
    }
    
    // Выводим результаты в виде таблицы
    std::cout << "\n\nРезультаты:" << std::endl;
    std::cout << std::string(35, '-') << std::endl;
    std::cout << std::setw(15) << "Параметр" << std::setw(18) << "Среднее значение" << std::endl;
    std::cout << std::string(35, '-') << std::endl;
    
    std::cout << std::setw(15) << "Температура" 
              << std::setw(18) << std::fixed << std::setprecision(2) << avgTemp << " °C" << std::endl;
    
    std::cout << std::setw(15) << "Давление" 
              << std::setw(18) << std::setprecision(2) << avgPressure << " hPa" << std::endl;
    
    std::cout << std::setw(15) << "Влажность" 
              << std::setw(18) << std::setprecision(2) << avgHumidity << " %" << std::endl;
    
    std::cout << std::string(35, '-') << std::endl;
    
    return 0;
}