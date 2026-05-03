// Реализация MainWindow — главного окна приложения.
// Здесь собрана логика: CRUD книг, поиск/фильтрация, импорт/экспорт JSON,
// сетевые запросы к OpenLibrary, статистика, настройки, тема оформления.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include "openlibraryclient.h"
#include "statisticswidget.h"
#include "bookdialog.h"
#include "bookfilterproxymodel.h"
#include "statusdelegate.h"
#include "book.h"

#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QThread>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QCloseEvent>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QStyleFactory>

// ─── Конструктор / Деструктор ───

MainWindow::MainWindow(DatabaseManager *db, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_db(db)
    , m_model(nullptr)
    , m_proxy(nullptr)
    , m_stats(nullptr)
    , m_networkThread(nullptr)
    , m_client(nullptr)
{
    ui->setupUi(this);

    // Инициализация всех подсистем
    setupMenuAndToolbar();
    setupTableModel();
    setupStatisticsTab();
    setupNetworkThread();
    setupConnections();
    loadSettings();

    // Начальное обновление статус-бара и статистики
    updateStatusBarCount();
    refreshStatistics();
}

MainWindow::~MainWindow()
{
    // Корректное завершение сетевого потока перед уничтожением окна
    if (m_networkThread) {
        m_networkThread->quit();
        m_networkThread->wait();  // Ждём завершения event loop потока
    }
    delete ui;
}

// ─── Настройка меню, тулбара и горячих клавиш ───

void MainWindow::setupMenuAndToolbar()
{
    // Создание QAction с иконками из ресурсов (.qrc)
    auto *actAdd    = new QAction(QIcon(":/icons/icons/add.png"),    tr("Добавить книгу"), this);
    auto *actEdit   = new QAction(QIcon(":/icons/icons/edit.png"),   tr("Редактировать"), this);
    auto *actDelete = new QAction(QIcon(":/icons/icons/delete.png"), tr("Удалить"), this);
    auto *actRefresh= new QAction(QIcon(":/icons/icons/refresh.png"),tr("Обновить статистику"), this);
    auto *actImport = new QAction(QIcon(":/icons/icons/import.png"), tr("Импорт из JSON"), this);
    auto *actExport = new QAction(QIcon(":/icons/icons/export.png"), tr("Экспорт в JSON"), this);
    auto *actExit   = new QAction(tr("Выход"), this);
    auto *actAbout  = new QAction(tr("О программе"), this);

    // Горячие клавиши (по ТЗ: Ctrl+N, Ctrl+R, Delete, F5, Ctrl+I, Ctrl+E, Ctrl+Q)
    actAdd->setShortcut(QKeySequence(tr("Ctrl+N")));
    actEdit->setShortcut(QKeySequence(tr("Ctrl+R")));
    actDelete->setShortcut(QKeySequence::Delete);
    actRefresh->setShortcut(QKeySequence(tr("F5")));
    actImport->setShortcut(QKeySequence(tr("Ctrl+I")));
    actExport->setShortcut(QKeySequence(tr("Ctrl+E")));
    actExit->setShortcut(QKeySequence(tr("Ctrl+Q")));

    // ── Меню «Файл» ──
    auto *menuFile = menuBar()->addMenu(tr("Файл"));
    menuFile->addAction(actImport);
    menuFile->addAction(actExport);
    menuFile->addSeparator();
    menuFile->addAction(actExit);

    // ── Меню «Правка» ──
    auto *menuEdit = menuBar()->addMenu(tr("Правка"));
    menuEdit->addAction(actAdd);
    menuEdit->addAction(actEdit);
    menuEdit->addAction(actDelete);

    // ── Меню «Вид» ──
    auto *menuView = menuBar()->addMenu(tr("Вид"));
    menuView->addAction(actRefresh);

    // Подменю переключения темы (творческая часть А)
    auto *menuTheme = menuView->addMenu(tr("Тема"));
    auto *actLight = new QAction(tr("Светлая"), this);
    auto *actDark  = new QAction(tr("Тёмная"), this);
    menuTheme->addAction(actLight);
    menuTheme->addAction(actDark);

    // Светлая тема — сброс к стандартной палитре
    connect(actLight, &QAction::triggered, this, [this]() {
        qApp->setStyleSheet("");
        qApp->setPalette(QApplication::style()->standardPalette());
        QSettings s;
        s.setValue("theme", "light");
        if (m_stats) m_stats->update();  // Перерисовать диаграмму с новыми цветами
    });

    // Тёмная тема — собственная палитра
    connect(actDark, &QAction::triggered, this, [this]() {
        QPalette dark;
        dark.setColor(QPalette::Window, QColor(53,53,53));
        dark.setColor(QPalette::WindowText, Qt::white);
        dark.setColor(QPalette::Base, QColor(42,42,42));
        dark.setColor(QPalette::AlternateBase, QColor(66,66,66));
        dark.setColor(QPalette::ToolTipBase, Qt::white);
        dark.setColor(QPalette::ToolTipText, Qt::white);
        dark.setColor(QPalette::Text, Qt::white);
        dark.setColor(QPalette::Button, QColor(53,53,53));
        dark.setColor(QPalette::ButtonText, Qt::white);
        dark.setColor(QPalette::BrightText, Qt::red);
        dark.setColor(QPalette::Link, QColor(42,130,218));
        dark.setColor(QPalette::Highlight, QColor(42,130,218));
        dark.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(dark);
        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
        QSettings s;
        s.setValue("theme", "dark");
        if (m_stats) m_stats->update();
    });

    // ── Меню «Справка» ──
    auto *menuHelp = menuBar()->addMenu(tr("Справка"));
    menuHelp->addAction(actAbout);

    // ── Тулбар (минимум 4 действия с иконками) ──
    ui->toolBar->addAction(actAdd);
    ui->toolBar->addAction(actEdit);
    ui->toolBar->addAction(actDelete);
    ui->toolBar->addAction(actRefresh);

    // ── Подключение QAction → слоты (новый синтаксис connect) ──
    connect(actAdd,     &QAction::triggered, this, &MainWindow::onAddBook);
    connect(actEdit,    &QAction::triggered, this, &MainWindow::onEditBook);
    connect(actDelete,  &QAction::triggered, this, &MainWindow::onRemoveBook);
    connect(actRefresh, &QAction::triggered, this, &MainWindow::refreshStatistics);
    connect(actImport,  &QAction::triggered, this, &MainWindow::onImportJson);
    connect(actExport,  &QAction::triggered, this, &MainWindow::onExportJson);
    connect(actExit,    &QAction::triggered, this, &QMainWindow::close);
    connect(actAbout,   &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("О программе"),
            tr("Каталог электронной библиотеки\n\nBMSTU, 2026\nQt %1").arg(qVersion()));
    });
}

// ─── Инициализация модели таблицы ───

void MainWindow::setupTableModel()
{
    // QSqlTableModel — модель, работающая напрямую с таблицей SQLite
    // OnFieldChange — изменения сохраняются в БД сразу при редактировании ячейки
    m_model = new QSqlTableModel(this, m_db->database());
    m_model->setTable("books");
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_model->select();

    // Русские заголовки столбцов
    m_model->setHeaderData(0,  Qt::Horizontal, tr("ID"));
    m_model->setHeaderData(1,  Qt::Horizontal, tr("Название"));
    m_model->setHeaderData(2,  Qt::Horizontal, tr("Автор"));
    m_model->setHeaderData(3,  Qt::Horizontal, tr("Год"));
    m_model->setHeaderData(4,  Qt::Horizontal, tr("ISBN"));
    m_model->setHeaderData(5,  Qt::Horizontal, tr("Жанр"));
    m_model->setHeaderData(6,  Qt::Horizontal, tr("Статус"));
    m_model->setHeaderData(7,  Qt::Horizontal, tr("Рейтинг"));
    m_model->setHeaderData(8,  Qt::Horizontal, tr("Страниц"));
    m_model->setHeaderData(9,  Qt::Horizontal, tr("Заметки"));
    m_model->setHeaderData(10, Qt::Horizontal, tr("Добавлено"));

    // Прокси-модель для поиска и фильтрации (между моделью и представлением)
    m_proxy = new BookFilterProxyModel(this);
    m_proxy->setSourceModel(m_model);

    // Устанавливаем прокси как модель для таблицы (view->setModel(proxy))
    ui->tableBooks->setModel(m_proxy);

    // Делегат для столбца «Статус» — QComboBox вместо текстового поля
    ui->tableBooks->setItemDelegateForColumn(6, new StatusDelegate(this));

    // Скрываем служебные столбцы (id и added_at)
    ui->tableBooks->setColumnHidden(0, true);
    ui->tableBooks->setColumnHidden(10, true);
    ui->tableBooks->horizontalHeader()->setStretchLastSection(true);

    // Заполняем комбобокс фильтра по статусу
    ui->comboStatusFilter->addItem(tr("Все"),           "");
    ui->comboStatusFilter->addItem(tr("К прочтению"),   "to_read");
    ui->comboStatusFilter->addItem(tr("Читаю"),         "reading");
    ui->comboStatusFilter->addItem(tr("Прочитано"),     "done");
}

// ─── Инициализация вкладки статистики ───

void MainWindow::setupStatisticsTab()
{
    // StatisticsWidget добавляется программно в layout, определённый в .ui
    m_stats = new StatisticsWidget(this);
    ui->statsMainLayout->addWidget(m_stats);
}

// ─── Инициализация сетевого потока ───

void MainWindow::setupNetworkThread()
{
    // Создаём поток и рабочий объект (клиент)
    m_networkThread = new QThread(this);
    m_client = new OpenLibraryClient();  // Без parent! Объект будет перемещён в другой поток.

    // moveToThread — перемещает event loop обработки клиента в отдельный поток.
    // После этого все слоты клиента выполняются в m_networkThread.
    m_client->moveToThread(m_networkThread);

    // При завершении потока — автоматически удаляем клиент
    connect(m_networkThread, &QThread::finished,
            m_client, &QObject::deleteLater);

    // Связь клиент → окно через сигналы (Qt::QueuedConnection автоматически,
    // т.к. отправитель и получатель в разных потоках)
    connect(m_client, &OpenLibraryClient::finished,
            this, &MainWindow::onFetchFinished);
    connect(m_client, &OpenLibraryClient::failed,
            this, &MainWindow::onFetchFailed);
    connect(m_client, &OpenLibraryClient::coverReady,
            this, &MainWindow::onCoverReady);

    m_networkThread->start();
}

// ─── Подключение сигналов виджетов ───

void MainWindow::setupConnections()
{
    // Поиск: при вводе текста — фильтрация через прокси-модель
    connect(ui->editSearch, &QLineEdit::textChanged,
            this, &MainWindow::onSearchTextChanged);

    // Фильтр по статусу: QComboBox → слот
    connect(ui->comboStatusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStatusFilterChanged);

    // Выбор строки в таблице → обновление панели деталей справа
    connect(ui->tableBooks->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onBookSelectionChanged);

    // Кнопки на вкладке «Импорт из OpenLibrary»
    connect(ui->btnFetch, &QPushButton::clicked,
            this, &MainWindow::onFetchByIsbn);
    connect(ui->btnAddToLibrary, &QPushButton::clicked,
            this, &MainWindow::onAddFetchedBook);
}

// ─── Настройки (QSettings) ───

void MainWindow::loadSettings()
{
    QSettings s;

    // Восстановление геометрии окна (размер + позиция)
    if (s.contains("geometry"))
        restoreGeometry(s.value("geometry").toByteArray());

    // Восстановление последнего выбранного фильтра по статусу
    if (s.contains("statusFilter")) {
        int idx = s.value("statusFilter").toInt();
        if (idx >= 0 && idx < ui->comboStatusFilter->count())
            ui->comboStatusFilter->setCurrentIndex(idx);
    }

    // Путь последнего файла импорта/экспорта
    m_lastImportExportPath = s.value("lastImportExportPath").toString();

    // Восстановление темы (творческая часть А)
    QString theme = s.value("theme", "light").toString();
    if (theme == "dark") {
        QPalette dark;
        dark.setColor(QPalette::Window, QColor(53,53,53));
        dark.setColor(QPalette::WindowText, Qt::white);
        dark.setColor(QPalette::Base, QColor(42,42,42));
        dark.setColor(QPalette::AlternateBase, QColor(66,66,66));
        dark.setColor(QPalette::ToolTipBase, Qt::white);
        dark.setColor(QPalette::ToolTipText, Qt::white);
        dark.setColor(QPalette::Text, Qt::white);
        dark.setColor(QPalette::Button, QColor(53,53,53));
        dark.setColor(QPalette::ButtonText, Qt::white);
        dark.setColor(QPalette::BrightText, Qt::red);
        dark.setColor(QPalette::Link, QColor(42,130,218));
        dark.setColor(QPalette::Highlight, QColor(42,130,218));
        dark.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(dark);
        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }
}

void MainWindow::saveSettings()
{
    QSettings s;
    s.setValue("geometry", saveGeometry());
    s.setValue("statusFilter", ui->comboStatusFilter->currentIndex());
    s.setValue("lastImportExportPath", m_lastImportExportPath);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}

// ─── CRUD-слоты: добавление, редактирование, удаление книг ───

void MainWindow::onAddBook()
{
    // Открываем диалог в режиме создания (пустые поля)
    BookDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Book b = dlg.getBook();
        int id = m_db->addBook(b);
        if (id < 0) {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось добавить книгу:\n%1").arg(m_db->lastError()));
            return;
        }
        m_model->select();      // Обновить данные в таблице
        updateStatusBarCount();
        refreshStatistics();
    }
}

void MainWindow::onEditBook()
{
    QModelIndex idx = ui->tableBooks->currentIndex();
    if (!idx.isValid()) {
        QMessageBox::information(this, tr("Редактирование"),
                                 tr("Выберите книгу для редактирования."));
        return;
    }

    // Преобразуем индекс прокси в индекс исходной модели
    QModelIndex sourceIdx = m_proxy->mapToSource(idx);
    int id = m_model->data(m_model->index(sourceIdx.row(), 0)).toInt();

    auto bookOpt = m_db->getBookById(id);
    if (!bookOpt) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Книга не найдена в базе данных."));
        return;
    }

    // Открываем диалог в режиме редактирования (поля заполнены)
    BookDialog dlg(this);
    dlg.setBook(*bookOpt);
    if (dlg.exec() == QDialog::Accepted) {
        Book b = dlg.getBook();
        b.id = id;  // Сохраняем id для UPDATE
        if (!m_db->updateBook(b)) {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось обновить книгу:\n%1").arg(m_db->lastError()));
            return;
        }
        m_model->select();
        updateStatusBarCount();
        refreshStatistics();
    }
}

void MainWindow::onRemoveBook()
{
    QModelIndex idx = ui->tableBooks->currentIndex();
    if (!idx.isValid()) {
        QMessageBox::information(this, tr("Удаление"),
                                 tr("Выберите книгу для удаления."));
        return;
    }

    QModelIndex sourceIdx = m_proxy->mapToSource(idx);
    int id = m_model->data(m_model->index(sourceIdx.row(), 0)).toInt();
    QString title = m_model->data(m_model->index(sourceIdx.row(), 1)).toString();

    // Подтверждение удаления
    auto answer = QMessageBox::question(this, tr("Подтверждение удаления"),
        tr("Удалить книгу «%1»?").arg(title),
        QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes) {
        if (!m_db->removeBook(id)) {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Не удалось удалить книгу:\n%1").arg(m_db->lastError()));
            return;
        }
        m_model->select();
        updateStatusBarCount();
        refreshStatistics();
    }
}

// ─── Поиск и фильтрация ───

void MainWindow::onSearchTextChanged(const QString &text)
{
    // Передаём текст в прокси-модель — она пересчитает видимые строки
    m_proxy->setSearchText(text);
}

void MainWindow::onStatusFilterChanged(int index)
{
    // Берём внутреннее значение (data) из комбобокса: "", "to_read", "reading", "done"
    QString status = ui->comboStatusFilter->itemData(index).toString();
    m_proxy->setStatusFilter(status);
}

// ─── Панель деталей книги (правая часть вкладки «Библиотека») ───

void MainWindow::onBookSelectionChanged()
{
    QModelIndex idx = ui->tableBooks->currentIndex();
    if (!idx.isValid()) {
        // Ничего не выбрано — очищаем панель
        ui->lblDetailTitle->setText(tr("Выберите книгу"));
        ui->lblDetailAuthor->clear();
        ui->lblDetailInfo->clear();
        ui->lblDetailNotes->clear();
        ui->lblDetailCover->clear();
        return;
    }

    // Получаем данные из исходной модели (через прокси)
    QModelIndex sourceIdx = m_proxy->mapToSource(idx);
    int row = sourceIdx.row();

    QString title  = m_model->data(m_model->index(row, 1)).toString();
    QString author = m_model->data(m_model->index(row, 2)).toString();
    int year       = m_model->data(m_model->index(row, 3)).toInt();
    QString isbn   = m_model->data(m_model->index(row, 4)).toString();
    QString genre  = m_model->data(m_model->index(row, 5)).toString();
    QString status = m_model->data(m_model->index(row, 6)).toString();
    int rating     = m_model->data(m_model->index(row, 7)).toInt();
    int pages      = m_model->data(m_model->index(row, 8)).toInt();
    QString notes  = m_model->data(m_model->index(row, 9)).toString();

    // Перевод статуса на русский для отображения
    QString statusRu;
    if (status == "to_read") statusRu = tr("К прочтению");
    else if (status == "reading") statusRu = tr("Читаю");
    else if (status == "done") statusRu = tr("Прочитано");

    ui->lblDetailTitle->setText(title);
    ui->lblDetailAuthor->setText(tr("Автор: %1").arg(author));

    QString info;
    if (year > 0) info += tr("Год: %1\n").arg(year);
    if (!isbn.isEmpty()) info += tr("ISBN: %1\n").arg(isbn);
    if (!genre.isEmpty()) info += tr("Жанр: %1\n").arg(genre);
    info += tr("Статус: %1\n").arg(statusRu);
    info += tr("Рейтинг: %1/5\n").arg(rating);
    if (pages > 0) info += tr("Страниц: %1").arg(pages);
    ui->lblDetailInfo->setText(info);

    ui->lblDetailNotes->setText(notes.isEmpty() ? "" : tr("Заметки: %1").arg(notes));

    // Отображение обложки из кэша (творческая часть D)
    if (!isbn.isEmpty()) {
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                            + "/covers/" + isbn + ".jpg";
        if (QFileInfo(cachePath).size() > 0) {
            QPixmap pix(cachePath);
            if (!pix.isNull()) {
                ui->lblDetailCover->setPixmap(pix.scaled(120, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        } else {
            ui->lblDetailCover->clear();
        }
    } else {
        ui->lblDetailCover->clear();
    }
}

// ─── Сетевые слоты: поиск по ISBN через OpenLibrary ───

void MainWindow::onFetchByIsbn()
{
    // Повторное нажатие во время запроса — отмена
    if (m_fetchInProgress) {
        QMetaObject::invokeMethod(m_client, "cancelRequest", Qt::QueuedConnection);
        m_fetchInProgress = false;
        ui->btnFetch->setText(tr("Найти"));
        ui->btnAddToLibrary->setEnabled(false);
        statusBar()->showMessage(tr("Запрос отменён"), 3000);
        return;
    }

    QString isbn = ui->editIsbn->text().trimmed();
    if (isbn.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Введите ISBN для поиска."));
        return;
    }

    // Переводим UI в состояние «загрузка»
    m_fetchInProgress = true;
    ui->btnFetch->setText(tr("Отмена"));
    ui->btnAddToLibrary->setEnabled(false);
    statusBar()->showMessage(tr("Загрузка…"));

    // Очищаем предыдущие результаты
    ui->lblResultTitle->setText("—");
    ui->lblResultAuthor->setText("—");
    ui->lblResultDate->setText("—");
    ui->lblResultPages->setText("—");
    ui->lblResultGenre->setText("—");
    ui->lblResultCover->clear();

    // Вызываем слот клиента через QueuedConnection (клиент в другом потоке)
    QMetaObject::invokeMethod(m_client, "fetchByIsbn",
                              Qt::QueuedConnection, Q_ARG(QString, isbn));
}

void MainWindow::onFetchFinished(const QVariantMap &data)
{
    // Сбрасываем состояние UI
    m_fetchInProgress = false;
    ui->btnFetch->setText(tr("Найти"));
    statusBar()->showMessage(tr("Данные получены"), 3000);

    // Сохраняем данные для последующего добавления в библиотеку
    m_lastFetchedData = data;

    // Заполняем карточку результата
    ui->lblResultTitle->setText(data.value("title").toString());
    ui->lblResultAuthor->setText(data.value("author").toString());
    ui->lblResultDate->setText(data.value("publish_date").toString());
    ui->lblResultPages->setText(data.value("pages").toInt() > 0
        ? QString::number(data.value("pages").toInt()) : "—");
    ui->lblResultGenre->setText(data.value("genre", "—").toString());

    ui->btnAddToLibrary->setEnabled(true);

    // Загрузка обложки (творческая часть D) — через рабочий поток
    QString isbn = data.value("isbn").toString();
    if (!isbn.isEmpty()) {
        QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/covers";
        QDir().mkpath(cacheDir);
        QString cachePath = cacheDir + "/" + isbn + ".jpg";

        if (QFileInfo(cachePath).size() > 0) {
            // Обложка уже в кэше — показываем сразу
            QPixmap pix(cachePath);
            if (!pix.isNull())
                ui->lblResultCover->setPixmap(pix.scaled(120, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            // Удаляем пустой/битый файл и запрашиваем обложку через рабочий поток
            QFile::remove(cachePath);
            QMetaObject::invokeMethod(m_client, "fetchCover", Qt::QueuedConnection,
                                      Q_ARG(QString, isbn), Q_ARG(QString, cachePath));
        }
    }
}

void MainWindow::onFetchFailed(const QString &message)
{
    m_fetchInProgress = false;
    ui->btnFetch->setText(tr("Найти"));
    statusBar()->clearMessage();
    QMessageBox::warning(this, tr("Ошибка загрузки"), message);
}

void MainWindow::onCoverReady(const QString &isbn, const QString &cachePath)
{
    // Обложка скачана — обновляем карточку результата, если ISBN совпадает
    if (m_lastFetchedData.value("isbn").toString() == isbn) {
        QPixmap pix(cachePath);
        if (!pix.isNull())
            ui->lblResultCover->setPixmap(pix.scaled(120, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

// ─── Добавление найденной книги в библиотеку ───

void MainWindow::onAddFetchedBook()
{
    if (m_lastFetchedData.isEmpty()) return;

    // Проверка дубликата по ISBN
    QString isbn = m_lastFetchedData.value("isbn").toString();
    if (!isbn.isEmpty() && m_db->existsByIsbn(isbn)) {
        QMessageBox::information(this, tr("Дубликат"),
            tr("Книга с ISBN %1 уже есть в библиотеке.").arg(isbn));
        return;
    }

    // Собираем Book из данных OpenLibrary
    Book b;
    b.title  = m_lastFetchedData.value("title").toString();
    b.author = m_lastFetchedData.value("author").toString();
    b.isbn   = isbn;
    b.pages  = m_lastFetchedData.value("pages").toInt();
    b.genre  = m_lastFetchedData.value("genre").toString();

    // Извлекаем год из строки publish_date (может быть "January 1, 2020" или "2020")
    QString dateStr = m_lastFetchedData.value("publish_date").toString();
    QRegularExpression yearRx("(\\d{4})");
    auto match = yearRx.match(dateStr);
    if (match.hasMatch())
        b.year = match.captured(1).toInt();

    int id = m_db->addBook(b);
    if (id < 0) {
        QMessageBox::warning(this, tr("Ошибка"),
            tr("Не удалось добавить книгу:\n%1").arg(m_db->lastError()));
        return;
    }

    m_model->select();
    updateStatusBarCount();
    refreshStatistics();

    ui->btnAddToLibrary->setEnabled(false);
    statusBar()->showMessage(tr("Книга «%1» добавлена в библиотеку").arg(b.title), 3000);

    // Переключаемся на вкладку «Библиотека»
    ui->tabWidget->setCurrentIndex(0);
}

// ─── Импорт / Экспорт JSON ───

void MainWindow::onExportJson()
{
    // Диалог выбора файла для сохранения
    QString path = QFileDialog::getSaveFileName(this, tr("Экспорт в JSON"),
        m_lastImportExportPath.isEmpty() ? QDir::homePath() : m_lastImportExportPath,
        tr("JSON файлы (*.json)"));
    if (path.isEmpty()) return;
    m_lastImportExportPath = path;

    // Получаем все книги и формируем JSON-массив (без id — внутреннее поле)
    QList<Book> books = m_db->allBooks();
    QJsonArray arr;
    for (const Book &b : books) {
        QJsonObject obj;
        obj["title"]  = b.title;
        obj["author"] = b.author;
        obj["year"]   = b.year;
        obj["isbn"]   = b.isbn;
        obj["genre"]  = b.genre;
        obj["status"] = b.status;
        obj["rating"] = b.rating;
        obj["pages"]  = b.pages;
        obj["notes"]  = b.notes;
        arr.append(obj);
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Ошибка"),
            tr("Не удалось открыть файл для записи:\n%1").arg(file.errorString()));
        return;
    }

    QJsonDocument doc(arr);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    statusBar()->showMessage(tr("Экспортировано %1 книг").arg(books.size()), 3000);
}

void MainWindow::onImportJson()
{
    // Диалог выбора файла для чтения
    QString path = QFileDialog::getOpenFileName(this, tr("Импорт из JSON"),
        m_lastImportExportPath.isEmpty() ? QDir::homePath() : m_lastImportExportPath,
        tr("JSON файлы (*.json)"));
    if (path.isEmpty()) return;
    m_lastImportExportPath = path;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Ошибка"),
            tr("Не удалось открыть файл:\n%1").arg(file.errorString()));
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Парсинг JSON с проверкой ошибок
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(this, tr("Ошибка"),
            tr("Невалидный JSON:\n%1").arg(parseError.errorString()));
        return;
    }

    if (!doc.isArray()) {
        QMessageBox::warning(this, tr("Ошибка"),
            tr("JSON файл должен содержать массив объектов."));
        return;
    }

    QJsonArray arr = doc.array();
    int imported = 0, skipped = 0, errors = 0;

    // Весь импорт в одной транзакции (для скорости и атомарности)
    QSqlDatabase db = m_db->database();
    if (!db.transaction()) {
        QMessageBox::warning(this, tr("Ошибка"),
            tr("Не удалось начать транзакцию."));
        return;
    }

    for (const QJsonValue &val : arr) {
        if (!val.isObject()) { errors++; continue; }
        QJsonObject obj = val.toObject();

        // Обязательные поля: title и author
        if (!obj.contains("title") || !obj.contains("author")) {
            errors++;
            continue;
        }

        Book b;
        b.title  = obj.value("title").toString();
        b.author = obj.value("author").toString();
        b.year   = obj.value("year").toInt();
        b.isbn   = obj.value("isbn").toString();
        b.genre  = obj.value("genre").toString();
        b.status = obj.value("status").toString("to_read");
        b.rating = obj.value("rating").toInt();
        b.pages  = obj.value("pages").toInt();
        b.notes  = obj.value("notes").toString();

        // Пропуск дубликатов по ISBN
        if (!b.isbn.isEmpty() && m_db->existsByIsbn(b.isbn)) {
            skipped++;
            continue;
        }

        if (m_db->addBook(b) < 0) {
            // При ошибке — откат всей транзакции
            db.rollback();
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Ошибка при импорте книги «%1»:\n%2\nИмпорт отменён.")
                    .arg(b.title, m_db->lastError()));
            return;
        }
        imported++;
    }

    if (!db.commit()) {
        db.rollback();
        QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось завершить транзакцию."));
        return;
    }

    m_model->select();
    updateStatusBarCount();
    refreshStatistics();

    // Сообщение в статус-бар с итогами импорта
    QString msg = tr("Импортировано: %1").arg(imported);
    if (skipped > 0) msg += tr(", пропущено (дубликаты): %1").arg(skipped);
    if (errors > 0) msg += tr(", ошибок: %1").arg(errors);
    statusBar()->showMessage(msg, 5000);
}

// ─── Статистика ───

void MainWindow::refreshStatistics()
{
    // Обновление диаграммы «книги по годам»
    QMap<int, int> yearCounts = m_db->countByYear();
    m_stats->setYearCounts(yearCounts);

    // Обновление числовых карточек
    QMap<QString, int> statusCounts = m_db->countByStatus();
    int total = statusCounts["to_read"] + statusCounts["reading"] + statusCounts["done"];

    ui->lblStatTotal->setText(QString::number(total));
    ui->lblStatRead->setText(QString::number(statusCounts["done"]));
    ui->lblStatReading->setText(QString::number(statusCounts["reading"]));

    // Средний рейтинг (только для книг с рейтингом > 0)
    QList<Book> allBooks = m_db->allBooks();
    double sumRating = 0;
    int ratedCount = 0;
    for (const Book &b : allBooks) {
        if (b.rating > 0) {
            sumRating += b.rating;
            ratedCount++;
        }
    }
    double avgRating = ratedCount > 0 ? sumRating / ratedCount : 0.0;
    ui->lblStatRating->setText(QString::number(avgRating, 'f', 1));
}

void MainWindow::updateStatusBarCount()
{
    // «N книг в коллекции» в статус-баре
    QMap<QString, int> counts = m_db->countByStatus();
    int total = counts["to_read"] + counts["reading"] + counts["done"];
    statusBar()->showMessage(tr("%1 книг в коллекции").arg(total));
}
