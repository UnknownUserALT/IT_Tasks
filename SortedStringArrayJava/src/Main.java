public class Main {
    public static void main(String[] args) {
        System.out.println("=== Тестирование класса SortedStringArray ===\n");

        // Создаем экземпляр класса
        SortedStringArray sortedArray = new SortedStringArray();

        System.out.println("1. Тест с пустым массивом:");
        System.out.println("   Размер массива: " + sortedArray.size());
        System.out.println("   Массив пуст? " + sortedArray.isEmpty());

        System.out.println("\n2. Добавляем строки в произвольном порядке:");
        sortedArray.add("Привет");
        sortedArray.add("Я");
        sortedArray.add("программирую");
        sortedArray.add("на");
        sortedArray.add("Java");
        sortedArray.add("Это интересно!");

        sortedArray.printAll();

        System.out.println("\n3. Тестируем методы:");
        System.out.println("   Размер массива: " + sortedArray.size());
        System.out.println("   Максимальная строка: \"" + sortedArray.getMaxLengthString() + "\"");
        System.out.printf("   Средняя длина строк: %.2f символов%n", sortedArray.getAverageLength());

        System.out.println("\n4. Добавляем еще строк:");
        sortedArray.add("!");
        sortedArray.add("Дополнительная строка для теста");
        sortedArray.add("test");

        sortedArray.printAll();

        System.out.println("\n5. Проверяем обновленные данные:");
        System.out.println("   Размер массива: " + sortedArray.size());
        System.out.println("   Максимальная строка: \"" + sortedArray.getMaxLengthString() + "\"");
        System.out.printf("   Средняя длина строк: %.2f символов%n", sortedArray.getAverageLength());

        System.out.println("\n6. Тест получения строк по индексу:");
        for (int i = 0; i < sortedArray.size(); i++) {
            System.out.printf("   Индекс %d: \"%s\"%n", i, sortedArray.get(i));
        }

        System.out.println("\n7. Тест обработки ошибок:");

        // Попытка получить максимальную строку из пустого массива
        SortedStringArray emptyArray = new SortedStringArray();
        try {
            emptyArray.getMaxLengthString();
        } catch (IllegalStateException e) {
            System.out.println("   Ошибка при получении max из пустого массива: " + e.getMessage());
        }

        // Попытка получить среднюю длину из пустого массива
        try {
            emptyArray.getAverageLength();
        } catch (IllegalStateException e) {
            System.out.println("   Ошибка при получении средней длины из пустого массива: " + e.getMessage());
        }

        // Попытка добавить null
        try {
            sortedArray.add(null);
        } catch (IllegalArgumentException e) {
            System.out.println("   Ошибка при добавлении null: " + e.getMessage());
        }

        // Попытка получить несуществующий индекс
        try {
            sortedArray.get(100);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("   Ошибка при получении по неверному индексу: " + e.getMessage());
        }

        System.out.println("\n8. Тест очистки массива:");
        sortedArray.clear();
        System.out.println("   Размер после очистки: " + sortedArray.size());
        System.out.println("   Массив пуст? " + sortedArray.isEmpty());

        // Добавляем строки разной длины с пробелами и специальными символами
        System.out.println("\n9. Тест с различными типами строк:");
        sortedArray.add("");
        sortedArray.add("   ");
        sortedArray.add("Очень длинная строка для проверки корректной работы алгоритма сортировки");
        sortedArray.add("средняя");
        sortedArray.add("!");
        sortedArray.add("12345");

        sortedArray.printAll();
        System.out.printf("   Средняя длина: %.2f символов%n", sortedArray.getAverageLength());
    }
}