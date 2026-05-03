// MainWindow — главное окно приложения.
// Содержит QTabWidget с тремя вкладками:
//   1. «Библиотека» — таблица книг (QTableView + QSqlTableModel + прокси), панель поиска, детали книги
//   2. «Статистика» — виджет StatisticsWidget (QPainter) и числовые карточки
//   3. «Импорт из OpenLibrary» — поиск по ISBN, карточка результата, кнопка добавления
// Снаружи — меню (Файл, Правка, Вид, Справка) и тулбар с основными действиями.

#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QSqlTableModel;
class QThread;
QT_END_NAMESPACE

class DatabaseManager;
class OpenLibraryClient;
class StatisticsWidget;
class BookFilterProxyModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(DatabaseManager *db, QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    /// Сохраняем настройки (геометрию окна, фильтры) при закрытии
    void closeEvent(QCloseEvent *event) override;

private slots:
    // ── CRUD-слоты (привязаны к QAction через connect) ──
    void onAddBook();
    void onEditBook();
    void onRemoveBook();

    // ── Поиск и фильтрация ──
    void onSearchTextChanged(const QString &text);
    void onStatusFilterChanged(int index);

    // ── Импорт / Экспорт JSON ──
    void onImportJson();
    void onExportJson();

    // ── Сетевые слоты (связь с OpenLibraryClient через сигналы) ──
    void onFetchByIsbn();
    void onFetchFinished(const QVariantMap &data);
    void onFetchFailed(const QString &message);
    void onCoverReady(const QString &isbn, const QString &cachePath);

    // ── Вспомогательные ──
    void onBookSelectionChanged();   // Обновление панели деталей при выборе книги
    void onAddFetchedBook();         // Добавление найденной через API книги в БД
    void refreshStatistics();        // Обновление вкладки статистики
    void updateStatusBarCount();     // Обновление «N книг в коллекции» в статус-баре

private:
    void setupMenuAndToolbar();   // Создание QAction, меню и тулбара
    void setupTableModel();       // Инициализация QSqlTableModel + прокси + делегат
    void setupStatisticsTab();    // Создание StatisticsWidget
    void setupNetworkThread();    // Создание потока и OpenLibraryClient (moveToThread)
    void setupConnections();      // Подключение сигналов виджетов к слотам
    void loadSettings();          // Восстановление настроек из QSettings
    void saveSettings();          // Сохранение настроек в QSettings

    Ui::MainWindow       *ui;
    DatabaseManager      *m_db;             // Менеджер БД (создан в main.cpp, живёт там же)
    QSqlTableModel       *m_model;          // Модель таблицы books
    BookFilterProxyModel *m_proxy;          // Прокси для поиска и фильтрации
    StatisticsWidget     *m_stats;          // Виджет диаграммы (QPainter)
    QThread              *m_networkThread;  // Поток для сетевых запросов
    OpenLibraryClient    *m_client;         // Сетевой клиент (живёт в m_networkThread)

    QVariantMap m_lastFetchedData;          // Последние полученные данные из OpenLibrary
    QString     m_lastImportExportPath;     // Путь последнего импорта/экспорта (QSettings)
    bool        m_fetchInProgress = false;  // Флаг активного сетевого запроса
};
