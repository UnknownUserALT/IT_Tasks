#include <iostream>
#include <vector>
#include <iomanip>

void analyzePressure(const std::vector<double>& data, double& minVal, double& maxVal, double& avgVal) {
    if (data.empty()) {
        minVal = maxVal = avgVal = 0.0;
        return;
    }
    
    const double* p = data.data();
    size_t size = data.size();
    
    minVal = *p;
    maxVal = *p;
    double sum = 0.0;
    
    for (size_t i = 0; i < size; i++) {
        double currentValue = *(p + i);
        
        if (currentValue < minVal) {
            minVal = currentValue;
        }
        
        if (currentValue > maxVal) {
            maxVal = currentValue;
        }
        
        sum += currentValue;
    }
    
    avgVal = sum / size;
}

int AnalyzeParamsOfProcess() {
    std::vector<double> pressure = {101.3, 100.8, 101.9, 100.5, 102.0};

    double minPressure, maxPressure, avgPressure;
    
    analyzePressure(pressure, minPressure, maxPressure, avgPressure);
    
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Данные давления: ";
    for (double p : pressure) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Min pressure: " << minPressure << std::endl;
    std::cout << "Max pressure: " << maxPressure << std::endl;
    std::cout << "Average pressure: " << avgPressure << std::endl;
    
    return 0;
}