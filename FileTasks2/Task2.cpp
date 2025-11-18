#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int Task2() {

    std::string filename = "../config.txt";
    std::string key = "timeout";
    std::string newValue = "60";

    std::fstream file(filename, std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 0;
    }

    std::vector<std::string> lines;
    std::string line;
    bool parameterFound = false;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.clear();
    file.seekp(0);

    for (size_t i = 0; i < lines.size(); ++i) {
        size_t pos = lines[i].find(key + "=");
        if (pos == 0) {
            lines[i] = key + "=" + newValue;
            parameterFound = true;
            std::cout << "Заменено: " << key << "=" << newValue << std::endl;
            break;
        }
    }

    if (!parameterFound) {
        std::cerr << "Параметр '" << key << "' не найден!" << std::endl;
        file.close();
        return 0;
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        file << lines[i];
        if (i != lines.size() - 1) {
            file << '\n';
        }
    }

    file.seekp(file.tellp());
    file.close();

    std::cout << "Изменения сохранены в файле: " << filename << std::endl;
    return 0;
}