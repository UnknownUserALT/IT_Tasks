import java.util.PriorityQueue;

/**
 * Главный движок симуляции на основе дискретно-событийного моделирования.
 */
public class Simulation {
    private final Config config;
    private final GasStation station;
    private final CarGenerator carGenerator;
    private final Statistics stats;
    private final PriorityQueue<Event> eventQueue;

    private double currentTime;
    private int lastRecordedDay;

    public Simulation(Config config) {
        // Сброс счётчиков для воспроизводимости (до создания станции)
        Car.resetIdCounter();
        FuelPump.resetIdCounter();

        this.config = config;
        this.station = new GasStation(config);
        this.carGenerator = new CarGenerator(config);
        this.stats = new Statistics(config);
        this.eventQueue = new PriorityQueue<>();
        this.currentTime = 0;
        this.lastRecordedDay = 0;
    }

    /**
     * Запустить симуляцию.
     */
    public void run() {

        // Планирование начальных событий
        double firstArrival = carGenerator.generateFirstArrival();
        eventQueue.add(new Event(EventType.CAR_ARRIVAL, firstArrival));
        eventQueue.add(new Event(EventType.FUEL_DELIVERY, config.deliveryInterval));

        // Главный цикл симуляции
        while (!eventQueue.isEmpty() && currentTime < config.simulationMinutes) {
            Event event = eventQueue.poll();
            currentTime = event.getTime();

            // Проверка превышения времени симуляции
            if (currentTime >= config.simulationMinutes) {
                break;
            }

            // Запись остатка топлива на конец каждого дня
            int currentDay = config.getDay(currentTime);
            while (lastRecordedDay < currentDay - 1) {
                lastRecordedDay++;
                stats.recordDailyFuelReserve(lastRecordedDay, station.getFuelReserve());
            }

            // Обработка события по типу
            switch (event.getType()) {
                case CAR_ARRIVAL:
                    handleCarArrival();
                    break;
                case SERVICE_END:
                    handleServiceEnd(event);
                    break;
                case FUEL_DELIVERY:
                    handleFuelDelivery();
                    break;
                case PUMP_OPEN:
                    handlePumpOpen();
                    break;
            }
        }

        // Запись статистики за последние дни
        for (int day = lastRecordedDay + 1; day <= config.simulationDays; day++) {
            stats.recordDailyFuelReserve(day, station.getFuelReserve());
        }
    }

    /**
     * Обработка события прибытия автомобиля.
     */
    private void handleCarArrival() {
        // Генерация автомобиля
        Car car = carGenerator.generateCar(currentTime);
        stats.recordArrival(currentTime);

        // Проверка наличия достаточного количества топлива
        if (!station.hasEnoughFuel(car)) {
            stats.recordRefusal(currentTime);
        } else {
            // Поиск свободной колонки
            FuelPump pump = station.findFreePump();
            if (pump != null) {
                // Немедленное начало обслуживания
                double endTime = pump.serve(car, currentTime);
                eventQueue.add(new Event(EventType.SERVICE_END, endTime, pump, car));
            } else {
                // Добавление в очередь
                station.addToQueue(car);
            }
        }

        // Планирование следующего прибытия
        double interval = carGenerator.generateInterval(currentTime);
        double nextArrival = currentTime + interval;
        if (nextArrival < config.simulationMinutes) {
            eventQueue.add(new Event(EventType.CAR_ARRIVAL, nextArrival));
        }
    }

    /**
     * Обработка события завершения обслуживания.
     */
    private void handleServiceEnd(Event event) {
        FuelPump pump = event.getPump();
        Car car = event.getCar();

        // Списание топлива
        station.consumeFuel(car.getDesiredFuel());

        // Запись статистики
        stats.recordServed(car);

        // Освобождение колонки
        pump.release(currentTime);

        // Проверка наличия автомобилей в очереди
        if (!station.isQueueEmpty()) {
            Car nextCar = station.pollFromQueue();

            // Проверка времени ожидания для срабатывания расширения
            double waitTime = currentTime - nextCar.getArrivalTime();
            if (waitTime > config.queueThreshold && station.canScheduleExpansion()) {
                // Планирование открытия колонки через задержку расширения
                double openTime = currentTime + config.expansionDelay;
                if (openTime < config.simulationMinutes) {
                    eventQueue.add(new Event(EventType.PUMP_OPEN, openTime));
                    station.setExpansionScheduled(true);
                }
            }

            // Проверка наличия топлива для ожидающего автомобиля
            if (station.hasEnoughFuel(nextCar)) {
                // Начало обслуживания следующего автомобиля
                double endTime = pump.serve(nextCar, currentTime);
                eventQueue.add(new Event(EventType.SERVICE_END, endTime, pump, nextCar));
            } else {
                // Автомобиль уезжает из-за нехватки топлива
                stats.recordRefusal(currentTime);
            }
        }
    }

    /**
     * Обработка события доставки топлива.
     */
    private void handleFuelDelivery() {
        // Запись уровня топлива до доставки
        stats.recordFuelDelivery(station.getFuelReserve());

        // Приём доставки (для простоты добавляем топливо сразу,
        // следующая доставка планируется по интервалу)
        station.receiveFuelDelivery(currentTime);

        // Планирование следующей доставки
        double nextDelivery = currentTime + config.deliveryInterval;
        if (nextDelivery < config.simulationMinutes) {
            eventQueue.add(new Event(EventType.FUEL_DELIVERY, nextDelivery));
        }
    }

    /**
     * Обработка события открытия колонки.
     */
    private void handlePumpOpen() {
        int day = config.getDay(currentTime);
        boolean added = station.addPump(day);

        if (added) {
            // Если есть автомобили в очереди, начать обслуживание немедленно
            FuelPump newPump = station.getPumps().get(station.getPumpCount() - 1);
            if (!station.isQueueEmpty()) {
                Car car = station.pollFromQueue();
                if (station.hasEnoughFuel(car)) {
                    double endTime = newPump.serve(car, currentTime);
                    eventQueue.add(new Event(EventType.SERVICE_END, endTime, newPump, car));
                } else {
                    stats.recordRefusal(currentTime);
                }
            }
        }
    }

    /**
     * Вывести итоговый отчёт.
     */
    public void printReport() {
        stats.printReport(station, config.simulationMinutes);
    }

    // Геттеры для тестирования
    public GasStation getStation() { return station; }
    public Statistics getStats() { return stats; }
}
