/**
 * @file Phase3Widget.h
 * @brief Заголовочный файл третьей фазы: "Размножение"
 */

#ifndef WINDOWCANNOTCLOSE_PHASE3WIDGET_H
#define WINDOWCANNOTCLOSE_PHASE3WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QList>

class CloneWindow;

/**
 * @brief Виджет третьей фазы игры
 *
 * Создаёт 5 окон-клонов при старте.
 * При закрытии окна крестиком появляются 2 новых.
 * Особое окно (зелёный фон) содержит кнопку победы.
 */
class Phase3Widget : public QWidget {
    Q_OBJECT
public:
    explicit Phase3Widget(QWidget *parent = nullptr);
    ~Phase3Widget();

    void startPhase();  ///< Запускает фазу - создаёт 5 окон

signals:
    void phaseCompleted();  ///< Сигнал завершения фазы

private:
    QLabel *m_label;                ///< Текст в главном окне
    QList<CloneWindow*> m_clones;   ///< Список всех клонов

    void spawnClone(bool isSpecial = false);  ///< Создаёт новый клон
    void onVictory();                        ///< Обработчик победы
    void cleanup();                           ///< Закрывает все клоны
};

#endif //WINDOWCANNOTCLOSE_PHASE3WIDGET_H
