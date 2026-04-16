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
    void undo() override; ///< Убрать элемент со сцены
    void redo() override; ///< Добавить элемент на сцену

private:
    QGraphicsScene *m_scene;
    QGraphicsItem  *m_item;
    /// true — команда владеет памятью элемента (элемент не на сцене)
    /// false — сцена владеет элементом
    bool            m_owned = false;
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
    void undo() override; ///< Вернуть элемент на сцену
    void redo() override; ///< Убрать элемент со сцены

private:
    QGraphicsScene *m_scene;
    QGraphicsItem  *m_item;
    /// true после удаления со сцены — команда владеет памятью
    bool            m_owned = true;
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
    void undo() override; ///< Вернуть элемент на oldPos
    void redo() override; ///< Переставить элемент на newPos

private:
    QGraphicsItem *m_item;
    QPointF        m_oldPos; ///< Позиция до перетаскивания
    QPointF        m_newPos; ///< Позиция после перетаскивания
    /// Флаг пропуска первого redo() — перемещение уже выполнено пользователем
    bool           m_firstRedo = true;
};
