#include "shapescene.h"
#include <QGraphicsSceneMouseEvent>

// Конструктор — просто передаём parent родительскому классу QGraphicsScene.
// Вся инициализация сцены происходит внутри QGraphicsScene.
ShapeScene::ShapeScene(QObject *parent)
    : QGraphicsScene(parent) {}

// Переопределяем обработчик нажатия кнопки мыши на сцене.
// Стандартный QGraphicsScene не различает клик по фигуре и по пустому месту —
// здесь мы добавляем эту логику.
void ShapeScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // items(pos) возвращает список фигур в точке клика.
    // Если список пуст — место свободно, испускаем сигнал с координатами.
    if (event->button() == Qt::LeftButton && items(event->scenePos()).isEmpty())
        emit clickedAtEmpty(event->scenePos());

    // Обязательно вызываем родительский метод — он обрабатывает
    // выделение фигур и начало перетаскивания при клике по элементу.
    QGraphicsScene::mousePressEvent(event);
}
