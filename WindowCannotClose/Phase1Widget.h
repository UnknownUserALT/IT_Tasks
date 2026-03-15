/**
 * @file Phase1Widget.h
 * @brief Заголовочный файл первой фазы: "Убегающая кнопка"
 */

#ifndef WINDOWCANNOTCLOSE_PHASE1WIDGET_H
#define WINDOWCANNOTCLOSE_PHASE1WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

/**
 * @brief Виджет первой фазы игры
 *
 * Кнопка "ВЫХОД" убегает от курсора мыши.
 * Разгадка: Tab для фокуса + Enter для нажатия.
 */
class Phase1Widget : public QWidget {
    Q_OBJECT
public:
    explicit Phase1Widget(QWidget *parent = nullptr);

signals:
    void phaseCompleted();  ///< Сигнал завершения фазы

private:
    QPushButton *m_exitButton;       ///< Кнопка "ВЫХОД" (убегает от мыши)
    QLabel *m_instructionLabel;     ///< Текст инструкции
    QLabel *m_hintLabel;            ///< Подсказка про клавиатуру

    void setupUI();                 ///< Настройка интерфейса
};

#endif //WINDOWCANNOTCLOSE_PHASE1WIDGET_H
