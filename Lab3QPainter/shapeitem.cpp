#include "shapeitem.h"
#include "commands.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsColorizeEffect>
#include <QColorDialog>
#include <QMenu>
#include <QRandomGenerator>
#include <QUndoStack>
#include <cmath>

// Генерация случайного насыщенного цвета заливки.
// Нижняя граница 80 исключает слишком тёмные цвета,
// верхняя граница 230 — слишком светлые (почти белые).
static QColor randomColor()
{
    return QColor(
        QRandomGenerator::global()->bounded(80, 230),
        QRandomGenerator::global()->bounded(80, 230),
        QRandomGenerator::global()->bounded(80, 230));
}

// =============================================================================
// Конструктор
// =============================================================================

ShapeItem::ShapeItem(ShapeType type, QUndoStack *undoStack, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_type(type), m_undoStack(undoStack)
{
    // Каждая новая фигура получает случайный цвет заливки
    m_brush   = QBrush(randomColor());
    // Чёрная обводка толщиной 2 пикселя по умолчанию
    m_basePen = QPen(Qt::black, 2);

    // ItemIsMovable            — встроенное перетаскивание мышью
    // ItemIsSelectable         — поддержка выделения кликом
    // ItemSendsGeometryChanges — уведомления itemChange при движении (нужно для undo)
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);

    // Без этого hoverEnterEvent/hoverLeaveEvent не вызываются
    setAcceptHoverEvents(true);
}

// =============================================================================
// Геометрия
// =============================================================================

QRectF ShapeItem::boundingRect() const
{
    const qreal s = SIZE;
    // Добавляем по 2 пикселя с каждой стороны под толстую обводку (до 8 пкс),
    // чтобы она не обрезалась границей bounding rect.
    return QRectF(-s / 2 - 2, -s / 2 - 2, s + 4, s + 4);
}

// shape() используется Qt для точного определения коллизий и hit-testing.
// Возвращаем реальный контур фигуры, а не описывающий прямоугольник.
QPainterPath ShapeItem::shape() const
{
    return buildPath();
}

// Строит QPainterPath в зависимости от типа фигуры.
// Все координаты — в системе координат элемента (центр = (0, 0)).
// Путь используется как для отрисовки (paint), так и для hit-testing и коллизий (shape()).
QPainterPath ShapeItem::buildPath() const
{
    QPainterPath path;
    const qreal s = SIZE; // 80 пкс

    switch (m_type) {
    case ShapeType::Rectangle:
        // Прямоугольник со стороной s, симметричный относительно центра.
        // Верхний левый угол (-s/2, -s/2) = (-40, -40),
        // нижний правый     ( s/2,  s/2) = ( 40,  40).
        path.addRect(-s / 2, -s / 2, s, s);
        break;

    case ShapeType::Ellipse:
        // Окружность с радиусом s/2 = 40 вокруг центра (0, 0).
        // Оба радиуса одинаковы → получается круг, а не эллипс.
        path.addEllipse(QPointF(0, 0), s / 2, s / 2);
        break;

    case ShapeType::Star:
        // Пятиконечная звезда строится отдельным методом —
        // алгоритм сложнее, чем для базовых примитивов.
        path = buildStarPath(s);
        break;
    }
    return path;
}

// Строит контур пятиконечной звезды через чередование внешних и внутренних вершин.
//
// Идея: расставить 10 точек по кругу с шагом 36° (= 360° / 10).
// Чётные точки (i = 0, 2, 4, 6, 8) — вершины лучей, лежат на outerR.
// Нечётные точки (i = 1, 3, 5, 7, 9) — впадины между лучами, лежат на innerR.
//
// Начальный угол −π/2 (= −90°) направляет первую вершину строго вверх,
// чтобы звезда стояла "остриём вверх", а не лежала на боку.
//
// Соотношение innerR / outerR = 0.4 — стандартное для классической звезды.
// Меньше (0.2) → острее лучи; больше (0.5+) → лучи становятся тупыми.
QPainterPath ShapeItem::buildStarPath(qreal size)
{
    QPainterPath path;
    const int   n      = 5;              // количество лучей звезды
    const qreal outerR = size / 2.0;    // радиус вершин лучей = 40 пкс
    const qreal innerR = outerR * 0.4;  // радиус впадин        = 16 пкс

    for (int i = 0; i < n * 2; ++i) {
        // Угол i-й точки: стартуем с −π/2 и делаем шаги по π/n = 36°.
        const qreal angle = -M_PI / 2.0 + i * M_PI / n;

        // Чётный индекс → внешняя вершина луча, нечётный → внутренняя впадина.
        const qreal r = (i % 2 == 0) ? outerR : innerR;

        // Перевод полярных координат (r, angle) в декартовы (x, y).
        QPointF pt(r * std::cos(angle), r * std::sin(angle));

        if (i == 0) path.moveTo(pt); // первая точка — начало контура, линия не рисуется
        else        path.lineTo(pt); // остальные — прямые отрезки от предыдущей точки
    }

    // Соединяем последнюю точку с первой, замыкая контур звезды
    path.closeSubpath();
    return path;
}

// =============================================================================
// Отрисовка
// =============================================================================

void ShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Сглаживание — плавные края без "лесенки" из пикселей
    painter->setRenderHint(QPainter::Antialiasing);

    // При коллизии обводка становится красной (толщина 3);
    // иначе применяется перо, выбранное пользователем.
    painter->setPen(m_colliding ? QPen(Qt::red, 3) : m_basePen);
    painter->setBrush(m_brush);
    painter->drawPath(buildPath());
}

// =============================================================================
// Публичные сеттеры
// =============================================================================

// Изменяет цвет заливки и запрашивает перерисовку.
// Вызывается из mouseDoubleClickEvent после выбора цвета в диалоге.
void ShapeItem::setFillColor(const QColor &color)
{
    m_brush.setColor(color);
    update(); // без update() изменение не отобразится на экране
}

// Изменяет перо обводки (толщину и стиль) и запрашивает перерисовку.
// Вызывается из контекстного меню.
void ShapeItem::setBasePen(const QPen &pen)
{
    m_basePen = pen;
    update();
}

// Вызывается из MainWindow::checkCollisions() каждые 100 мс.
// update() вызывается только при реальном изменении флага — экономим перерисовки.
void ShapeItem::setColliding(bool colliding)
{
    if (m_colliding != colliding) {
        m_colliding = colliding;
        update();
    }
}

// =============================================================================
// Графические эффекты
// =============================================================================

// Применяет нужный QGraphicsEffect в зависимости от текущего состояния.
// Приоритет: выделен > наведён > нет эффекта.
// setGraphicsEffect() удаляет предыдущий эффект автоматически.
void ShapeItem::updateEffect()
{
    if (isSelected()) {
        // Синеватый оттенок сигнализирует о выделении
        auto *fx = new QGraphicsColorizeEffect();
        fx->setColor(QColor(60, 120, 255));
        fx->setStrength(0.35); // 0 = без изменений, 1 = полная замена цвета
        setGraphicsEffect(fx);
    } else if (m_hovered) {
        // Мягкая тень подчёркивает активный элемент под курсором
        auto *fx = new QGraphicsDropShadowEffect();
        fx->setBlurRadius(14);      // степень размытости тени
        fx->setOffset(4, 4);        // смещение тени вправо и вниз
        fx->setColor(QColor(0, 0, 0, 170)); // чёрный цвет с прозрачностью
        setGraphicsEffect(fx);
    } else {
        setGraphicsEffect(nullptr); // эффект не нужен — убираем
    }
}

// =============================================================================
// Обработчики событий
// =============================================================================

// Двойной клик открывает системный диалог выбора цвета заливки.
void ShapeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // Передаём текущий цвет как начальный — пользователь видит уже выбранный цвет
    QColor color = QColorDialog::getColor(m_brush.color(), nullptr, "Цвет заливки");
    // isValid() == false если пользователь нажал "Отмена"
    if (color.isValid())
        setFillColor(color);
    QGraphicsItem::mouseDoubleClickEvent(event);
}

// Контекстное меню (правая кнопка мыши) позволяет изменить
// толщину и стиль обводки, а также удалить фигуру.
void ShapeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;

    // ── Толщина обводки ──────────────────────────────────────────────────
    QMenu *widthMenu = menu.addMenu("Толщина обводки");
    for (int w : {1, 2, 3, 5, 8}) {
        QAction *a = widthMenu->addAction(QString::number(w) + " пкс");
        // Используем QObject::connect, так как ShapeItem не наследует QObject
        QObject::connect(a, &QAction::triggered, a, [this, w]() {
            m_basePen.setWidth(w);
            update();
        });
    }

    // ── Стиль обводки ────────────────────────────────────────────────────
    QMenu *styleMenu = menu.addMenu("Стиль обводки");
    const QList<QPair<QString, Qt::PenStyle>> styles = {
        {"Сплошная",      Qt::SolidLine},
        {"Пунктир",       Qt::DashLine},
        {"Точки",         Qt::DotLine},
        {"Штрих-пунктир", Qt::DashDotLine},
    };
    for (const auto &[name, style] : styles) {
        QAction *a = styleMenu->addAction(name);
        QObject::connect(a, &QAction::triggered, a, [this, style]() {
            m_basePen.setStyle(style);
            update();
        });
    }

    menu.addSeparator();

    // Удаление через команду — операция поддерживает отмену (Ctrl+Z)
    QAction *delAction = menu.addAction("Удалить");
    QObject::connect(delAction, &QAction::triggered, delAction, [this]() {
        if (m_undoStack)
            m_undoStack->push(new RemoveItemCommand(scene(), this));
    });

    // Показываем меню в позиции курсора и блокируем до выбора пункта
    menu.exec(event->screenPos());
}

// Курсор вошёл в область фигуры — включаем тень
void ShapeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    updateEffect();
    QGraphicsItem::hoverEnterEvent(event);
}

// Курсор покинул область фигуры — убираем тень
void ShapeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    updateEffect();
    QGraphicsItem::hoverLeaveEvent(event);
}

// Запоминаем позицию в начале перетаскивания,
// чтобы потом сформировать MoveItemCommand.
void ShapeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressPos = pos();
    QGraphicsItem::mousePressEvent(event);
}

// По окончании перетаскивания сравниваем текущую позицию с начальной.
// Если фигура сдвинулась — добавляем команду перемещения в стек отмены.
void ShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if (m_undoStack && pos() != m_pressPos)
        m_undoStack->push(new MoveItemCommand(this, m_pressPos, pos()));
}

// itemChange вызывается Qt при различных изменениях состояния элемента.
// Нас интересует только смена флага выделения — обновляем эффект.
QVariant ShapeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged)
        updateEffect();
    return QGraphicsItem::itemChange(change, value);
}
