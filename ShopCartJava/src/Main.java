
public class Main {
    public static void main(String[] args) {
        Cart cart = new Cart();

        Product laptop = new Product("Ноутбук", 75000.50);
        Product smartphone = new Product("Смартфон", 45000.00);
        Product headphones = new Product("Наушники", 8500.99);
        Product mouse = new Product("Компьютерная мышь", 2500.50);
        Product keyboard = new Product("Клавиатура", 5500.75);

        System.out.println("=== ДОБАВЛЕНИЕ ТОВАРОВ В КОРЗИНУ ===");
        cart.addProduct(laptop);
        cart.addProduct(smartphone);
        cart.addProduct(headphones);
        cart.addProduct(mouse);
        cart.addProduct(keyboard);

        cart.printCart();

        System.out.println("\n=== УДАЛЕНИЕ ТОВАРА ===");
        cart.removeProduct("Смартфон");

        System.out.println("\n=== КОРЗИНА ПОСЛЕ УДАЛЕНИЯ ===");
        cart.printCart();

        System.out.println("\n=== ДОПОЛНИТЕЛЬНЫЕ ДЕЙСТВИЯ ===");
        cart.removeProduct("Планшет");

        Product charger = new Product("Зарядное устройство", 3500.00);
        cart.addProduct(charger);

        cart.printCart();
    }
}