public class Main {
    public static void main(String[] args) {
        try {

            // Прямоугольник
            Rectangle rectangle = new Rectangle(5, 10);
            System.out.println("Прямоугольник (5x10):");
            System.out.println("  Площадь: " + rectangle.calculateArea());
            System.out.println("  Периметр: " + rectangle.calculatePerimeter());

            // Круг
            Circle circle = new Circle(7);
            System.out.println("\nКруг (радиус=7):");
            System.out.println("  Площадь: " + circle.calculateArea());
            System.out.println("  Периметр: " + circle.calculatePerimeter());

            // Треугольник
            Triangle triangle = new Triangle(3, 4, 5);
            System.out.println("\nТреугольник (3,4,5):");
            System.out.println("  Площадь: " + triangle.calculateArea());
            System.out.println("  Периметр: " + triangle.calculatePerimeter());

        } catch (Exception e) {
            System.out.println("Unexpected error: " + e.getMessage());
        }
    }
}