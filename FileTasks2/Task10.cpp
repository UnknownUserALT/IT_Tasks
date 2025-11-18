#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ConfigManager {
    std::string filename;

public:
    ConfigManager(const std::string& file = "../config.json") : filename(file) {}

    void createConfig() {
        std::ofstream file(filename);
        file << "{\n";
        file << " \"app_name\": \"My Application\",\n";
        file << " \"version\": \"1.0.0\",\n";
        file << " \"settings\": {\n";
        file << " \"max_connections\": 100,\n";
        file << " \"timeout\": 30,\n";
        file << " \"debug_mode\": true\n";
        file << " },\n";
        file << " \"database\": {\n";
        file << " \"host\": \"localhost\",\n";
        file << " \"port\": 5432,\n";
        file << " \"username\": \"admin\"\n";
        file << " }\n";
        file << "}";
        file.close();
        std::cout << "Конфигурационный файл создан: " << filename << "\n";
    }

    void readConfig() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Ошибка: Не удалось открыть файл " << filename << "\n";
            return;
        }

        std::cout << "\n=== Содержимое конфигурационного файла ===\n";
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << "\n";
        }
        file.close();
    }

    bool updateConfigValue(const std::string& key, const std::string& newValue) {
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            std::cout << "Ошибка: Не удалось открыть файл для чтения\n";
            return false;
        }

        std::stringstream buffer;
        std::string line;
        bool keyFound = false;

        while (std::getline(inputFile, line)) {
            // Ищем строку, содержащую нужный ключ
            size_t pos = line.find("\"" + key + "\"");
            if (pos != std::string::npos) {
                keyFound = true;
                
                // Находим позицию двоеточия после ключа
                size_t colonPos = line.find(':', pos);
                if (colonPos != std::string::npos) {
                    // Находим позицию значения (после двоеточия)
                    size_t valueStart = line.find_first_not_of(" \t", colonPos + 1);
                    
                    if (valueStart != std::string::npos) {
                        // Определяем конец текущего значения
                        size_t valueEnd = line.find_last_not_of(" \t,");
                        if (valueEnd != std::string::npos && valueEnd >= valueStart) {
                            // Заменяем значение
                            std::string beforeValue = line.substr(0, valueStart);
                            std::string afterValue = (valueEnd + 1 < line.length()) ? 
                                                   line.substr(valueEnd + 1) : "";
                            
                            line = beforeValue + newValue + afterValue;
                        }
                    }
                }
            }
            buffer << line << "\n";
        }
        inputFile.close();

        if (!keyFound) {
            std::cout << "Ключ '" << key << "' не найден в конфигурационном файле\n";
            return false;
        }

        std::ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            std::cout << "Ошибка: Не удалось открыть файл для записи\n";
            return false;
        }

        outputFile << buffer.str();
        outputFile.close();

        std::cout << "Значение '" << key << "' успешно изменено на: " << newValue << "\n";
        return true;
    }

    void updateTimeout(int newTimeout) {
        updateConfigValue("timeout", std::to_string(newTimeout));
    }

    void updateDebugMode(bool debug) {
        updateConfigValue("debug_mode", debug ? "true" : "false");
    }

    void updateHost(const std::string& newHost) {
        updateConfigValue("host", "\"" + newHost + "\"");
    }
};

int Task10() {
    ConfigManager configManager;

    configManager.createConfig();

    configManager.readConfig();

    std::cout << "\n=== Изменение настроек ===\n";
    configManager.updateTimeout(60);  // Меняем timeout с 30 на 60
    configManager.updateDebugMode(false);  // Выключаем debug_mode
    configManager.updateHost("127.0.0.1");  // Меняем host

    configManager.readConfig();

    return 0;
}