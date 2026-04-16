#include "commands.h"
#include <QGraphicsItem>
#include <QGraphicsScene>

// =============================================================================
// AddItemCommand — добавление элемента на сцену
// =============================================================================

AddItemCommand::AddItemCommand(QGraphicsScene *scene, QGraphicsItem *item,
                               QUndoCommand *parent)
    : QUndoCommand(parent), m_scene(scene), m_item(item)
{
    // Текст отображается в кнопке "Отменить: Добавить элемент"
    setText("Добавить элемент");
}

AddItemCommand::~AddItemCommand()
{
    // Удаляем элемент из памяти только если он НЕ находится на сцене.
    // Если элемент на сцене — за его память отвечает сцена.
    if (m_owned) delete m_item;
}

// Отмена: убираем элемент со сцены, берём владение памятью
void AddItemCommand::undo()
{
    m_scene->removeItem(m_item);
    m_owned = true; // команда снова отвечает за удаление
}

// Выполнение (и повтор после Ctrl+Shift+Z): добавляем элемент на сцену
void AddItemCommand::redo()
{
    m_scene->addItem(m_item);
    m_owned = false; // сцена теперь владеет элементом
}

// =============================================================================
// RemoveItemCommand — удаление элемента со сцены
// =============================================================================

RemoveItemCommand::RemoveItemCommand(QGraphicsScene *scene, QGraphicsItem *item,
                                     QUndoCommand *parent)
    : QUndoCommand(parent), m_scene(scene), m_item(item)
{
    setText("Удалить элемент");
}

RemoveItemCommand::~RemoveItemCommand()
{
    // Аналогично AddItemCommand: удаляем только если владеем элементом
    if (m_owned) delete m_item;
}

// Отмена: возвращаем элемент на сцену, отдаём владение сцене
void RemoveItemCommand::undo()
{
    m_scene->addItem(m_item);
    m_owned = false;
}

// Выполнение: убираем элемент со сцены, берём владение памятью
void RemoveItemCommand::redo()
{
    m_scene->removeItem(m_item);
    m_owned = true;
}

// =============================================================================
// MoveItemCommand — перемещение элемента
// =============================================================================

MoveItemCommand::MoveItemCommand(QGraphicsItem *item, QPointF oldPos, QPointF newPos,
                                 QUndoCommand *parent)
    : QUndoCommand(parent), m_item(item), m_oldPos(oldPos), m_newPos(newPos)
{
    setText("Переместить элемент");
}

// Отмена: возвращаем элемент на позицию до перетаскивания
void MoveItemCommand::undo()
{
    m_item->setPos(m_oldPos);
}

// Выполнение: переставляем элемент на позицию после перетаскивания.
// Первый вызов пропускаем — QUndoStack::push() вызывает redo() сразу,
// но элемент уже стоит на newPos после перетаскивания пользователем.
// Без пропуска элемент бы "дёрнулся" на месте.
void MoveItemCommand::redo()
{
    if (m_firstRedo) { m_firstRedo = false; return; }
    m_item->setPos(m_newPos);
}
