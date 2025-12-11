public class Triangle extends Figure {
    private double sideA;
    private double sideB;
    private double sideC;

    public Triangle(double sideA, double sideB, double sideC) {
        this.sideA = sideA;
        this.sideB = sideB;
        this.sideC = sideC;
        validateFigure();
    }

    @Override
    protected void validateFigure() {
        // Треугольник существует, если:
        // 1. Все стороны положительны
        // 2. Выполняется неравенство треугольника для каждой пары сторон
        if (sideA <= 0 || sideB <= 0 || sideC <= 0) {
            throwValidationError("Стороны треугольника должны быть положительные.");
        }

        if (!(sideA + sideB > sideC &&
                sideA + sideC > sideB &&
                sideB + sideC > sideA)) {
            throwValidationError("Нарушено неравенство треугольника: сумма любых двух сторон должна быть больше третьей стороны.");
        }
    }

    @Override
    public double calculateArea() {
        // Используем формулу Герона
        double s = calculatePerimeter() / 2;
        return Math.sqrt(s * (s - sideA) * (s - sideB) * (s - sideC));
    }

    @Override
    public double calculatePerimeter() {
        return sideA + sideB + sideC;
    }

}