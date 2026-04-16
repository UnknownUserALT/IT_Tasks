#pragma once
#include <QMainWindow>
#include <QUndoStack>
#include <QTimer>
#include "shapeitem.h"

class ShapeScene;
class ShapeView;

/// Текущий активный инструмент на панели
enum class ActiveTool { Select, Rectangle, Ellipse, Star, Clock };

/**
 * @brief Главное окно приложения — интерактивный редактор фигур.
 *
 * Компонует интерфейс из:
 *  - ShapeScene + ShapeView (холст для размещения фигур);
 *  - QToolBar с кнопками выбора инструмента, экспорта и Undo/Redo;
 *  - QUndoStack — центральный стек команд для отмены/повтора действий;
 *  - QTimer (100 мс) — периодическая проверка коллизий между фигурами.
 *
 * Логика добавления фигур:
 *  ShapeScene::clickedAtEmpty → onSceneClicked() → addShape() / addClock()
 *  → создаём элемент → помещаем в AddItemCommand → push() в стек.
 *
 * Логика коллизий:
 *  checkCollisions() обходит все элементы сцены, вызывает collidingItems()
 *  и обновляет флаг m_colliding у каждого ShapeItem — тот сам меняет цвет обводки.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onSceneClicked(QPointF pos); ///< Добавить фигуру в точку клика
    void onDeletePressed();           ///< Удалить все выделенные элементы
    void checkCollisions();           ///< Проверить и обновить коллизии
    void exportScene();               ///< Экспортировать сцену в PNG

private:
    void setupToolBar(); ///< Создать панель инструментов
    void setupScene();   ///< Создать сцену и вид

    void addShape(ShapeType type, QPointF pos); ///< Добавить ShapeItem на сцену
    void addClock(QPointF pos);                 ///< Добавить ClockItem на сцену

    ShapeScene  *m_scene          = nullptr;
    ShapeView   *m_view           = nullptr;
    QUndoStack  *m_undoStack      = nullptr;
    QTimer      *m_collisionTimer = nullptr;
    ActiveTool   m_activeTool     = ActiveTool::Select;
};
