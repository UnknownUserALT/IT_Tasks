#include "shapescene.h"
#include <QGraphicsSceneMouseEvent>

ShapeScene::ShapeScene(QObject *parent)
    : QGraphicsScene(parent) {}

void ShapeScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && items(event->scenePos()).isEmpty())
        emit clickedAtEmpty(event->scenePos());
    QGraphicsScene::mousePressEvent(event);
}
