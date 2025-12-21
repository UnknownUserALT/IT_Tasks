Запуск Java проекта в IntelliJ IDEA
🚀 Быстрый старт
1. Открытие проекта
   Open → Выберите папку с проектом

IDEA автоматически определит структуру

2. Структура проекта
   text
   src/
   ├── Main.java
   ├── Cart.java
   └── Product.java
3. Настройка SDK
   File → Project Structure (Ctrl+Alt+Shift+S)

Project → SDK: Выберите JDK 8+

Project language level: 8+

▶️ Запуск программы
Способ 1: Из редактора
Откройте Main.java

Нажмите зелёную стрелку рядом с классом

Выберите Run 'Main.main()'

Способ 2: Через меню
Run → Run...

Выберите Main

🔧 Сборка (Build)
Build → Build Project (Ctrl+F9)

Автосборка включена по умолчанию

📦 Если файлы не в src/
Правый клик на папку → Mark Directory as → Sources Root

Или настройте в Project Structure → Modules → Sources

⚙️ Проблемы и решения
Ошибка: "Cannot resolve symbol"
File → Invalidate Caches → Invalidate and Restart

Или Build → Rebuild Project

Ошибка: Main class not found
Проверьте наличие public static void main(String[] args)

Убедитесь, что класс Main публичный

