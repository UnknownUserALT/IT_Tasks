import java.util.Random;

/**
 * Класс конфигурации, содержащий все параметры симуляции.
 * Все параметры можно изменять без изменения логики.
 */
public class Config {
    // Длительность симуляции
    public final int simulationDays;
    public final double simulationMinutes;

    // Настройки колонок
    public final int initialPumps;
    public final int maxPumps;
    public final double defaultFlowRate; // литров в минуту

    // Порог очереди для расширения
    public final double queueThreshold; // минуты
    public final double expansionDelay; // минуты (конвертировано из дней)

    // Настройки топлива
    public final double initialFuel; // литры
    public final double maxFuelCapacity; // литры
    public final double deliveryInterval; // минуты (конвертировано из часов)
    public final double deliveryVolume; // литры
    public final double deliveryDuration; // минуты

    // Генератор случайных чисел
    public final long randomSeed;
    public final Random random;

    // Варианты объёма бака
    public final int[] tankCapacities = {40, 50, 60, 70, 80};

    // Интенсивность прибытия по времени суток (авто в минуту)
    public final double nightRate = 0.03;       // 00:00 - 05:59
    public final double morningPeakRate = 0.25; // 06:00 - 09:59
    public final double dayRate = 0.12;         // 10:00 - 15:59
    public final double eveningPeakRate = 0.30; // 16:00 - 19:59
    public final double eveningRate = 0.08;     // 20:00 - 23:59

    public Config() {
        this(7, 2, 10, 20.0, 12.0, 2, 10000, 15000, 24, 10000, 30, 42);
    }

    public Config(int simulationDays, int initialPumps, int maxPumps, double defaultFlowRate,
                  double queueThreshold, int expansionDelayDays, double initialFuel,
                  double maxFuelCapacity, int deliveryIntervalHours, double deliveryVolume,
                  double deliveryDuration, long randomSeed) {
        this.simulationDays = simulationDays;
        this.simulationMinutes = simulationDays * 24 * 60;
        this.initialPumps = initialPumps;
        this.maxPumps = maxPumps;
        this.defaultFlowRate = defaultFlowRate;
        this.queueThreshold = queueThreshold;
        this.expansionDelay = expansionDelayDays * 24 * 60; // конвертация дней в минуты
        this.initialFuel = initialFuel;
        this.maxFuelCapacity = maxFuelCapacity;
        this.deliveryInterval = deliveryIntervalHours * 60; // конвертация часов в минуты
        this.deliveryVolume = deliveryVolume;
        this.deliveryDuration = deliveryDuration;
        this.randomSeed = randomSeed;
        this.random = new Random(randomSeed);
    }

    /**
     * Получить интенсивность прибытия (лямбда) в зависимости от времени суток.
     * @param timeInMinutes текущее время симуляции в минутах
     * @return интенсивность прибытия в авто/минуту
     */
    public double getArrivalRate(double timeInMinutes) {
        // Получить час суток (0-23)
        int minuteOfDay = (int) (timeInMinutes % (24 * 60));
        int hour = minuteOfDay / 60;

        if (hour >= 0 && hour < 6) {
            return nightRate;
        } else if (hour >= 6 && hour < 10) {
            return morningPeakRate;
        } else if (hour >= 10 && hour < 16) {
            return dayRate;
        } else if (hour >= 16 && hour < 20) {
            return eveningPeakRate;
        } else {
            return eveningRate;
        }
    }

    /**
     * Получить текущий день симуляции (нумерация с 1).
     */
    public int getDay(double timeInMinutes) {
        return (int) (timeInMinutes / (24 * 60)) + 1;
    }
}
