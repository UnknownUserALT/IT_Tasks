#pragma once
#include <QGraphicsObject>
#include <QTimer>

class QUndoStack;

/**
 * @brief Пользовательский элемент сцены — аналоговые часы.
 *
 * Наследует QGraphicsObject (= QObject + QGraphicsItem), что позволяет
 * использовать сигналы/слоты — в частности, подключить QTimer::timeout
 * к слоту tick() для перерисовки каждую секунду.
 *
 * Циферблат рисуется целиком через QPainter в методе paint():
 *  - окружность фона, часовые и минутные метки;
 *  - часовая, минутная и секундная стрелки на основе QTime::currentTime();
 *  - центральная заглушка (чёрная + красная точка).
 *
 * Поддерживает перемещение с фиксацией в QUndoStack и удаление
 * через контекстное меню.
 */
class ClockItem : public QGraphicsObject {
    Q_OBJECT
public:
    /// Уникальный тип для qgraphicsitem_cast
    enum { Type = QGraphicsItem::UserType + 2 };
    int type() const override { return Type; }

    /// Радиус циферблата в пикселях
    static constexpr qreal RADIUS = 60.0;

    explicit ClockItem(QUndoStack *undoStack, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)        override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)      override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private slots:
    /// Вызывается таймером каждую секунду — перезапрашивает перерисовку
    void tick();

private:
    QTimer     m_timer;     ///< Таймер обновления (интервал 1000 мс)
    QPointF    m_pressPos;  ///< Позиция в момент нажатия кнопки мыши
    QUndoStack *m_undoStack;///< Стек отмены (не владеем)
};
