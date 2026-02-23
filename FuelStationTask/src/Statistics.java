import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Собирает и вычисляет статистику симуляции.
 */
public class Statistics {
    private final Config config;

    // Счётчики
    private int totalArrivals;
    private int totalServed;
    private int totalRefusals;
    private int fuelDeliveries;

    // Отслеживание времени ожидания
    private double totalWaitTime;
    private double maxWaitTime;

    // Отслеживание времени обслуживания
    private double totalServiceTime;

    // Отслеживание топлива (для вычисления среднего)
    private final List<Double> fuelReserveAtDeliveries;

    // Статистика по дням
    private final Map<Integer, DailyStats> dailyStats;

    public Statistics(Config config) {
        this.config = config;
        this.totalArrivals = 0;
        this.totalServed = 0;
        this.totalRefusals = 0;
        this.fuelDeliveries = 0;
        this.totalWaitTime = 0;
        this.maxWaitTime = 0;
        this.totalServiceTime = 0;
        this.fuelReserveAtDeliveries = new ArrayList<>();
        this.dailyStats = new HashMap<>();
    }

    /**
     * Записать прибытие автомобиля.
     */
    public void recordArrival(double time) {
        totalArrivals++;
        int day = config.getDay(time);
        getDailyStats(day).arrivals++;
    }

    /**
     * Записать отказ (автомобиль уехал из-за нехватки топлива).
     */
    public void recordRefusal(double time) {
        totalRefusals++;
        int day = config.getDay(time);
        getDailyStats(day).refusals++;
    }

    /**
     * Записать обслуженный автомобиль.
     */
    public void recordServed(Car car) {
        totalServed++;
        double waitTime = car.getWaitTime();
        double serviceTime = car.getServiceTime();

        totalWaitTime += waitTime;
        totalServiceTime += serviceTime;

        if (waitTime > maxWaitTime) {
            maxWaitTime = waitTime;
        }

        int day = config.getDay(car.getEndServiceTime());
        DailyStats ds = getDailyStats(day);
        ds.served++;
        ds.totalWaitTime += waitTime;
    }

    /**
     * Записать доставку топлива.
     */
    public void recordFuelDelivery(double fuelReserveBefore) {
        fuelDeliveries++;
        fuelReserveAtDeliveries.add(fuelReserveBefore);
    }

    /**
     * Записать остаток топлива на конец дня (для дневной статистики).
     */
    public void recordDailyFuelReserve(int day, double fuelReserve) {
        getDailyStats(day).endOfDayFuelReserve = fuelReserve;
    }

    private DailyStats getDailyStats(int day) {
        return dailyStats.computeIfAbsent(day, k -> new DailyStats());
    }

    public double getAverageWaitTime() {
        return totalServed > 0 ? totalWaitTime / totalServed : 0;
    }

    public double getAverageServiceTime() {
        return totalServed > 0 ? totalServiceTime / totalServed : 0;
    }

    public double getAverageFuelReserveAtDelivery() {
        if (fuelReserveAtDeliveries.isEmpty()) return 0;
        double sum = 0;
        for (double val : fuelReserveAtDeliveries) {
            sum += val;
        }
        return sum / fuelReserveAtDeliveries.size();
    }

    /**
     * Вывести итоговый отчёт.
     */
    public void printReport(GasStation station, double simulationEndTime) {
        System.out.println();
        System.out.println("=== Результаты симуляции АЗС (" + config.simulationDays + " дней) ===");
        System.out.printf("  Прибыло автомобилей:      %d%n", totalArrivals);
        System.out.printf("  Обслужено:                %d%n", totalServed);
        System.out.printf("  Отказов (нет топлива):    %d%n", totalRefusals);
        System.out.printf("  Ср. ожидание в очереди:   %.1f мин%n", getAverageWaitTime());
        System.out.printf("  Макс. ожидание:           %.1f мин%n", maxWaitTime);
        System.out.printf("  Ср. время обслуживания:   %.1f мин%n", getAverageServiceTime());
        System.out.printf("  Макс. длина очереди:      %d%n", station.getMaxQueueLength());
        System.out.printf("  Колонок в начале:         %d%n", config.initialPumps);

        int finalPumps = station.getPumpCount();
        int added = station.getAdditionalPumpsOpened();
        if (added > 0) {
            // Найти когда были открыты колонки
            StringBuilder pumpInfo = new StringBuilder();
            for (FuelPump pump : station.getPumps()) {
                if (pump.getOpenedAtDay() > 0) {
                    if (pumpInfo.length() > 0) pumpInfo.append(", ");
                    pumpInfo.append("день ").append(pump.getOpenedAtDay());
                }
            }
            System.out.printf("  Колонок в конце:          %d (+%d открыто: %s)%n",
                    finalPumps, added, pumpInfo.toString());
        } else {
            System.out.printf("  Колонок в конце:          %d%n", finalPumps);
        }

        System.out.printf("  Доставок топлива:         %d%n", fuelDeliveries);
        System.out.printf("  Мин. остаток топлива:     %.0f л%n", station.getMinFuelReserve());
        System.out.printf("  Ср. остаток при доставке: %.0f л%n", getAverageFuelReserveAtDelivery());

        // Загрузка колонок
        System.out.print("  Загрузка колонок:         ");
        for (FuelPump pump : station.getPumps()) {
            double pumpStartTime = pump.getOpenedAtDay() * 24 * 60.0;
            double utilization = pump.getUtilization(simulationEndTime, pumpStartTime);
            System.out.printf("#%d: %.0f%%  ", pump.getId(), utilization);
        }
        System.out.println();

        // Подневная разбивка (бонус)
        printDailyBreakdown();
    }

    /**
     * Вывести статистику по дням.
     */
    private void printDailyBreakdown() {
        System.out.println();
        System.out.println("=== Статистика по дням ===");
        System.out.println("День | Прибыло | Обслужено | Отказы | Ср. ожидание | Остаток топлива");
        System.out.println("-----|---------|-----------|--------|--------------|----------------");

        for (int day = 1; day <= config.simulationDays; day++) {
            DailyStats ds = dailyStats.get(day);
            if (ds == null) {
                ds = new DailyStats();
            }
            double avgWait = ds.served > 0 ? ds.totalWaitTime / ds.served : 0;
            System.out.printf("%4d | %7d | %9d | %6d | %10.1f мин | %14.0f л%n",
                    day, ds.arrivals, ds.served, ds.refusals, avgWait, ds.endOfDayFuelReserve);
        }
    }

    /**
     * Внутренний класс для дневной статистики.
     */
    private static class DailyStats {
        int arrivals = 0;
        int served = 0;
        int refusals = 0;
        double totalWaitTime = 0;
        double endOfDayFuelReserve = 0;
    }
}
