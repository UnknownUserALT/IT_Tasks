#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Функция для выделения памяти под матрицу
int** allocateMatrix(int n, int t) {
    int **matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[t];
    }
    return matrix;
}

// ЭТАП 5: Функция для освобождения памяти
void freeMatrix(int **matrix, int n) {
    if (matrix == nullptr) return;

    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Функция для проверки корректности ввода
bool validateInput(int value) {
    return value >= 0 && value <= 100;
}

// Функция для ввода данных от пользователя с проверкой
void inputLoadData(int **load, int n, int t) {
    cout << "Введите нагрузку для каждого узла по интервалам (0-100%):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Узел " << i << ": ";
        for (int j = 0; j < t; j++) {
            int value;
            cin >> value;

            // Проверка корректности ввода
            while (!validateInput(value)) {
                cout << "Ошибка! Нагрузка должна быть в диапазоне 0-100%. Введите снова: ";
                cin >> value;
            }

            load[i][j] = value;
        }
    }
}

// Функция для округления до ближайшего целого
int roundToNearest(double value) {
    return static_cast<int>(round(value));
}

// Улучшенная функция для красивого вывода таблицы
void printMatrix(int **load, int n, int t, const string& title = "Матрица нагрузки:") {
    cout << title << endl;

    // Верхняя граница таблицы
    cout << "┌─────────";
    for (int j = 0; j < t; j++) {
        cout << "────────";
    }
    cout << "┐" << endl;

    // Заголовок времени
    cout << "│ Узел\\Вр ";
    for (int j = 0; j < t; j++) {
        cout << "│  t=" << setw(2) << j;
    }
    cout << "│" << endl;

    // Разделитель
    cout << "├─────────";
    for (int j = 0; j < t; j++) {
        cout << "────────";
    }
    cout << "┤" << endl;

    // Данные
    for (int i = 0; i < n; i++) {
        cout << "│ Узел " << i << "  ";
        for (int j = 0; j < t; j++) {
            cout << "│" << setw(6) << load[i][j] << "%";
        }
        cout << "│" << endl;
    }

    // Нижняя граница таблицы
    cout << "└─────────";
    for (int j = 0; j < t; j++) {
        cout << "────────";
    }
    cout << "┘" << endl << endl;
}

// ЭТАП 1: Функция для вычисления средней нагрузки каждого узла за всё время
double *averageLoadPerNode(int **load, int n, int t) {
    double *averages = new double[n];

    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < t; j++) {
            sum += *(*(load + i) + j);
        }
        averages[i] = sum / t;
    }

    return averages;
}

void normalizeLoad(int **load, int n, int t) {
    cout << "=== НОРМАЛИЗАЦИЯ НАГРУЗКИ ===" << endl;
    bool anyNormalization = false;

    for (int j = 0; j < t; j++) {
        for (int i = 0; i < n; i++) {
            // Проверяем, превышает ли нагрузка 80% в текущий момент времени
            int currentLoad = *(*(load + i) + j);
            if (currentLoad > 80) {
                if (!anyNormalization) {
                    cout << "Обнаружены перегруженные узлы:" << endl;
                    anyNormalization = true;
                }

                cout << "  Время t=" << j << ": Узел " << i << " перегружен ("
                     << currentLoad << "%)" << endl;

                // Вычисляем 10% от текущей нагрузки с округлением
                int loadToRedistribute = roundToNearest(currentLoad * 0.1);

                // Убедимся, что не уходим в отрицательные значения
                if (loadToRedistribute > currentLoad) {
                    loadToRedistribute = currentLoad;
                }

                // Вычисляем количество узлов, которые могут принять нагрузку
                int availableNodes = 0;
                for (int k = 0; k < n; k++) {
                    if (k != i) {
                        availableNodes++;
                    }
                }

                if (availableNodes > 0 && loadToRedistribute > 0) {
                    // Распределяем нагрузку равными долями на остальные узлы
                    int loadPerNode = loadToRedistribute / availableNodes;
                    int remainder = loadToRedistribute % availableNodes;

                    // Уменьшаем нагрузку на перегруженном узле
                    *(*(load + i) + j) -= loadToRedistribute;

                    // Увеличиваем нагрузку на остальных узлах
                    int nodesProcessed = 0;
                    for (int k = 0; k < n; k++) {
                        if (k != i) {
                            int addedLoad = loadPerNode;
                            if (nodesProcessed < remainder) {
                                addedLoad++;
                            }
                            *(*(load + k) + j) += addedLoad;
                            nodesProcessed++;
                        }
                    }

                    cout << "    Перераспределено " << loadToRedistribute
                         << "% на " << availableNodes << " узлов" << endl;
                }
            }
        }
    }

    if (!anyNormalization) {
        cout << "  Нормализация не требуется - нет перегруженных узлов" << endl;
    }
}
// ЭТАП 3: Функция для определения критического интервала
int findCriticalInterval(int **load, int n, int t) {
    int maxTotalLoad = 0;
    int criticalInterval = 0;

    cout << "Суммарные нагрузки по интервалам:" << endl;

    // Сначала вычисляем все суммарные нагрузки
    int *totalLoads = new int[t];
    for (int j = 0; j < t; j++) {
        totalLoads[j] = 0;
        for (int i = 0; i < n; i++) {
            totalLoads[j] += *(*(load + i) + j);
        }
        if (totalLoads[j] > maxTotalLoad) {
            maxTotalLoad = totalLoads[j];
            criticalInterval = j;
        }
    }

    // Затем выводим с правильными отметками максимума
    for (int j = 0; j < t; j++) {
        cout << "  t=" << j << ": " << totalLoads[j] << "%";
        if (totalLoads[j] == maxTotalLoad) {
            cout << " ← МАКСИМУМ";
        }
        cout << endl;
    }

    delete[] totalLoads;
    return criticalInterval;
}

// Функция для вычисления суммарной нагрузки в указанный интервал
int calculateTotalLoadAtInterval(int **load, int n, int interval) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += *(*(load + i) + interval);
    }
    return total;
}

// Функция для анализа эффективности системы
void analyzeSystem(int **load, int n, int t) {
    cout << "\n=== АНАЛИЗ ЭФФЕКТИВНОСТИ СИСТЕМЫ ===" << endl;

    // Анализ по узлам
    cout << "\nСтатистика по узлам:" << endl;
    for (int i = 0; i < n; i++) {
        int maxLoad = *(*(load + i) + 0);
        int minLoad = *(*(load + i) + 0);
        double sum = 0;

        for (int j = 0; j < t; j++) {
            int currentLoad = *(*(load + i) + j);
            maxLoad = max(maxLoad, currentLoad);
            minLoad = min(minLoad, currentLoad);
            sum += currentLoad;
        }

        double avg = sum / t;
        cout << "  Узел " << i << ": макс=" << maxLoad << "%, мин=" << minLoad
             << "%, средняя=" << fixed << setprecision(1) << avg << "%" << endl;
    }
}

int DistributionLoadInSystem() {
    int n, t;

    cout << "=== СИСТЕМА УПРАВЛЕНИЯ НАГРУЗКОЙ ===" << endl;
    cout << "=== ПОЛНАЯ ВЕРСИЯ С ВСЕМИ ЭТАПАМИ 1-5 ===" << endl << endl;

    cout << "Введите количество узлов: ";
    cin >> n;
    cout << "Введите количество временных интервалов: ";
    cin >> t;

    if (n <= 0 || t <= 0) {
        cout << "Ошибка: размеры должны быть положительными числами!" << endl;
        return 1;
    }

    int **load = allocateMatrix(n, t);

    inputLoadData(load, n, t);

    cout << "\n" << string(60, '=') << endl;
    cout << "ЭТАП 4: ВЫВОД РЕЗУЛЬТАТОВ" << endl;
    cout << string(60, '=') << endl;

    cout << "\n--- ИСХОДНЫЕ ДАННЫЕ ---" << endl;
    printMatrix(load, n, t, "Исходная матрица нагрузок:");

    cout << "--- ЭТАП 1: СРЕДНЯЯ НАГРУЗКА ПО УЗЛАМ ---" << endl;
    double *nodeAverages = averageLoadPerNode(load, n, t);
    for (int i = 0; i < n; i++) {
        cout << "  Узел " << i << ": " << fixed << setprecision(1)
             << nodeAverages[i] << "%" << endl;
    }
    cout << endl;

    cout << "--- ЭТАП 3: КРИТИЧЕСКИЙ ИНТЕРВАЛ ---" << endl;
    int originalCriticalInterval = findCriticalInterval(load, n, t);
    int originalTotalLoad = calculateTotalLoadAtInterval(load, n, originalCriticalInterval);
    cout << ">>> Критический интервал: " << originalCriticalInterval
         << " (суммарная нагрузка = " << originalTotalLoad << "%)" << endl;

    cout << "\n--- ЭТАП 2: НОРМАЛИЗАЦИЯ НАГРУЗКИ ---" << endl;
    normalizeLoad(load, n, t);

    cout << "\n--- РЕЗУЛЬТАТЫ ПОСЛЕ НОРМАЛИЗАЦИИ ---" << endl;
    printMatrix(load, n, t, "Матрица нагрузок после нормализации:");

    cout << "--- СРЕДНЯЯ НАГРУЗКА ПОСЛЕ НОРМАЛИЗАЦИИ ---" << endl;
    double *normalizedAverages = averageLoadPerNode(load, n, t);
    for (int i = 0; i < n; i++) {
        cout << "  Узел " << i << ": " << fixed << setprecision(1)
             << normalizedAverages[i] << "%" << endl;
    }

    cout << "\n--- КРИТИЧЕСКИЙ ИНТЕРВАЛ ПОСЛЕ НОРМАЛИЗАЦИИ ---" << endl;
    int normalizedCriticalInterval = findCriticalInterval(load, n, t);
    int normalizedTotalLoad = calculateTotalLoadAtInterval(load, n, normalizedCriticalInterval);
    cout << ">>> Критический интервал после нормализации: " << normalizedCriticalInterval
         << " (суммарная нагрузка = " << normalizedTotalLoad << "%)" << endl;

    analyzeSystem(load, n, t);

    cout << "\n" << string(60, '=') << endl;
    cout << "ИТОГОВЫЙ ОТЧЕТ" << endl;
    cout << string(60, '=') << endl;
    cout << "• Количество узлов: " << n << endl;
    cout << "• Количество временных интервалов: " << t << endl;
    cout << "• Исходный критический интервал: t=" << originalCriticalInterval
         << " (нагрузка: " << originalTotalLoad << "%)" << endl;
    cout << "• Критический интервал после нормализации: t=" << normalizedCriticalInterval
         << " (нагрузка: " << normalizedTotalLoad << "%)" << endl;
    cout << "• Изменение суммарной нагрузки: "
         << (normalizedTotalLoad - originalTotalLoad) << "%" << endl;

    cout << "\n" << string(60, '=') << endl;
    cout << "ЭТАП 5: ОСВОБОЖДЕНИЕ ПАМЯТИ" << endl;
    cout << string(60, '=') << endl;

    freeMatrix(load, n);
    delete[] nodeAverages;
    delete[] normalizedAverages;

    cout << "✓ Память успешно освобождена" << endl;
    cout << "✓ Программа завершена корректно" << endl;
    
    return 0;
}