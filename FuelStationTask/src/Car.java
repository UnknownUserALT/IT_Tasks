/**
 * Представляет автомобиль, прибывающий на АЗС.
 */
public class Car {
    private static int nextId = 1;

    private final int id;
    private final double tankCapacity;    // Полный объём бака в литрах
    private final double fuelLevel;       // Текущее количество топлива в баке
    private final double desiredFuel;     // Сколько литров хочет залить
    private final double arrivalTime;     // Время прибытия на станцию

    private double startServiceTime;      // Время начала обслуживания
    private double endServiceTime;        // Время окончания обслуживания

    public Car(double tankCapacity, double fuelLevel, double desiredFuel, double arrivalTime) {
        this.id = nextId++;
        this.tankCapacity = tankCapacity;
        this.fuelLevel = fuelLevel;
        // Соблюдение инварианта: desiredFuel <= tankCapacity - fuelLevel
        double maxPossible = tankCapacity - fuelLevel;
        this.desiredFuel = Math.min(desiredFuel, maxPossible);
        this.arrivalTime = arrivalTime;
        this.startServiceTime = -1;
        this.endServiceTime = -1;
    }

    /**
     * Сгенерировать случайный автомобиль на основе конфигурации.
     */
    public static Car generate(Config config, double arrivalTime) {
        // Случайный объём бака из {40, 50, 60, 70, 80}
        int[] capacities = config.tankCapacities;
        double tankCapacity = capacities[config.random.nextInt(capacities.length)];

        // Уровень топлива: равномерно от 10% до 90% от объёма бака
        double minFuel = tankCapacity * 0.10;
        double maxFuel = tankCapacity * 0.90;
        double fuelLevel = minFuel + config.random.nextDouble() * (maxFuel - minFuel);

        // Желаемое топливо: равномерно от 5 до (tankCapacity - fuelLevel)
        double freeSpace = tankCapacity - fuelLevel;
        double minDesired = 5.0;
        double maxDesired = freeSpace;
        double desiredFuel;
        if (maxDesired <= minDesired) {
            desiredFuel = maxDesired;
        } else {
            desiredFuel = minDesired + config.random.nextDouble() * (maxDesired - minDesired);
        }

        return new Car(tankCapacity, fuelLevel, desiredFuel, arrivalTime);
    }

    /**
     * Сбросить счётчик ID (полезно для множественных запусков симуляции).
     */
    public static void resetIdCounter() {
        nextId = 1;
    }

    public double getDesiredFuel() { return desiredFuel; }
    public double getArrivalTime() { return arrivalTime; }
    public double getEndServiceTime() { return endServiceTime; }

    // Сеттеры для времени обслуживания
    public void setStartServiceTime(double time) { this.startServiceTime = time; }
    public void setEndServiceTime(double time) { this.endServiceTime = time; }

    /**
     * Вычислить время ожидания в очереди.
     */
    public double getWaitTime() {
        if (startServiceTime < 0) return 0;
        return startServiceTime - arrivalTime;
    }

    /**
     * Вычислить длительность обслуживания.
     */
    public double getServiceTime() {
        if (endServiceTime < 0 || startServiceTime < 0) return 0;
        return endServiceTime - startServiceTime;
    }

    @Override
    public String toString() {
        return String.format("Car[id=%d, tank=%.0f, fuel=%.1f, desired=%.1f, arrival=%.1f]",
                id, tankCapacity, fuelLevel, desiredFuel, arrivalTime);
    }
}
