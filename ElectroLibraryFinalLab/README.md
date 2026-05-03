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

## Как собрать (CLion, Windows)

### Требования
- **CLion** (2022.3 или новее)
- **Qt 6.4+** (модули: Core, Gui, Widgets, Sql, Network, Concurrent)
- **CMake 3.16+** (встроен в CLion)
- **Компилятор с поддержкой C++17** (MinGW или MSVC, идут в комплекте с Qt)

### Пошаговая инструкция

1. **Установить Qt 6.4+**
   Скачать Qt Online Installer с [qt.io](https://www.qt.io/download-qt-installer) и установить нужную версию (например, `6.7.0`). При установке выбрать компилятор (MinGW или MSVC) и отметить модули: Core, Gui, Widgets, Sql, Network, Concurrent.

2. **Открыть проект в CLion**
   `File → Open` → выбрать корневую папку проекта (где лежит `CMakeLists.txt`). CLion автоматически обнаружит CMake-проект.

3. **Настроить тулчейн**
   `File → Settings → Build, Execution, Deployment → Toolchains`. Убедиться, что настроен тулчейн с тем же компилятором, который был выбран при установке Qt (MinGW или Visual Studio).

4. **Указать путь к Qt в CMake**
   `File → Settings → Build, Execution, Deployment → CMake`. В поле **CMake options** добавить:
   ```
   -DCMAKE_PREFIX_PATH=C:/Qt/6.7.0/mingw_64
   ```
   (Заменить путь на актуальный. Для MSVC это будет, например, `C:/Qt/6.7.0/msvc2022_64`.)

5. **Собрать и запустить**
   Нажать **Build → Build Project** (или `Ctrl+F9`). Затем выбрать конфигурацию `library_catalog` в выпадающем списке и нажать **Run** (`Shift+F10`).

6. **Если приложение не запускается** (не находит Qt DLL)
   Добавить путь к Qt DLL в переменную `PATH`. В CLion: `Run → Edit Configurations → Environment variables`:
   ```
   PATH=C:\Qt\6.7.0\mingw_64\bin;%PATH%
   ```
   Либо скопировать нужные DLL (`Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll`, `Qt6Sql.dll`, `Qt6Network.dll`, `Qt6Concurrent.dll`) в папку с исполняемым файлом.

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
