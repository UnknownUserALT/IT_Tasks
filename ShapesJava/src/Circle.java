public class Circle extends Figure {
    private double radius;

    public Circle(double radius) {
        this.radius = radius;
        validateFigure();
    }

    @Override
    protected void validateFigure() {
        // Круг существует, если радиус положительный
        if (radius <= 0) {
            throwValidationError("Радиус круга должен быть положительный");
        }
    }

    @Override
    public double calculateArea() {
        return Math.PI * radius * radius;
    }

    @Override
    public double calculatePerimeter() {
        return 2 * Math.PI * radius;
    }
}