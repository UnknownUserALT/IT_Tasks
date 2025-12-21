import java.util.ArrayList;

public class Cart {
    private ArrayList<Product> products;

    public Cart() {
        products = new ArrayList<>();
    }

    // Добавить товар в корзину
    public void addProduct(Product product) {
        products.add(product);
        System.out.println("Товар '" + product.getName() + "' добавлен в корзину.");
    }

    // Удалить товар по названию
    public void removeProduct(String name) {
        boolean removed = products.removeIf(product -> product.getName().equalsIgnoreCase(name));
        if (removed) {
            System.out.println("Товар '" + name + "' удален из корзины.");
        } else {
            System.out.println("Товар '" + name + "' не найден в корзине.");
        }
    }

    // Посчитать общую сумму
    public double getTotal() {
        double total = 0;
        for (Product product : products) {
            total += product.getPrice();
        }
        return total;
    }

    // Вывести содержимое корзины
    public void printCart() {
        if (products.isEmpty()) {
            System.out.println("Корзина пуста.");
            return;
        }

        System.out.println("\n=== СОДЕРЖИМОЕ КОРЗИНЫ ===");
        for (int i = 0; i < products.size(); i++) {
            System.out.println((i + 1) + ". " + products.get(i));
        }
        System.out.println("==========================");
        System.out.printf("ИТОГО: %.2f руб.\n", getTotal());
    }
}