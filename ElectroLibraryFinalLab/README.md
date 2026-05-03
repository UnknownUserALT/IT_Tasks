# Каталог электронной библиотеки

Настольное приложение на Qt 6 (C++) для ведения коллекции книг с поддержкой поиска по ISBN через OpenLibrary API.

## Возможности

- Добавление, редактирование и удаление книг
- Поиск и фильтрация по названию, автору и статусу прочтения
- Импорт/экспорт коллекции в JSON
- Автоматическое получение данных о книге по ISBN из OpenLibrary
- Статистика чтения с интерактивной столбчатой диаграммой (QPainter)
- Сохранение настроек окна между сессиями

## Творческая часть

### A. Светлая/тёмная тема
Переключение темы без перезапуска приложения через меню **Вид → Тема**. Выбранная тема сохраняется в QSettings и восстанавливается при следующем запуске. Виджет статистики использует цвета из текущей палитры.

### D. Обложки книг из OpenLibrary
При поиске книги по ISBN автоматически загружается обложка с covers.openlibrary.org. Обложки кэшируются локально в `QStandardPaths::CacheLocation/covers/`. Обложка отображается на карточке результата поиска и в панели деталей книги.

## Как собрать

### Требования
- Qt 6.4+ (модули: Core, Gui, Widgets, Sql, Network, Concurrent)
- CMake 3.16+
- Компилятор с поддержкой C++17 (g++, clang, MSVC)

### Сборка (Linux)
```bash
cmake -B build
cmake --build build
./build/library_catalog
```

### Сборка (Windows, MSVC)
```cmd
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
build\Release\library_catalog.exe
```

## Структура проекта

```
├── CMakeLists.txt
├── README.md
├── resources/
│   ├── icons.qrc
│   └── icons/*.png
└── src/
    ├── main.cpp
    ├── book.h
    ├── mainwindow.h / .cpp / .ui
    ├── bookdialog.h / .cpp / .ui
    ├── databasemanager.h / .cpp
    ├── openlibraryclient.h / .cpp
    └── statisticswidget.h / .cpp
```

## Архитектура

- **DatabaseManager** — единственная точка доступа к SQLite. Prepared statements, без конкатенации строк в SQL.
- **BookFilterProxyModel** (QSortFilterProxyModel) — фильтрация по статусу и поиск по title/author.
- **OpenLibraryClient** — работает в отдельном потоке (moveToThread). Связь с UI через сигналы/слоты.
- **StatisticsWidget** — отрисовка диаграммы через QPainter, подсветка при наведении.
