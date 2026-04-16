#pragma once
#include <QGraphicsView>

/**
 * @brief Расширенный QGraphicsView с поддержкой трансформаций и горячих клавиш.
 *
 * Переопределяет два обработчика:
 *
 * wheelEvent — интерпретирует колесо мыши с модификаторами:
 *   - Ctrl  + колесо → вращение фигуры под курсором (setRotation, шаг 5°);
 *   - Shift + колесо → масштабирование фигуры под курсором (setScale, ×1.1/×0.9).
 *   На macOS Shift+прокрутка приходит как горизонтальная дельта —
 *   используем angleDelta().x() как запасной вариант.
 *   Без модификатора — стандартная прокрутка сцены.
 *
 * keyPressEvent — обрабатывает клавиатурные команды:
 *   - Page Up   → увеличить zValue выделенных элементов (на передний план);
 *   - Page Down → уменьшить zValue выделенных элементов (на задний план);
 *   - Delete/Backspace → испустить сигнал deletePressed().
 */
class ShapeView : public QGraphicsView {
    Q_OBJECT
public:
    explicit ShapeView(QWidget *parent = nullptr);

signals:
    /// Испускается при нажатии Delete или Backspace
    void deletePressed();

protected:
    void wheelEvent(QWheelEvent *event)  override;
    void keyPressEvent(QKeyEvent *event) override;
};
