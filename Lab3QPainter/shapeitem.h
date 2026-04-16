#pragma once
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QPainterPath>

/// Типы поддерживаемых геометрических фигур
enum class ShapeType { Rectangle, Ellipse, Star };

class QUndoStack;

/**
 * @brief Пользовательский элемент сцены — геометрическая фигура.
 *
 * Поддерживает три формы: прямоугольник, эллипс и пятиконечная звезда.
 * Реализует:
 *  - перетаскивание и выделение через стандартные флаги Qt;
 *  - двойной клик → диалог выбора цвета заливки;
 *  - контекстное меню → настройка толщины и стиля обводки / удаление;
 *  - hover-эффект (тень) и эффект выделения (колоризация);
 *  - подсветку красной обводкой при коллизии с другими фигурами;
 *  - запись операции перемещения в QUndoStack.
 */
class ShapeItem : public QGraphicsItem {
public:
    /// Уникальный тип для qgraphicsitem_cast
    enum { Type = QGraphicsItem::UserType + 1 };
    int type() const override { return Type; }

    /// Размер фигуры (сторона/диаметр описывающего квадрата) в пикселях
    static constexpr qreal SIZE = 80.0;

    explicit ShapeItem(ShapeType type, QUndoStack *undoStack,
                       QGraphicsItem *parent = nullptr);

    // ── Обязательные методы QGraphicsItem ───────────────────────────────
    QRectF       boundingRect() const override;
    QPainterPath shape()        const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    // ── Геттеры ─────────────────────────────────────────────────────────
    ShapeType shapeType() const { return m_type; }
    QColor    fillColor() const { return m_brush.color(); }
    QPen      basePen()   const { return m_basePen; }

    // ── Сеттеры ─────────────────────────────────────────────────────────
    void setFillColor(const QColor &color); ///< Изменить цвет заливки
    void setBasePen(const QPen &pen);       ///< Изменить базовое перо обводки
    void setColliding(bool colliding);      ///< Включить/выключить подсветку коллизии

protected:
    // ── Обработчики событий сцены ────────────────────────────────────────
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;  ///< Открыть QColorDialog
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override; ///< Меню пера / удаление
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)  override;       ///< Включить тень
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)  override;       ///< Убрать тень
    void mousePressEvent(QGraphicsSceneMouseEvent *event)   override;      ///< Запомнить позицию
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;      ///< Зафиксировать перемещение
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath buildPath() const;                   ///< Построить путь текущей фигуры
    static QPainterPath buildStarPath(qreal size);   ///< Построить путь звезды
    void updateEffect();                              ///< Обновить графический эффект

    ShapeType   m_type;
    QBrush      m_brush;               ///< Заливка фигуры
    QPen        m_basePen;             ///< Обычная обводка (не коллизионная)
    bool        m_colliding = false;   ///< Флаг пересечения с другой фигурой
    bool        m_hovered   = false;   ///< Флаг наведения курсора
    QPointF     m_pressPos;            ///< Позиция в момент нажатия кнопки мыши
    QUndoStack *m_undoStack;           ///< Стек отмены (не владеем)
};
