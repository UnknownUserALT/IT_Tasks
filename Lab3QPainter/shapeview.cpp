#include "shapeview.h"
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>

ShapeView::ShapeView(QWidget *parent)
    : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::NoDrag);
    setFocusPolicy(Qt::StrongFocus);
}

void ShapeView::wheelEvent(QWheelEvent *event) {
    const Qt::KeyboardModifiers mods = event->modifiers();

    if (mods & Qt::ControlModifier) {
        // ── Вращение фигуры под курсором (Ctrl + колесо) ────────────────
        QGraphicsItem *item = itemAt(event->position().toPoint());
        if (item) {
            const qreal delta = (event->angleDelta().y() > 0) ? 5.0 : -5.0;
            item->setRotation(item->rotation() + delta);
        }
        event->accept();

    } else if (mods & Qt::ShiftModifier) {
        // ── Масштабирование фигуры под курсором (Shift + колесо) ────────
        QGraphicsItem *item = itemAt(event->position().toPoint());
        if (item) {
            // На macOS Shift+прокрутка передаётся как горизонтальная дельта
            const int wheelDelta = event->angleDelta().y() != 0
                                       ? event->angleDelta().y()
                                       : event->angleDelta().x();
            const qreal factor = (wheelDelta > 0) ? 1.1 : 0.9;
            const qreal newScale = item->scale() * factor;
            if (newScale > 0.05 && newScale < 20.0) // ограничиваем диапазон масштаба
                item->setScale(newScale);
        }
        event->accept();

    } else {
        // ── Обычная прокрутка сцены ──────────────────────────────────────
        QGraphicsView::wheelEvent(event);
    }
}

void ShapeView::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_PageUp:
        // Поднять выделенные фигуры на один уровень по z-порядку
        for (QGraphicsItem *item : scene()->selectedItems())
            item->setZValue(item->zValue() + 1);
        event->accept();
        break;
    case Qt::Key_PageDown:
        // Опустить выделенные фигуры на один уровень по z-порядку
        for (QGraphicsItem *item : scene()->selectedItems())
            item->setZValue(item->zValue() - 1);
        event->accept();
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        // Удалить выделенные фигуры
        emit deletePressed();
        event->accept();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
