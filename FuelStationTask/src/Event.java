/**
 * Представляет событие в дискретно-событийной симуляции.
 */
public class Event implements Comparable<Event> {
    private final EventType type;
    private final double time;
    private final FuelPump pump;    // Связанная колонка (для SERVICE_END, PUMP_OPEN)
    private final Car car;          // Связанный автомобиль (для SERVICE_END)

    public Event(EventType type, double time) {
        this(type, time, null, null);
    }

    public Event(EventType type, double time, FuelPump pump, Car car) {
        this.type = type;
        this.time = time;
        this.pump = pump;
        this.car = car;
    }

    public EventType getType() { return type; }
    public double getTime() { return time; }
    public FuelPump getPump() { return pump; }
    public Car getCar() { return car; }

    @Override
    public int compareTo(Event other) {
        return Double.compare(this.time, other.time);
    }

    @Override
    public String toString() {
        return String.format("Event[%s at %.2f min]", type, time);
    }
}
