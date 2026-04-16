#include "commands.h"
#include <QGraphicsItem>
#include <QGraphicsScene>

// =============================================================================
// AddItemCommand
// =============================================================================

AddItemCommand::AddItemCommand(QGraphicsScene *scene, QGraphicsItem *item,
                               QUndoCommand *parent)
    : QUndoCommand(parent), m_scene(scene), m_item(item)
{
    setText("Добавить элемент");
}

AddItemCommand::~AddItemCommand()
{
    // Удаляем элемент только если он сейчас не на сцене —
    // иначе за его жизнь отвечает сцена.
    if (m_owned) delete m_item;
}

void AddItemCommand::undo()
{
    m_scene->removeItem(m_item);
    m_owned = true; // команда снова владеет элементом
}

void AddItemCommand::redo()
{
    m_scene->addItem(m_item);
    m_owned = false; // элемент передан сцене
}

// =============================================================================
// RemoveItemCommand
// =============================================================================

RemoveItemCommand::RemoveItemCommand(QGraphicsScene *scene, QGraphicsItem *item,
                                     QUndoCommand *parent)
    : QUndoCommand(parent), m_scene(scene), m_item(item)
{
    setText("Удалить элемент");
}

RemoveItemCommand::~RemoveItemCommand()
{
    if (m_owned) delete m_item;
}

void RemoveItemCommand::undo()
{
    // Возвращаем элемент на сцену — сцена снова владеет им
    m_scene->addItem(m_item);
    m_owned = false;
}

void RemoveItemCommand::redo()
{
    // Убираем элемент со сцены — команда берёт владение
    m_scene->removeItem(m_item);
    m_owned = true;
}

// =============================================================================
// MoveItemCommand
// =============================================================================

MoveItemCommand::MoveItemCommand(QGraphicsItem *item, QPointF oldPos, QPointF newPos,
                                 QUndoCommand *parent)
    : QUndoCommand(parent), m_item(item), m_oldPos(oldPos), m_newPos(newPos)
{
    setText("Переместить элемент");
}

void MoveItemCommand::undo()
{
    m_item->setPos(m_oldPos);
}

void MoveItemCommand::redo()
{
    // QUndoStack::push() сразу вызывает redo(), но элемент уже находится
    // в newPos после перетаскивания — пропускаем первое срабатывание.
    if (m_firstRedo) { m_firstRedo = false; return; }
    m_item->setPos(m_newPos);
}
