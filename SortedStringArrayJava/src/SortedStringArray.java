import java.util.ArrayList;
import java.util.List;

public class SortedStringArray {
    private List<String> strings;

    public SortedStringArray() {
        strings = new ArrayList<>();
    }

    /**
     * Добавляет строку в массив, сохраняя порядок по возрастанию длины
     */
    public void add(String str) {
        if (str == null) {
            throw new IllegalArgumentException("Нельзя добавить null строку");
        }

        // Находим позицию для вставки
        int insertIndex = 0;
        while (insertIndex < strings.size() && strings.get(insertIndex).length() < str.length()) {
            insertIndex++;
        }

        // Вставляем строку на найденную позицию
        strings.add(insertIndex, str);
    }

    /**
     * Возвращает максимальную по длине строку
     */
    public String getMaxLengthString() {
        if (strings.isEmpty()) {
            throw new IllegalStateException("Массив пуст");
        }
        return strings.get(strings.size() - 1); // Последний элемент самый длинный
    }

    /**
     * Возвращает среднюю длину всех строк
     */
    public double getAverageLength() {
        if (strings.isEmpty()) {
            throw new IllegalStateException("Массив пуст");
        }

        int totalLength = 0;
        for (String str : strings) {
            totalLength += str.length();
        }

        return (double) totalLength / strings.size();
    }

    /**
     * Возвращает количество строк в массиве
     */
    public int size() {
        return strings.size();
    }

    /**
     * Проверяет, пуст ли массив
     * @return true если массив пуст
     */
    public boolean isEmpty() {
        return strings.isEmpty();
    }

    /**
     * Возвращает строку по индексу
     */
    public String get(int index) {
        if (index < 0 || index >= strings.size()) {
            throw new IndexOutOfBoundsException("Неверный индекс: " + index);
        }
        return strings.get(index);
    }

    /**
     * Выводит все строки с их длинами
     */
    public void printAll() {
        if (strings.isEmpty()) {
            System.out.println("Массив пуст");
            return;
        }

        System.out.println("Строки в порядке возрастания длины:");
        for (int i = 0; i < strings.size(); i++) {
            String str = strings.get(i);
            System.out.printf("%d. \"%s\" (длина: %d)%n", i + 1, str, str.length());
        }
    }

    /**
     * Очищает массив
     */
    public void clear() {
        strings.clear();
    }
}