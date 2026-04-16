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
    // ItemIsMovable            — перетаскивание мышью
    // ItemIsSelectable         — выделение кликом
    // ItemSendsGeometryChanges — уведомления при изменении позиции (нужно для undo)
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);

    // Подключаем таймер к слоту tick(): каждую секунду таймер испускает
    // сигнал timeout → вызывается tick() → update() → перерисовка со свежим временем
    connect(&m_timer, &QTimer::timeout, this, &ClockItem::tick);
    m_timer.start(1000); // интервал 1000 мс = 1 секунда
}

// =============================================================================
// Геометрия
// =============================================================================

// Определяет прямоугольник, внутри которого гарантированно находится весь элемент.
// Qt использует его для оптимизации перерисовки — не трогает то, что за пределами.
QRectF ClockItem::boundingRect() const
{
    // +6 пикселей запаса с каждой стороны под обводку циферблата
    const qreal r = RADIUS + 6;
    return QRectF(-r, -r, r * 2, r * 2);
}

// =============================================================================
// Таймер
// =============================================================================

// Вызывается таймером каждую секунду.
// update() сообщает Qt: "перерисуй этот элемент" — Qt вызовет paint().
void ClockItem::tick()
{
    update();
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
    // Сглаживание — плавные края без "лесенки"
    painter->setRenderHint(QPainter::Antialiasing);

    // ── Циферблат (фон) ──────────────────────────────────────────────────
    painter->setBrush(QBrush(QColor(255, 255, 240))); // цвет слоновой кости
    painter->setPen(QPen(Qt::black, 2));
    painter->drawEllipse(QPointF(0, 0), RADIUS, RADIUS);

    // ── Часовые метки (12 штук, шаг 30°) ────────────────────────────────
    for (int i = 0; i < 12; ++i) {
        painter->save();             // сохраняем текущую систему координат
        painter->rotate(i * 30.0);  // поворачиваем на i × 30°
        painter->setPen(QPen(Qt::black, 2));
        // Линия идёт вдоль повёрнутой оси Y, от (RADIUS−10) до (RADIUS−2)
        painter->drawLine(QPointF(0, -(RADIUS - 10)), QPointF(0, -(RADIUS - 2)));
        painter->restore();          // возвращаем систему координат обратно
    }

    // ── Минутные метки (60 штук, шаг 6°, без часовых позиций) ───────────
    for (int i = 0; i < 60; ++i) {
        if (i % 5 == 0) continue; // часовые позиции уже нарисованы — пропускаем
        painter->save();
        painter->rotate(i * 6.0);  // 360° / 60 меток = 6° на метку
        painter->setPen(QPen(Qt::gray, 1));
        // Короче часовых: от (RADIUS−5) до (RADIUS−2)
        painter->drawLine(QPointF(0, -(RADIUS - 5)), QPointF(0, -(RADIUS - 2)));
        painter->restore();
    }

    // Берём текущее время один раз — одинаковое для всех стрелок
    const QTime time = QTime::currentTime();

    // ── Часовая стрелка ─────────────────────────────────────────────────
    // Угол = 30° × час + 0.5° × минута (плавное движение между делениями)
    // % 12 — переводим из 24-часового формата в 12-часовой
    painter->save();
    painter->rotate(30.0 * (time.hour() % 12) + time.minute() * 0.5);
    painter->setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap)); // толстая, с круглым концом
    painter->drawLine(QPointF(0, 6), QPointF(0, -RADIUS * 0.50));     // короткая: 50% радиуса
    painter->restore();

    // ── Минутная стрелка ────────────────────────────────────────────────
    // Угол = 6° × минута + 0.1° × секунда (плавное движение)
    painter->save();
    painter->rotate(6.0 * time.minute() + time.second() * 0.1);
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(QPointF(0, 8), QPointF(0, -RADIUS * 0.75));     // длиннее часовой: 75% радиуса
    painter->restore();

    // ── Секундная стрелка ───────────────────────────────────────────────
    // Угол = 6° × секунда (один полный оборот за 60 секунд)
    painter->save();
    painter->rotate(6.0 * time.second());
    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));   // красная и тонкая
    painter->drawLine(QPointF(0, 12), QPointF(0, -RADIUS * 0.85));    // самая длинная: 85% радиуса
    painter->restore();

    // ── Центральная заглушка (скрывает основания стрелок) ───────────────
    painter->setBrush(QBrush(Qt::black));
    painter->setPen(Qt::NoPen);                                // без обводки
    painter->drawEllipse(QPointF(0, 0), 4.0, 4.0);            // чёрный круг
    painter->setBrush(QBrush(Qt::red));
    painter->drawEllipse(QPointF(0, 0), 2.0, 2.0);            // красная точка поверх
}

// =============================================================================
// Обработчики событий
// =============================================================================

// Запоминаем позицию до начала перетаскивания
void ClockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressPos = pos();
    QGraphicsObject::mousePressEvent(event);
}

// После перетаскивания: если позиция изменилась — фиксируем в стеке отмены
void ClockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);
    if (m_undoStack && pos() != m_pressPos)
        m_undoStack->push(new MoveItemCommand(this, m_pressPos, pos()));
}

// Контекстное меню часов — только пункт "Удалить"
void ClockItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *delAction = menu.addAction("Удалить");
    // Удаление через команду — поддерживает Ctrl+Z
    connect(delAction, &QAction::triggered, [this]() {
        if (m_undoStack)
            m_undoStack->push(new RemoveItemCommand(scene(), this));
    });
    menu.exec(event->screenPos());
}
