#include "shapeview.h"
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>

// Конструктор — настраиваем параметры отображения сцены.
ShapeView::ShapeView(QWidget *parent)
    : QGraphicsView(parent)
{
    // Сглаживание краёв фигур при отрисовке (без него края "ступенчатые")
    setRenderHint(QPainter::Antialiasing);

    // Плавная фильтрация при масштабировании растровых изображений
    setRenderHint(QPainter::SmoothPixmapTransform);

    // Отключаем перетаскивание самого вида — иначе зажатая кнопка мыши
    // двигала бы "камеру", а не фигуры
    setDragMode(QGraphicsView::NoDrag);

    // Вид получает фокус клавиатуры при клике на него.
    // Без этого keyPressEvent не будет срабатывать.
    setFocusPolicy(Qt::StrongFocus);
}

// Обработчик колеса мыши — вращение и масштабирование фигур.
void ShapeView::wheelEvent(QWheelEvent *event) {
    // Читаем, какие клавиши-модификаторы зажаты (Ctrl, Shift, Alt...)
    const Qt::KeyboardModifiers mods = event->modifiers();

    if (mods & Qt::ControlModifier) {
        // ── Вращение фигуры под курсором (Ctrl + колесо) ────────────────
        QGraphicsItem *item = itemAt(event->position().toPoint());
        if (item) {
            // angleDelta().y() > 0 — колесо вверх, < 0 — вниз
            const qreal delta = (event->angleDelta().y() > 0) ? 5.0 : -5.0;
            // Добавляем шаг к текущему углу поворота
            item->setRotation(item->rotation() + delta);
        }
        // Помечаем событие как обработанное — дальше не передаём
        event->accept();

    } else if (mods & Qt::ShiftModifier) {
        // ── Масштабирование фигуры под курсором (Shift + колесо) ────────
        QGraphicsItem *item = itemAt(event->position().toPoint());
        if (item) {
            // На macOS Shift+прокрутка передаётся как горизонтальная дельта,
            // поэтому берём x() как запасной вариант
            const int wheelDelta = event->angleDelta().y() != 0
                                       ? event->angleDelta().y()
                                       : event->angleDelta().x();
            // Колесо вверх — увеличить на 10%, вниз — уменьшить на 10%
            const qreal factor = (wheelDelta > 0) ? 1.1 : 0.9;
            const qreal newScale = item->scale() * factor;
            // Ограничиваем диапазон масштаба: не меньше 5% и не больше 2000%
            if (newScale > 0.05 && newScale < 20.0)
                item->setScale(newScale);
        }
        event->accept();

    } else {
        // ── Обычная прокрутка сцены (без модификаторов) ──────────────────
        QGraphicsView::wheelEvent(event);
    }
}

// Обработчик нажатий клавиш клавиатуры.
void ShapeView::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_PageUp:
        // Поднять все выделенные фигуры на один уровень (поверх остальных).
        // zValue определяет порядок отрисовки: больше z — рисуется поверх.
        for (QGraphicsItem *item : scene()->selectedItems())
            item->setZValue(item->zValue() + 1);
        event->accept();
        break;

    case Qt::Key_PageDown:
        // Опустить все выделенные фигуры на один уровень (под остальные)
        for (QGraphicsItem *item : scene()->selectedItems())
            item->setZValue(item->zValue() - 1);
        event->accept();
        break;

    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        // Сигнал перехватывает MainWindow и удаляет выделенные фигуры
        // через стек отмены, чтобы Ctrl+Z мог их вернуть
        emit deletePressed();
        event->accept();
        break;

    default:
        // Все остальные клавиши передаём родителю (стандартное поведение вида)
        QGraphicsView::keyPressEvent(event);
    }
}
