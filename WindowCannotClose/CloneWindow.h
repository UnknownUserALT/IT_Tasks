/**
 * @file CloneWindow.h
 * @brief Заголовочный файл окна-клона для третьей фазы
 */

#ifndef WINDOWCANNOTCLOSE_CLONEWINDOW_H
#define WINDOWCANNOTCLOSE_CLONEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>
#include <functional>

/**
 * @brief Окно-клон для третьей фазы "Размножение"
 *
 * При закрытии создаёт 2 новых клона.
 * Особое окно (зелёный фон) содержит кнопку победы.
 */
class CloneWindow : public QWidget {
    Q_OBJECT
public:
    explicit CloneWindow(bool isSpecial, QWidget *parent = nullptr);

    void setSpawnCallback(std::function<void(bool isSpecial)> callback);  ///< Callback создания клонов
    void setVictoryCallback(std::function<void()> callback);               ///< Callback победы
    static void resetVictoryFlag();                                         ///< Сброс флага победы

protected:
    void closeEvent(QCloseEvent *event) override;  ///< Закрытие создаёт клоны

private:
    QLabel *m_label;                        ///< Дразнящий текст
    QPushButton *m_button;                  ///< Кнопка победы (только в особом окне)
    bool m_isSpecial;                       ///< Является ли окно особым
    std::function<void(bool)> m_spawnCallback;   ///< Callback для создания клонов
    std::function<void()> m_victoryCallback;     ///< Callback для победы
    static bool m_victoryAchieved;          ///< Общий флаг победы для всех клонов

    void setupUI();                         ///< Настройка интерфейса
};

#endif //WINDOWCANNOTCLOSE_CLONEWINDOW_H
