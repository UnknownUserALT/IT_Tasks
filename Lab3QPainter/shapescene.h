#pragma once
#include <QGraphicsScene>

/**
 * @brief Расширенная QGraphicsScene с поддержкой клика по пустому месту.
 *
 * Стандартная сцена не различает клик по элементу и клик по пустому фону.
 * ShapeScene переопределяет mousePressEvent: если левая кнопка нажата
 * на пустом месте (нет элементов под курсором), испускает сигнал
 * clickedAtEmpty(). MainWindow слушает этот сигнал и добавляет новую фигуру.
 *
 * Клик по существующему элементу по-прежнему передаётся базовому классу,
 * который обрабатывает выделение и начало перетаскивания.
 */
class ShapeScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit ShapeScene(QObject *parent = nullptr);

signals:
    /// Испускается при клике левой кнопкой мыши по пустому месту сцены.
    /// scenePos — координаты точки клика в системе координат сцены.
    void clickedAtEmpty(QPointF scenePos);

protected:
    /// Переопределяем для отслеживания кликов по пустому месту
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};
