/**
 * @file Phase2Widget.h
 * @brief Заголовочный файл второй фазы: "Убегающее окно"
 */

#ifndef WINDOWCANNOTCLOSE_PHASE2WIDGET_H
#define WINDOWCANNOTCLOSE_PHASE2WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QResizeEvent>

/**
 * @brief Виджет второй фазы игры
 *
 * Окно убегает при нажатии крестика.
 * Разгадка: Ctrl+Q показывает скрытое поле → ввести "quit".
 */
class Phase2Widget : public QWidget {
    Q_OBJECT
public:
    explicit Phase2Widget(QWidget *parent = nullptr);

    void showSecretInput();  ///< Показать скрытое поле ввода по Ctrl+Q

signals:
    void phaseCompleted();  ///< Сигнал завершения фазы

protected:
    void resizeEvent(QResizeEvent *event) override;  ///< Для позиционирования подсказки

private:
    QLabel *m_textLabel;       ///< Текст "Ладно, просто закрой окно"
    QLabel *m_hintLabel;       ///< Еле заметная подсказка "Ctrl+Q"
    QLabel *m_errorLabel;      ///< Сообщение об ошибке ввода
    QLineEdit *m_secretInput;  ///< Скрытое поле для ввода "quit"

    void setupUI();  ///< Настройка интерфейса
};

#endif //WINDOWCANNOTCLOSE_PHASE2WIDGET_H
