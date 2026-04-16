#pragma once
#include <QUndoCommand>
#include <QPointF>

class QGraphicsItem;
class QGraphicsScene;

/**
 * @brief Команда добавления элемента на сцену.
 *
 * При redo() добавляет элемент на сцену, при undo() — убирает.
 * Владеет памятью элемента тогда, когда тот не находится на сцене:
 * деструктор удаляет элемент только если m_owned == true.
 */
class AddItemCommand : public QUndoCommand {
public:
    AddItemCommand(QGraphicsScene *scene, QGraphicsItem *item,
                   QUndoCommand *parent = nullptr);
    ~AddItemCommand() override;
    void undo() override;
    void redo() override;

private:
    QGraphicsScene *m_scene;
    QGraphicsItem  *m_item;
    bool            m_owned = false; // true, когда элемент НЕ находится на сцене
};

/**
 * @brief Команда удаления элемента со сцены.
 *
 * Зеркальна AddItemCommand: при redo() убирает элемент,
 * при undo() — возвращает обратно.
 * После удаления команда берёт на себя владение памятью элемента.
 */
class RemoveItemCommand : public QUndoCommand {
public:
    RemoveItemCommand(QGraphicsScene *scene, QGraphicsItem *item,
                      QUndoCommand *parent = nullptr);
    ~RemoveItemCommand() override;
    void undo() override;
    void redo() override;

private:
    QGraphicsScene *m_scene;
    QGraphicsItem  *m_item;
    bool            m_owned = true; // true после удаления со сцены
};

/**
 * @brief Команда перемещения элемента.
 *
 * Запоминает позицию до (oldPos) и после (newPos) перетаскивания.
 * Первый вызов redo() — холостой, т.к. перемещение уже произошло
 * через ItemIsMovable; последующие вызовы восстанавливают newPos.
 */
class MoveItemCommand : public QUndoCommand {
public:
    MoveItemCommand(QGraphicsItem *item, QPointF oldPos, QPointF newPos,
                    QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;

private:
    QGraphicsItem *m_item;
    QPointF        m_oldPos;
    QPointF        m_newPos;
    bool           m_firstRedo = true; // пропустить первый вызов — перемещение уже выполнено
};
