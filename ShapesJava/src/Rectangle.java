public class Rectangle extends Figure {
    private double length;
    private double width;

    public Rectangle(double length, double width) {
        this.length = length;
        this.width = width;
        validateFigure();
    }

    @Override
    protected void validateFigure() {
        // Прямоугольник существует, если обе стороны положительны
        if (length <= 0 || width <= 0) {
            throwValidationError("Стороны прямоугольника должны быть положительные");
        }
    }

    @Override
    public double calculateArea() {
        return length * width;
    }

    @Override
    public double calculatePerimeter() {
        return 2 * (length + width);
    }
}