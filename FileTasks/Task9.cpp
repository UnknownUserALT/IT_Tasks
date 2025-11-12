#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

int Task9() {
    ifstream dataFile("../temperature_data.txt");
    
    if (!dataFile.is_open()) {
        cout << "Ошибка: файл temperature_data.txt не найден!" << endl;
        return 1;
    }
    
    map<string, vector<double>> sensorData;
    
    string line;
    
    while (getline(dataFile, line)) {
        stringstream ss(line);
        string sensorName;
        double temperature;
        
        if (ss >> sensorName >> temperature) {
            sensorData[sensorName].push_back(temperature);
        }
    }
    
    dataFile.close();
    
    ofstream reportFile("../report.txt");
    
    if (!reportFile.is_open()) {
        cout << "Ошибка: не удалось создать файл report.txt!" << endl;
        return 1;
    }
    
    for (const auto& sensor : sensorData) {
        const string& name = sensor.first;
        const vector<double>& temperatures = sensor.second;
        
        double minTemp = *min_element(temperatures.begin(), temperatures.end());
        
        double maxTemp = *max_element(temperatures.begin(), temperatures.end());
        
        double sum = accumulate(temperatures.begin(), temperatures.end(), 0.0);
        double avgTemp = sum / temperatures.size();
        
        reportFile << name << ": min = " << minTemp
                   << ", max = " << maxTemp 
                   << ", avg = " << avgTemp << endl;
    }
    
    reportFile.close();
    
    cout << "Отчёт о температурных данных сохранён в report.txt" << endl;
    
    return 0;
}