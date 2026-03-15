/**
 * @file Phase4Widget.h
 * @brief Заголовочный файл четвёртой фазы: "Финальный босс"
 */

#ifndef WINDOWCANNOTCLOSE_PHASE4WIDGET_H
#define WINDOWCANNOTCLOSE_PHASE4WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QStringList>
#include <QKeyEvent>
#include <QShowEvent>

/**
 * @brief Виджет четвёртой (финальной) фазы игры
 *
 * Окно "сходит с ума": скачет, меняет размер, заголовок мигает.
 * Разгадка: нажать Пробел для остановки и победы.
 */
class Phase4Widget : public QWidget {
    Q_OBJECT
public:
    explicit Phase4Widget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;   ///< Для отлова Пробела
    void showEvent(QShowEvent *event) override;      ///< Для автозапуска таймеров

private:
    QLabel *m_messageLabel;       ///< Текст сообщения / победы
    QPushButton *m_exitButton;    ///< Кнопка выхода
    QTimer *m_jumpTimer;          ///< Таймер скачков окна
    QTimer *m_resizeTimer;        ///< Таймер изменения размера
    QTimer *m_titleTimer;         ///< Таймер мигания заголовка

    int m_titleIndex;             ///< Текущий индекс символа в заголовке
    QStringList m_titleChars;     ///< Символы для мигания заголовка

    void setupUI();               ///< Настройка интерфейса
    void startCrazyMode();        ///< Запустить "сумасшедший" режим
    void stopCrazyMode();         ///< Остановить все таймеры
    void showVictory();           ///< Показать победный экран

private slots:
    void jumpWindow();    ///< Переместить окно случайно
    void resizeWindow();  ///< Изменить размер случайно
    void flashTitle();    ///< Мигнуть заголовком
};

#endif //WINDOWCANNOTCLOSE_PHASE4WIDGET_H
