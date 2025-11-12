#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <numeric>
#include <iomanip>

using namespace std;

int Task10() {
    ifstream logFile("../power_log.txt");
    
    if (!logFile.is_open()) {
        cout << "Ошибка: файл power_log.txt не найден!" << endl;
        return 1;
    }
    
    map<string, vector<double>> deviceData;
    
    string line;
    
    while (getline(logFile, line)) {
        stringstream ss(line);
        string timestamp, deviceName;
        double power;
        
        if (ss >> timestamp >> deviceName >> power) {
            deviceData[deviceName].push_back(power);
        }
    }
    
    logFile.close();
    
    ofstream reportFile("../energy_report.txt");
    
    if (!reportFile.is_open()) {
        cout << "Ошибка: не удалось создать файл energy_report.txt!" << endl;
        return 1;
    }
    
    reportFile << fixed << setprecision(2);
    
    for (const auto& device : deviceData) {
        const string& name = device.first;
        const vector<double>& powerReadings = device.second;
        
        int measurements = powerReadings.size();
        
        double totalPower = accumulate(powerReadings.begin(), powerReadings.end(), 0.0);
        
        double averagePower = totalPower / measurements;
        
        reportFile << name << ": measurements = " << measurements
                   << ", total = " << totalPower << " kWh"
                   << ", average = " << averagePower << " kW" << endl;
    }
    
    reportFile.close();
    
    cout << "Анализ завершён. Результаты сохранены в energy_report.txt" << endl;
    
    return 0;
}