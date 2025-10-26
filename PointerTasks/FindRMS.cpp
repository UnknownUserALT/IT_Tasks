#include <iostream>
#include <vector>
#include <cmath>

double computeRMS(const std::vector<double>& signal) {
    if (signal.empty()) {
        return 0.0;
    }
    
    const double* p = signal.data();
    size_t N = signal.size();
    
    double sum_of_squares = 0.0;
    
    for (size_t i = 0; i < N; i++) {
        double value = *(p + i);
        sum_of_squares += value * value;
    }


    double rms = std::sqrt(sum_of_squares / N);
    
    return rms;
}

int FindRMS() {
    std::vector<double> signal = {2.0, 3.0, 1.0, 4.0, 3.0};
    
    double rms_value = computeRMS(signal);
    
    std::cout << "Сигнал: ";
    for (size_t i = 0; i < signal.size(); i++) {
        std::cout << signal[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "RMS value: " << rms_value << std::endl;
    
    return 0;
}