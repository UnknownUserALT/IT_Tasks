#include "mainwindow.h"
#include "shapescene.h"
#include "shapeview.h"
#include "shapeitem.h"
#include "clockitem.h"
#include "commands.h"

#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QStatusBar>
#include <QLabel>
#include <QSet>

// =============================================================================
// Конструктор
// =============================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // QUndoStack создаём первым — он нужен сцене и элементам
    m_undoStack = new QUndoStack(this);

    setupScene();
    setupToolBar();

    // Таймер проверки коллизий: каждые 100 мс обходим сцену и обновляем
    // флаги пересечений у всех ShapeItem.
    m_collisionTimer = new QTimer(this);
    connect(m_collisionTimer, &QTimer::timeout, this, &MainWindow::checkCollisions);
    m_collisionTimer->start(100);

    setWindowTitle("Редактор фигур");
    resize(1300, 900);
    statusBar()->showMessage("Выберите инструмент и кликните по холсту для добавления фигуры.");
}

// =============================================================================
// Инициализация сцены и вида
// =============================================================================

void MainWindow::setupScene()
{
    // Фиксированный размер сцены 1200×800; при необходимости вид
    // автоматически покажет полосы прокрутки.
    m_scene = new ShapeScene(this);
    m_scene->setSceneRect(0, 0, 1200, 800);

    m_view = new ShapeView(this);
    m_view->setScene(m_scene);
    m_view->setBackgroundBrush(QBrush(QColor(220, 220, 220))); // серый фон холста

    setCentralWidget(m_view);

    // Клик по пустому месту сцены → добавить фигуру
    connect(m_scene, &ShapeScene::clickedAtEmpty, this, &MainWindow::onSceneClicked);
    // Delete/Backspace из вида → удалить выделенные элементы
    connect(m_view,  &ShapeView::deletePressed,   this, &MainWindow::onDeletePressed);
}

// =============================================================================
// Панель инструментов
// =============================================================================

void MainWindow::setupToolBar()
{
    QToolBar *tb = addToolBar("Инструменты");
    tb->setMovable(false);

    // QActionGroup обеспечивает взаимное исключение кнопок-переключателей
    auto *toolGroup = new QActionGroup(this);
    toolGroup->setExclusive(true);

    // Вспомогательная лямбда: регистрирует кнопку-переключатель инструмента
    auto addTool = [&](const QString &label, ActiveTool tool) -> QAction * {
        QAction *a = tb->addAction(label);
        a->setCheckable(true);
        toolGroup->addAction(a);
        connect(a, &QAction::triggered, [this, tool, label]() {
            m_activeTool = tool;
            statusBar()->showMessage("Инструмент: " + label);
        });
        return a;
    };

    addTool("Выбор",         ActiveTool::Select)->setChecked(true);
    addTool("Прямоугольник", ActiveTool::Rectangle);
    addTool("Эллипс",        ActiveTool::Ellipse);
    addTool("Звезда",        ActiveTool::Star);
    addTool("Часы",          ActiveTool::Clock);

    tb->addSeparator();

    QAction *exportAction = tb->addAction("Экспорт PNG");
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportScene);

    tb->addSeparator();

    // Действия отмены/возврата берём напрямую из стека —
    // они автоматически обновляют текст ("Отменить: Добавить элемент" и т.п.)
    QAction *undoAction = m_undoStack->createUndoAction(this, "Отменить");
    undoAction->setShortcut(QKeySequence::Undo); // Ctrl+Z

    QAction *redoAction = m_undoStack->createRedoAction(this, "Повторить");
    redoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Z));

    tb->addAction(undoAction);
    tb->addAction(redoAction);

    // Строка-подсказка с перечнем горячих клавиш
    auto *hint = new QLabel(
        "  Ctrl+Колесо: вращение   Shift+Колесо: масштаб   PgUp/PgDn: z-порядок   "
        "Del: удалить   2хКлик: цвет   ПКМ: настройки обводки");
    hint->setStyleSheet("color: #555; font-size: 11px;");
    tb->addWidget(hint);
}

// =============================================================================
// Слоты
// =============================================================================

// Вызывается при клике по пустому месту сцены.
// В зависимости от активного инструмента создаём нужный тип фигуры.
void MainWindow::onSceneClicked(QPointF pos)
{
    switch (m_activeTool) {
    case ActiveTool::Rectangle: addShape(ShapeType::Rectangle, pos); break;
    case ActiveTool::Ellipse:   addShape(ShapeType::Ellipse,   pos); break;
    case ActiveTool::Star:      addShape(ShapeType::Star,      pos); break;
    case ActiveTool::Clock:     addClock(pos);                        break;
    default: break; // ActiveTool::Select — ничего не добавляем
    }
}

// Создаёт ShapeItem, помещает операцию добавления в стек отмены.
// Элемент не добавляется на сцену напрямую — это делает AddItemCommand::redo(),
// которую QUndoStack::push() вызывает немедленно.
void MainWindow::addShape(ShapeType type, QPointF pos)
{
    auto *item = new ShapeItem(type, m_undoStack);
    item->setPos(pos);
    m_undoStack->push(new AddItemCommand(m_scene, item));
}

void MainWindow::addClock(QPointF pos)
{
    auto *item = new ClockItem(m_undoStack);
    item->setPos(pos);
    m_undoStack->push(new AddItemCommand(m_scene, item));
}

// Удаляет все выделенные элементы как один шаг отмены (макрос).
// beginMacro/endMacro объединяет несколько команд в одну запись стека,
// чтобы Ctrl+Z удалял всю группу сразу, а не по одному элементу.
void MainWindow::onDeletePressed()
{
    const auto selected = m_scene->selectedItems();
    if (selected.isEmpty()) return;

    m_undoStack->beginMacro("Удалить элементы");
    for (QGraphicsItem *item : selected)
        m_undoStack->push(new RemoveItemCommand(m_scene, item));
    m_undoStack->endMacro();
}

// =============================================================================
// Обнаружение коллизий
// =============================================================================

// Вызывается таймером каждые 100 мс.
// collidingItems() использует shape() каждого элемента для точного
// определения пересечений (не просто bounding rect).
void MainWindow::checkCollisions()
{
    const QList<QGraphicsItem *> all = m_scene->items();
    QSet<QGraphicsItem *> colliding;

    // Первый проход: собираем все элементы, у которых есть хоть одно пересечение
    for (QGraphicsItem *item : all) {
        if (!item->collidingItems().isEmpty())
            colliding.insert(item);
    }

    // Второй проход: обновляем флаг коллизии только у ShapeItem.
    // ClockItem не подсвечиваем — у него нет метода setColliding().
    for (QGraphicsItem *item : all) {
        if (auto *shape = qgraphicsitem_cast<ShapeItem *>(item))
            shape->setColliding(colliding.contains(item));
    }
}

// =============================================================================
// Экспорт сцены
// =============================================================================

// Рендерит всю сцену в QImage через QGraphicsScene::render(QPainter*) —
// это стандартный Qt-способ «снять скриншот» сцены в оффскринный буфер.
void MainWindow::exportScene()
{
    const QString filename = QFileDialog::getSaveFileName(
        this, "Экспорт сцены в PNG",
        QDir::homePath() + "/scene.png",
        "PNG изображения (*.png)");
    if (filename.isEmpty()) return;

    // Создаём QImage того же размера, что и прямоугольник сцены
    const QRectF rect = m_scene->sceneRect();
    QImage image(rect.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white); // белый фон вместо прозрачного

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    m_scene->render(&painter); // Qt рисует все элементы сцены в наш буфер
    painter.end();

    if (image.save(filename, "PNG"))
        statusBar()->showMessage("Экспортировано: " + filename, 4000);
    else
        statusBar()->showMessage("Ошибка экспорта!", 4000);
}
