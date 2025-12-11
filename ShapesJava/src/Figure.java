public abstract class Figure {
    public abstract double calculateArea();
    public abstract double calculatePerimeter();
    protected abstract void validateFigure();

    protected void throwValidationError(String message) {
        throw new RuntimeException("There is no figure with such parameters. " + message);
    }
}