/**
 * Представляет топливную колонку на АЗС.
 */
public class FuelPump {
    private static int nextId = 1;

    private final int id;
    private final double flowRate;     // Литров в минуту
    private final int openedAtDay;     // День, когда колонка была открыта
    private boolean busy;              // Занята ли колонка обслуживанием
    private Car currentCar;            // Обслуживаемый автомобиль (null если свободна)

    // Отслеживание статистики
    private double totalBusyTime;      // Общее время обслуживания
    private double lastBusyStartTime;  // Время начала текущего обслуживания

    public FuelPump(double flowRate, int openedAtDay) {
        this.id = nextId++;
        this.flowRate = flowRate;
        this.openedAtDay = openedAtDay;
        this.busy = false;
        this.currentCar = null;
        this.totalBusyTime = 0;
        this.lastBusyStartTime = 0;
    }

    /**
     * Сбросить счётчик ID.
     */
    public static void resetIdCounter() {
        nextId = 1;
    }

    /**
     * Начать обслуживание автомобиля.
     * @param car автомобиль для обслуживания
     * @param currentTime текущее время симуляции
     * @return время окончания обслуживания
     */
    public double serve(Car car, double currentTime) {
        this.busy = true;
        this.currentCar = car;
        this.lastBusyStartTime = currentTime;

        car.setStartServiceTime(currentTime);
        double serviceTime = car.getDesiredFuel() / flowRate;
        double endTime = currentTime + serviceTime;
        car.setEndServiceTime(endTime);

        return endTime;
    }

    /**
     * Освободить колонку после завершения обслуживания.
     * @param currentTime текущее время симуляции
     * @return обслуженный автомобиль
     */
    public Car release(double currentTime) {
        Car servedCar = this.currentCar;
        this.totalBusyTime += (currentTime - lastBusyStartTime);
        this.busy = false;
        this.currentCar = null;
        return servedCar;
    }

    /**
     * Вычислить процент загруженности колонки.
     * @param totalSimulationTime общее время симуляции в минутах
     * @param pumpStartTime когда колонка стала доступна (0 для начальных колонок)
     * @return загруженность в процентах (0-100)
     */
    public double getUtilization(double totalSimulationTime, double pumpStartTime) {
        double availableTime = totalSimulationTime - pumpStartTime;
        if (availableTime <= 0) return 0;
        return (totalBusyTime / availableTime) * 100;
    }

    // Геттеры
    public int getId() { return id; }
    public int getOpenedAtDay() { return openedAtDay; }
    public boolean isBusy() { return busy; }

    @Override
    public String toString() {
        return String.format("FuelPump[id=%d, flowRate=%.1f, busy=%s, openedDay=%d]",
                id, flowRate, busy, openedAtDay);
    }
}
