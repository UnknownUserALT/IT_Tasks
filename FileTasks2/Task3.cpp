#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct PartInfo {
    string filename;
    int lineCount;
    long long fileSize;
};

class LogSplitter {
    string inputFile;
    long long maxPartSize;
    vector<PartInfo> parts;

public:
    LogSplitter(const string& input, long long partSize)
        : inputFile(input), maxPartSize(partSize) {}

    bool split() {
        ifstream inFile(inputFile, ios::binary);
        if (!inFile.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << inputFile << endl;
            return false;
        }

        // Получаем размер файла
        inFile.seekg(0, ios::end);
        long long totalSize = inFile.tellg();
        inFile.seekg(0, ios::beg);

        cout << "Размер файла: " << totalSize << " байт" << endl;
        cout << "Максимальный размер части: " << maxPartSize << " байт" << endl;
        cout << "\nНачинаем разделение...\n" << endl;

        int partNumber = 1;
        long long bytesRead = 0;

        while (bytesRead < totalSize) {
            string partFilename = "log_part" + to_string(partNumber) + ".txt";
            ofstream outFile(partFilename);

            if (!outFile.is_open()) {
                cerr << "Ошибка: не удалось создать файл " << partFilename << endl;
                inFile.close();
                return false;
            }

            long long currentPartSize = 0;
            int lineCount = 0;
            string line;

            while (getline(inFile, line)) {
                long long lineSize = line.length() + 1;

                if (currentPartSize + lineSize > maxPartSize && lineCount > 0) {
                    inFile.seekg(-(lineSize), ios::cur);
                    break;
                }

                outFile << line << endl;
                currentPartSize += lineSize;
                lineCount++;

                long long currentPos = inFile.tellg();
                if (currentPos == -1) {
                    bytesRead = totalSize;
                    break;
                }
                bytesRead = currentPos;
            }

            outFile.close();

            PartInfo info;
            info.filename = partFilename;
            info.lineCount = lineCount;
            info.fileSize = currentPartSize;
            parts.push_back(info);

            cout << "Создан файл: " << partFilename
                 << " (строк: " << lineCount
                 << ", размер: " << currentPartSize << " байт)" << endl;

            partNumber++;

            if (inFile.eof() || bytesRead >= totalSize) {
                break;
            }
        }

        inFile.close();
        cout << "\nРазделение завершено! Создано частей: " << parts.size() << endl;
        return true;
    }

    bool generateIndex() {
        ofstream indexFile("log_index.txt");
        if (!indexFile.is_open()) {
            cerr << "Ошибка: не удалось создать файл индекса" << endl;
            return false;
        }

        indexFile << "=== ИНДЕКС ЛОГ-ФАЙЛОВ ===" << endl;
        indexFile << "Исходный файл: " << inputFile << endl;
        indexFile << "Количество частей: " << parts.size() << endl;
        indexFile << "Максимальный размер части: " << maxPartSize << " байт" << endl;
        indexFile << "\n" << string(70, '=') << endl;
        indexFile << left << setw(20) << "Файл"
                  << setw(15) << "Строк"
                  << setw(20) << "Размер (байт)" << endl;
        indexFile << string(70, '-') << endl;

        long long totalLines = 0;
        long long totalBytes = 0;

        for (const auto& part : parts) {
            indexFile << left << setw(20) << part.filename
                      << setw(15) << part.lineCount
                      << setw(20) << part.fileSize << endl;
            totalLines += part.lineCount;
            totalBytes += part.fileSize;
        }

        indexFile << string(70, '=') << endl;
        indexFile << "ИТОГО: " << totalLines << " строк, "
                  << totalBytes << " байт" << endl;

        indexFile.close();
        cout << "\nФайл индекса создан: log_index.txt" << endl;
        return true;
    }

    // Получение информации о частях
    const vector<PartInfo>& getParts() const {
        return parts;
    }
};

// Функция для создания тестового лог-файла
void createTestLog(const string& filename, int lineCount) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось создать тестовый файл" << endl;
        return;
    }

    for (int i = 1; i <= lineCount; i++) {
        file << "[2024-11-18 12:34:" << setfill('0') << setw(2) << (i % 60)
             << "] INFO: Запись лога номер " << i
             << " - Обработка данных пользователя" << endl;
    }

    file.close();
    cout << "Создан тестовый файл " << filename
         << " с " << lineCount << " строками" << endl;
}

int Task3() {
    string inputFile = "../application.log";
    long long partSize = 1048;

    cout << "=== СОЗДАНИЕ ТЕСТОВОГО ФАЙЛА ===" << endl;
    createTestLog(inputFile, 100);
    cout << endl;

    cout << "=== РАЗДЕЛЕНИЕ ФАЙЛА ===" << endl;
    LogSplitter splitter(inputFile, partSize);

    if (splitter.split()) {
        splitter.generateIndex();

        cout << "\n=== СТАТИСТИКА ===" << endl;
        const auto& parts = splitter.getParts();
        for (size_t i = 0; i < parts.size(); i++) {
            cout << "Часть " << (i + 1) << ": "
                 << parts[i].filename << " - "
                 << parts[i].lineCount << " строк, "
                 << parts[i].fileSize << " байт" << endl;
        }
    } else {
        cerr << "Ошибка при разделении файла" << endl;
        return 1;
    }

    return 0;
}