#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <random>

// Лямбда-функция для измерения времени выполнения
auto measureTime = [](const std::string& operationName, auto&& operation) {
    auto start = std::chrono::steady_clock::now();
    operation();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << operationName << ": " << duration.count() << " ms" << std::endl;
    return duration;
};

int Task12() {
    const size_t DATA_SIZE = 100000;
    
    std::vector<int> data;
    data.reserve(DATA_SIZE);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000);
    
    std::cout << "Генерация " << DATA_SIZE << " случайных чисел..." << std::endl;
    for (size_t i = 0; i < DATA_SIZE; ++i) {
        data.push_back(dist(gen));
    }
    std::cout << "Генерация завершена." << std::endl << std::endl;

    // Метод 1: Запись каждого числа по отдельности (текстовый формат)
    measureTime("Запись по одному (текстовый)", [&data]() {
        std::ofstream file("../method1_single_text.txt");
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла method1_single_text.txt" << std::endl;
            return;
        }
        for (int num : data) {
            file << num << " ";
        }
        file.close();
    });

    // Метод 2: Запись всех чисел сразу (текстовый формат)
    measureTime("Запись всех сразу (текстовый)", [&data]() {
        std::ofstream file("../method2_bulk_text.txt");
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла method2_bulk_text.txt" << std::endl;
            return;
        }
        std::string buffer;
        buffer.reserve(data.size() * 8); // Предварительное выделение памяти
        
        for (int num : data) {
            buffer += std::to_string(num) + " ";
        }
        file << buffer;
        file.close();
    });

    // Метод 3: Запись каждого числа по отдельности (бинарный формат)
    measureTime("Запись по одному (бинарный)", [&data]() {
        std::ofstream file("method3_single_binary.bin", std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла method3_single_binary.bin" << std::endl;
            return;
        }
        for (int num : data) {
            file.write(reinterpret_cast<const char*>(&num), sizeof(num));
        }
        file.close();
    });

    // Метод 4: Запись всех чисел сразу (бинарный формат)
    measureTime("Запись всех сразу (бинарный)", [&data]() {
        std::ofstream file("method4_bulk_binary.bin", std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла method4_bulk_binary.bin" << std::endl;
            return;
        }
        file.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(int));
        file.close();
    });

    std::cout << "\n=== СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ===" << std::endl;
    
    std::vector<long long> times;
    
    // Измеряем метод 1 (по одному, текстовый)
    auto time1 = measureTime("Метод 1 - По одному (текст)", [&data]() {
        std::ofstream file("test1.txt");
        for (int num : data) {
            file << num << " ";
        }
    }).count();

    // Измеряем метод 2 (все сразу, текстовый)
    auto time2 = measureTime("Метод 2 - Все сразу (текст)", [&data]() {
        std::ofstream file("test2.txt");
        std::string buffer;
        for (int num : data) {
            buffer += std::to_string(num) + " ";
        }
        file << buffer;
    }).count();

    // Измеряем метод 3 (по одному, бинарный)
    auto time3 = measureTime("Метод 3 - По одному (бинар)", [&data]() {
        std::ofstream file("test3.bin", std::ios::binary);
        for (int num : data) {
            file.write(reinterpret_cast<const char*>(&num), sizeof(num));
        }
    }).count();

    // Измеряем метод 4 (все сразу, бинарный)
    auto time4 = measureTime("Метод 4 - Все сразу (бинар)", [&data]() {
        std::ofstream file("test4.bin", std::ios::binary);
        file.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(int));
    }).count();

    std::cout << "\n=== ИТОГОВЫЕ РЕЗУЛЬТАТЫ ===" << std::endl;
    std::cout << "Самый быстрый метод: ";
    
    if (time4 <= time3 && time4 <= time2 && time4 <= time1) {
        std::cout << "Запись всех сразу (бинарный)" << std::endl;
    } else if (time3 <= time2 && time3 <= time1) {
        std::cout << "Запись по одному (бинарный)" << std::endl;
    } else if (time2 <= time1) {
        std::cout << "Запись всех сразу (текстовый)" << std::endl;
    } else {
        std::cout << "Запись по одному (текстовый)" << std::endl;
    }

    std::cout << "\nОтносительная производительность:" << std::endl;
    std::cout << "Бинарная запись всех сразу в " << static_cast<double>(time1) / time4 << " раз быстрее записи по одному (текст)" << std::endl;

    return 0;
}