/**
 * Точка входа для симуляции АЗС.
 */
public class Main {
    public static void main(String[] args) {
        // Проверка запроса справки
        if (args.length > 0 && (args[0].equals("-h") || args[0].equals("--help") || args[0].equals("help"))) {
            printHelp();
            return;
        }

        // Проверка запроса списка сценариев
        if (args.length > 0 && (args[0].equals("-l") || args[0].equals("--list") || args[0].equals("list"))) {
            ConfigPresets.printAvailablePresets();
            return;
        }

        // Получение конфигурации
        Config config = parseArgs(args);

        System.out.println("Запуск симуляции АЗС...");
        System.out.println("Параметры:");
        System.out.println("  Длительность: " + config.simulationDays + " дней");
        System.out.println("  Начальных колонок: " + config.initialPumps);
        System.out.println("  Макс. колонок: " + config.maxPumps);
        System.out.println("  Скорость заправки: " + config.defaultFlowRate + " л/мин");
        System.out.println("  Порог ожидания: " + config.queueThreshold + " мин");
        System.out.println("  Начальный запас топлива: " + config.initialFuel + " л");
        System.out.println("  Интервал доставки: " + (config.deliveryInterval / 60) + " ч");
        System.out.println("  Seed: " + config.randomSeed);
        System.out.println();

        // Создание и запуск симуляции
        Simulation simulation = new Simulation(config);
        simulation.run();

        // Вывод результатов
        simulation.printReport();
    }

    /**
     * Вывод справки по использованию.
     */
    private static void printHelp() {
        System.out.println("Симуляция работы АЗС");
        System.out.println();
        System.out.println("Использование:");
        System.out.println("  java Main                     - запуск со стандартными параметрами");
        System.out.println("  java Main <сценарий>          - запуск с предустановленным сценарием");
        System.out.println("  java Main <дни> <колонки> ... - запуск с пользовательскими параметрами");
        System.out.println();
        System.out.println("Опции:");
        System.out.println("  -h, --help, help   - показать эту справку");
        System.out.println("  -l, --list, list   - показать список доступных сценариев");
        System.out.println();
        System.out.println("Пользовательские параметры (порядок):");
        System.out.println("  1. дни симуляции (по умолчанию: 7)");
        System.out.println("  2. начальных колонок (по умолчанию: 2)");
        System.out.println("  3. seed для генератора (по умолчанию: 42)");
        System.out.println("  4. макс. колонок (по умолчанию: 10)");
        System.out.println("  5. интервал доставки в часах (по умолчанию: 24)");
        System.out.println("  6. скорость заправки л/мин (по умолчанию: 20.0)");
        System.out.println();
        System.out.println("Примеры:");
        System.out.println("  java Main highload            - сценарий высокой нагрузки");
        System.out.println("  java Main shortage            - сценарий дефицита топлива");
        System.out.println("  java Main 14 3 100            - 14 дней, 3 колонки, seed=100");
    }

    /**
     * Разбор аргументов командной строки.
     * Поддерживает как названия сценариев, так и числовые параметры.
     */
    private static Config parseArgs(String[] args) {
        // Если аргументов нет - стандартный режим
        if (args.length == 0) {
            return ConfigPresets.standard();
        }

        // Проверяем, является ли первый аргумент названием сценария
        String firstArg = args[0];
        if (!isNumeric(firstArg)) {
            return ConfigPresets.getByName(firstArg);
        }

        // Иначе разбираем как числовые параметры
        int simulationDays = 7;
        int initialPumps = 2;
        int maxPumps = 10;
        double defaultFlowRate = 20.0;
        double queueThreshold = 12.0;
        int expansionDelayDays = 2;
        double initialFuel = 10000;
        double maxFuelCapacity = 15000;
        int deliveryIntervalHours = 24;
        double deliveryVolume = 10000;
        double deliveryDuration = 30;
        long randomSeed = 42;

        try {
            if (args.length >= 1) {
                simulationDays = Integer.parseInt(args[0]);
            }
            if (args.length >= 2) {
                initialPumps = Integer.parseInt(args[1]);
            }
            if (args.length >= 3) {
                randomSeed = Long.parseLong(args[2]);
            }
            if (args.length >= 4) {
                maxPumps = Integer.parseInt(args[3]);
            }
            if (args.length >= 5) {
                deliveryIntervalHours = Integer.parseInt(args[4]);
            }
            if (args.length >= 6) {
                defaultFlowRate = Double.parseDouble(args[5]);
            }
        } catch (NumberFormatException e) {
            System.err.println("Ошибка разбора аргументов. Использую значения по умолчанию.");
            return ConfigPresets.standard();
        }

        return new Config(simulationDays, initialPumps, maxPumps, defaultFlowRate,
                queueThreshold, expansionDelayDays, initialFuel, maxFuelCapacity,
                deliveryIntervalHours, deliveryVolume, deliveryDuration, randomSeed);
    }

    /**
     * Проверка, является ли строка числом.
     */
    private static boolean isNumeric(String str) {
        try {
            Double.parseDouble(str);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }
}
