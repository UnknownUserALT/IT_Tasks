#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>
using namespace std;
int EngineVibrationAnalyze() {
    setlocale(LC_ALL, "ru");
    
        // Генератор случайных значений амплитуд вибрации
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 2.5); // Амплитуды от 0 до 2.5
    
    // Создание и заполнение вектора амплитуд вибрации (50 значений)
    std::vector<double> vibration_amplitudes(50);
    std::generate(vibration_amplitudes.begin(), vibration_amplitudes.end(), [&]() { return dis(gen); });
    
    // Вывод исходных данных
    std::cout << "ВИБРАЦИОННЫЙ АНАЛИЗ ДВИГАТЕЛЯ" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "Исходные амплитуды вибрации (50 измерений):" << std::endl;
    for (size_t i = 0; i < vibration_amplitudes.size(); ++i) {
        std::cout << std::fixed << std::setprecision(3) << std::setw(6) << vibration_amplitudes[i];
        if (vibration_amplitudes[i] < 0.1) std::cout << " ⚠ ШУМ";
        if ((i + 1) % 5 == 0) std::cout << std::endl;
        else std::cout << " | ";
    }
    std::cout << std::endl;
    
    // 1. Удаление шумов (значения < 0.1)
    int initial_count = vibration_amplitudes.size();
    vibration_amplitudes.erase(
        std::remove_if(vibration_amplitudes.begin(), vibration_amplitudes.end(),
            [](double amplitude) { return amplitude < 0.1; }),
        vibration_amplitudes.end()
    );
    int removed_noise = initial_count - vibration_amplitudes.size();
    
    std::cout << "После удаления шумов (< 0.1):" << std::endl;
    std::cout << "Удалено шумов: " << removed_noise << " измерений" << std::endl;
    std::cout << "Осталось измерений: " << vibration_amplitudes.size() << std::endl;
    
    if (!vibration_amplitudes.empty()) {
        std::cout << "Отфильтрованные амплитуды:" << std::endl;
        for (size_t i = 0; i < vibration_amplitudes.size(); ++i) {
            std::cout << std::setw(6) << vibration_amplitudes[i];
            if ((i + 1) % 8 == 0) std::cout << std::endl;
            else std::cout << " | ";
        }
        std::cout << std::endl << std::endl;
    }
    // Проверка наличия данных после фильтрации
    if (vibration_amplitudes.empty()) {
        std::cout << "Нет данных для анализа после фильтрации шумов!" << std::endl;
        return 0;
    }

    // 2. Найдите среднее и максимальное значение амплитуды
    double average_amplitude = std::accumulate(vibration_amplitudes.begin(), vibration_amplitudes.end(), 0.0) / vibration_amplitudes.size();
    auto max_amplitude_it = std::max_element(vibration_amplitudes.begin(), vibration_amplitudes.end());

    std::cout << "СТАТИСТИКА АМПЛИТУД:" << std::endl;
    std::cout << "Средняя амплитуда: " << std::setprecision(4) << average_amplitude << std::endl;
    std::cout << "Максимальная амплитуда: " << std::setprecision(4) << *max_amplitude_it << std::endl;

    // 3. Проверьте, превышают ли все значения порог 0.5
    bool all_above_threshold = std::all_of(vibration_amplitudes.begin(), vibration_amplitudes.end(),
        [](double amplitude) { return amplitude > 0.5; });

    std::cout << "АНАЛИЗ ПОРОГА 0.5:" << std::endl;
    std::cout << "Все амплитуды > 0.5: " << (all_above_threshold ? "ДА" : "НЕТ") << std::endl;

    if (!all_above_threshold) {
        int below_threshold = std::count_if(vibration_amplitudes.begin(), vibration_amplitudes.end(),
            [](double amplitude) { return amplitude <= 0.5; });

        std::cout << "Амплитуд ≤ 0.5: " << below_threshold << " измерений" << std::endl;

        // Найдем амплитуды ниже порога
        if (below_threshold > 0) {
            std::cout << "Значения ≤ 0.5: ";
            std::vector<double> low_amplitudes;
            std::copy_if(vibration_amplitudes.begin(), vibration_amplitudes.end(),
                        std::back_inserter(low_amplitudes),
                        [](double amp) { return amp <= 0.5; });

            for (size_t i = 0; i < low_amplitudes.size(); ++i) {
                std::cout << low_amplitudes[i];
                if (i < low_amplitudes.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    // 4. Выведите 10 наибольших амплитуд для анализа
    std::vector<double> sorted_amplitudes = vibration_amplitudes;
    std::sort(sorted_amplitudes.begin(), sorted_amplitudes.end(), std::greater<double>());

    int top_count = std::min(10, static_cast<int>(sorted_amplitudes.size()));

    std::cout << "ТОП-" << top_count << " НАИБОЛЬШИХ АМПЛИТУД:" << std::endl;
    std::cout << "№  | Амплитуда | Отклонение от среднего" << std::endl;
    std::cout << "---|-----------|-----------------------" << std::endl;

    for (int i = 0; i < top_count; ++i) {
        double deviation = ((sorted_amplitudes[i] - average_amplitude) / average_amplitude) * 100;
        std::cout << std::setw(2) << (i + 1) << " | "
                  << std::setw(8) << std::setprecision(4) << sorted_amplitudes[i] << " | "
                  << std::setw(6) << std::setprecision(1) << deviation << "%";

        // Маркировка опасных значений
        if (sorted_amplitudes[i] > 2.0) std::cout << " 🚨 ОПАСНО";
        else if (sorted_amplitudes[i] > 1.5) std::cout << " ⚠ ВНИМАНИЕ";
        else if (sorted_amplitudes[i] > 1.0) std::cout << " ℹ НОРМА";

        std::cout << std::endl;
    }


    return 0;
}