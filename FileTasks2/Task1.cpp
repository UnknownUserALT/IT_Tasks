#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility> // для std::pair

class TextAnalyzer {
    std::string filename;
    std::string search_word;
    std::vector<std::pair<int, int>> positions;

public:
    TextAnalyzer(const std::string& filename, const std::string& search_word)
        : filename(filename), search_word(search_word) {}

    bool analyze() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
            return false;
        }

        std::string line;
        int line_number = 1;

        while (std::getline(file, line)) {
            std::streampos line_start_pos = file.tellg();

            findWordInLine(line, line_number);

            if (!file.eof()) {
                file.seekg(line_start_pos);
            }

            line_number++;
        }

        file.close();
        return true;
    }

    void findWordInLine(const std::string& line, int line_number) {
        size_t pos = 0;

        while ((pos = line.find(search_word, pos)) != std::string::npos) {
            if (isWholeWord(line, pos, search_word.length())) {
                positions.emplace_back(line_number, static_cast<int>(pos + 1));
            }
            pos += search_word.length();
        }
    }

    bool isWholeWord(const std::string& line, size_t pos, size_t word_length) {
        if (pos > 0 && std::isalnum(line[pos - 1])) {
            return false;
        }

        size_t after_pos = pos + word_length;
        if (after_pos < line.length() && std::isalnum(line[after_pos])) {
            return false;
        }

        return true;
    }

    bool saveResults(const std::string& output_filename) {
        std::ofstream outfile(output_filename);
        if (!outfile.is_open()) {
            std::cerr << "Ошибка: не удалось создать файл результатов " << output_filename << std::endl;
            return false;
        }

        outfile << "Результаты поиска слова: \"" << search_word << "\"\n";
        outfile << "========================================\n";

        for (const auto& position : positions) {
            outfile << "Строка: " << position.first
                    << ", Позиция: " << position.second
                    << ", Слово: " << search_word << "\n";
        }

        outfile << "========================================\n";
        outfile << "Общее количество вхождений: " << positions.size() << "\n";

        outfile.close();
        return true;
    }

    void printResults() const {
        std::cout << "Найдено вхождений слова \"" << search_word << "\": " << positions.size() << std::endl;
        std::cout << "Позиции (строка, позиция):" << std::endl;

        for (const auto& position : positions) {
            std::cout << "Строка " << position.first << ", позиция " << position.second << std::endl;
        }
    }

    size_t getCount() const {
        return positions.size();
    }
};

int Task1() {
    std::string filename, search_word, output_filename;

    filename = "../task1.txt";
    search_word = "герой";
    output_filename = "../result.txt";

    TextAnalyzer analyzer(filename, search_word);

    if (analyzer.analyze()) {
        if (analyzer.saveResults(output_filename)) {
            std::cout << "Результаты успешно сохранены в файл: " << output_filename << std::endl;
        }

        analyzer.printResults();
    } else {
        std::cerr << "Анализ завершился с ошибкой." << std::endl;
        return 1;
    }

    return 0;
}