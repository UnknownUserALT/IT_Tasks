#include "clockitem.h"
#include "commands.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QTime>
#include <QUndoStack>
#include <cmath>

// =============================================================================
// Конструктор
// =============================================================================

ClockItem::ClockItem(QUndoStack *undoStack, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_undoStack(undoStack)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);

    // Каждую секунду таймер вызывает tick(), который запрашивает перерисовку.
    // Интервал 1000 мс достаточен для точности до секунды.
    connect(&m_timer, &QTimer::timeout, this, &ClockItem::tick);
    m_timer.start(1000);
}

// =============================================================================
// Геометрия
// =============================================================================

// boundingRect определяет область, которую Qt перерисовывает при update().
// Добавляем 6 пикселей к радиусу под обводку циферблата.
QRectF ClockItem::boundingRect() const
{
    const qreal r = RADIUS + 6;
    return QRectF(-r, -r, r * 2, r * 2);
}

// =============================================================================
// Таймер
// =============================================================================

void ClockItem::tick()
{
    update(); // перерисовать элемент — Qt вызовет paint()
}

// =============================================================================
// Отрисовка
// =============================================================================

// Все трансформации применяются в локальной системе координат элемента:
// центр циферблата = (0, 0).
// Стрелки рисуются через rotate() + drawLine() — после поворота ось Y
// направлена вверх, поэтому конечная точка стрелки имеет отрицательный Y.
void ClockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);

    // ── Циферблат (фон) ──────────────────────────────────────────────────
    painter->setBrush(QBrush(QColor(255, 255, 240))); // слоновая кость
    painter->setPen(QPen(Qt::black, 2));
    painter->drawEllipse(QPointF(0, 0), RADIUS, RADIUS);

    // ── Часовые метки (12 штук, шаг 30°) ────────────────────────────────
    for (int i = 0; i < 12; ++i) {
        painter->save();
        painter->rotate(i * 30.0);
        painter->setPen(QPen(Qt::black, 2));
        // Линия от (RADIUS−10) до (RADIUS−2) — длиной 8 пкс
        painter->drawLine(QPointF(0, -(RADIUS - 10)), QPointF(0, -(RADIUS - 2)));
        painter->restore();
    }

    // ── Минутные метки (60 штук, шаг 6°, без часовых позиций) ───────────
    for (int i = 0; i < 60; ++i) {
        if (i % 5 == 0) continue; // часовые позиции уже нарисованы
        painter->save();
        painter->rotate(i * 6.0);
        painter->setPen(QPen(Qt::gray, 1));
        painter->drawLine(QPointF(0, -(RADIUS - 5)), QPointF(0, -(RADIUS - 2)));
        painter->restore();
    }

    const QTime time = QTime::currentTime();

    // ── Часовая стрелка ─────────────────────────────────────────────────
    // Угол = 30° × час + 0.5° × минута (плавное движение между делениями)
    painter->save();
    painter->rotate(30.0 * (time.hour() % 12) + time.minute() * 0.5);
    painter->setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(QPointF(0, 6), QPointF(0, -RADIUS * 0.50)); // короткая и толстая
    painter->restore();

    // ── Минутная стрелка ────────────────────────────────────────────────
    // Угол = 6° × минута + 0.1° × секунда (плавное движение)
    painter->save();
    painter->rotate(6.0 * time.minute() + time.second() * 0.1);
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(QPointF(0, 8), QPointF(0, -RADIUS * 0.75)); // длиннее часовой
    painter->restore();

    // ── Секундная стрелка ───────────────────────────────────────────────
    // Угол = 6° × секунда (один оборот за 60 секунд)
    painter->save();
    painter->rotate(6.0 * time.second());
    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(QPointF(0, 12), QPointF(0, -RADIUS * 0.85)); // самая длинная
    painter->restore();

    // ── Центральная заглушка (скрывает основания стрелок) ───────────────
    painter->setBrush(QBrush(Qt::black));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(0, 0), 4.0, 4.0); // чёрный круг
    painter->setBrush(QBrush(Qt::red));
    painter->drawEllipse(QPointF(0, 0), 2.0, 2.0); // красная точка поверх
}

// =============================================================================
// Обработчики событий
// =============================================================================

void ClockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressPos = pos(); // запоминаем позицию до перетаскивания
    QGraphicsObject::mousePressEvent(event);
}

void ClockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);
    // Если позиция изменилась — фиксируем перемещение в стеке отмены
    if (m_undoStack && pos() != m_pressPos)
        m_undoStack->push(new MoveItemCommand(this, m_pressPos, pos()));
}

void ClockItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *delAction = menu.addAction("Удалить");
    connect(delAction, &QAction::triggered, [this]() {
        if (m_undoStack)
            m_undoStack->push(new RemoveItemCommand(scene(), this));
    });
    menu.exec(event->screenPos());
}
