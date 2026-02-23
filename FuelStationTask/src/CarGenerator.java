/**
 * Генерирует прибытие автомобилей по неоднородному пуассоновскому процессу.
 */
public class CarGenerator {
    private final Config config;

    public CarGenerator(Config config) {
        this.config = config;
    }

    /**
     * Сгенерировать интервал до прибытия следующего автомобиля.
     * Использует экспоненциальное распределение: Δt = -ln(U) / λ(t)
     * где U ~ Uniform(0, 1)
     *
     * @param currentTime текущее время симуляции в минутах
     * @return интервал времени до следующего прибытия
     */
    public double generateInterval(double currentTime) {
        double lambda = config.getArrivalRate(currentTime);
        double u = config.random.nextDouble();
        // Избегаем log(0), убеждаясь что u > 0
        while (u == 0) {
            u = config.random.nextDouble();
        }
        return -Math.log(u) / lambda;
    }

    /**
     * Сгенерировать время прибытия первого автомобиля.
     */
    public double generateFirstArrival() {
        return generateInterval(0);
    }

    /**
     * Сгенерировать новый автомобиль со случайными параметрами.
     */
    public Car generateCar(double arrivalTime) {
        return Car.generate(config, arrivalTime);
    }
}
