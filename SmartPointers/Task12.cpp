#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

int Task12() {
    std::vector<std::string> logs = {
        "INFO: Start",
        "ERROR: Disk failure",
        "WARNING: Low memory",
        "ERROR: Disk failure",
        "INFO: Stop"
    };
    
    std::shared_ptr<std::vector<std::string>> pLogs = 
        std::make_shared<std::vector<std::string>>(logs);
    
    std::vector<std::string> errors;
    
    // Лямбда-функция для фильтрации ошибок
    auto errorFilter = [](const std::string& log) {
        return log.find("ERROR") != std::string::npos;
    };
    
    // Копируем только сообщения об ошибках
    std::copy_if(pLogs->begin(), pLogs->end(), std::back_inserter(errors), errorFilter);
    
    // Удаляем дубликаты
    std::sort(errors.begin(), errors.end());
    auto last = std::unique(errors.begin(), errors.end());
    errors.erase(last, errors.end());
    
    // Вывод исходного журнала
    std::cout << "Исходный журнал:" << std::endl;
    for (const auto& log : *pLogs) {
        std::cout << log << std::endl;
    }
    
    // Вывод очищенного списка ошибок
    std::cout << "Ошибки:" << std::endl;
    for (const auto& error : errors) {
        std::cout << error << std::endl;
    }
    
    return 0;
}