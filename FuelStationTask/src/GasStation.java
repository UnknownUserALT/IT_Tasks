import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
 * Представляет АЗС с колонками, очередью и хранилищем топлива.
 */
public class GasStation {
    private final Config config;
    private final List<FuelPump> pumps;
    private final Queue<Car> queue;

    private double fuelReserve;
    private final double maxFuelCapacity;
    private double lastDeliveryTime;

    // Отслеживание расширения
    private boolean expansionScheduled;
    private int additionalPumpsOpened;

    // Отслеживание статистики
    private int maxQueueLength;
    private double minFuelReserve;

    public GasStation(Config config) {
        this.config = config;
        this.pumps = new ArrayList<>();
        this.queue = new LinkedList<>();

        this.fuelReserve = config.initialFuel;
        this.maxFuelCapacity = config.maxFuelCapacity;
        this.lastDeliveryTime = 0;

        this.expansionScheduled = false;
        this.additionalPumpsOpened = 0;
        this.maxQueueLength = 0;
        this.minFuelReserve = config.initialFuel;

        // Создание начальных колонок
        for (int i = 0; i < config.initialPumps; i++) {
            pumps.add(new FuelPump(config.defaultFlowRate, 0));
        }
    }

    /**
     * Найти свободную колонку или вернуть null, если все заняты.
     */
    public FuelPump findFreePump() {
        for (FuelPump pump : pumps) {
            if (!pump.isBusy()) {
                return pump;
            }
        }
        return null;
    }

    /**
     * Проверить, достаточно ли топлива для автомобиля.
     */
    public boolean hasEnoughFuel(Car car) {
        return fuelReserve >= car.getDesiredFuel();
    }

    /**
     * Списать топливо после обслуживания.
     */
    public void consumeFuel(double amount) {
        fuelReserve -= amount;
        if (fuelReserve < minFuelReserve) {
            minFuelReserve = fuelReserve;
        }
    }

    /**
     * Добавить автомобиль в очередь.
     */
    public void addToQueue(Car car) {
        queue.add(car);
        if (queue.size() > maxQueueLength) {
            maxQueueLength = queue.size();
        }
    }

    /**
     * Получить следующий автомобиль из очереди.
     */
    public Car pollFromQueue() {
        return queue.poll();
    }

    /**
     * Проверить, пуста ли очередь.
     */
    public boolean isQueueEmpty() {
        return queue.isEmpty();
    }

    /**
     * Принять доставку топлива.
     */
    public void receiveFuelDelivery(double currentTime) {
        fuelReserve = Math.min(fuelReserve + config.deliveryVolume, maxFuelCapacity);
        lastDeliveryTime = currentTime;
    }

    /**
     * Добавить новую колонку.
     * @return true если колонка добавлена, false если достигнут максимум
     */
    public boolean addPump(int day) {
        if (pumps.size() >= config.maxPumps) {
            return false;
        }
        pumps.add(new FuelPump(config.defaultFlowRate, day));
        additionalPumpsOpened++;
        expansionScheduled = false; // Сброс флага
        return true;
    }

    /**
     * Проверить, можно ли запланировать расширение.
     */
    public boolean canScheduleExpansion() {
        return !expansionScheduled && pumps.size() < config.maxPumps;
    }

    /**
     * Отметить, что расширение запланировано.
     */
    public void setExpansionScheduled(boolean scheduled) {
        this.expansionScheduled = scheduled;
    }

    public boolean isExpansionScheduled() {
        return expansionScheduled;
    }

    // Геттеры
    public List<FuelPump> getPumps() { return pumps; }
    public double getFuelReserve() { return fuelReserve; }
    public int getAdditionalPumpsOpened() { return additionalPumpsOpened; }
    public int getMaxQueueLength() { return maxQueueLength; }
    public double getMinFuelReserve() { return minFuelReserve; }
    public int getPumpCount() { return pumps.size(); }
}
