#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

using namespace std;

int Task11() {
    ifstream dataFile("../vibration_log.txt");
    
    if (!dataFile.is_open()) {
        cout << "Ошибка: файл vibration_log.txt не найден!" << endl;
        return 1;
    }
    
    vector<double> amplitudes;
    double value;
    
    while (dataFile >> value) {
        amplitudes.push_back(value);
    }
    
    dataFile.close();
    
    ofstream reportFile("../vibration_report.txt");
    
    if (!reportFile.is_open()) {
        cout << "Ошибка: не удалось создать файл vibration_report.txt!" << endl;
        return 1;
    }
    
    reportFile << fixed << setprecision(4);
    
    int totalMeasurements = amplitudes.size();
    
    vector<double> filteredAmplitudes;
    copy_if(amplitudes.begin(), amplitudes.end(), 
             back_inserter(filteredAmplitudes),
             [](double amp) { return amp > 0.05; });
    
    // 4. Вычисление среднего значения ОТФИЛЬТРОВАННЫХ измерений
    double sum = accumulate(filteredAmplitudes.begin(), filteredAmplitudes.end(), 0.0);
    double average = filteredAmplitudes.empty() ? 0.0 : sum / filteredAmplitudes.size();
    
    auto [minIt, maxIt] = minmax_element(amplitudes.begin(), amplitudes.end());
    double minAmplitude = *minIt;
    double maxAmplitude = *maxIt;
    
    reportFile << "Количество измерений: " << totalMeasurements << endl;
    
    reportFile << "Фильтрованные значения (>0.05): ";
    for_each(filteredAmplitudes.begin(), filteredAmplitudes.end(),
             [&reportFile](double amp) { reportFile << amp << " "; });
    reportFile << endl;
    
    reportFile << "Среднее значение: " << average << endl;
    reportFile << "Минимум: " << minAmplitude << endl;
    reportFile << "Максимум: " << maxAmplitude << endl;
    
    reportFile.close();
    
    cout << "Анализ вибрационных данных завершён. Отчёт сохранён в vibration_report.txt" << endl;
    
    return 0;
}