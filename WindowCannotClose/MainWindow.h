/**
 * @file MainWindow.h
 * @brief Заголовочный файл главного окна игры
 */

#ifndef WINDOWCANNOTCLOSE_MAINWINDOW_H
#define WINDOWCANNOTCLOSE_MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QKeyEvent>
#include <QCloseEvent>

class Phase2Widget;
class Phase3Widget;
class Phase4Widget;

/**
 * @brief Главное окно игры-головоломки
 *
 * Содержит QStackedWidget с 4 фазами.
 * Переопределяет closeEvent и keyPressEvent для механики игры.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;   ///< Закрытие окна (разное для каждой фазы)
    void keyPressEvent(QKeyEvent *event) override;  ///< Обработка клавиш (Ctrl+Q)

private:
    QStackedWidget *m_stack;    ///< Контейнер для переключения фаз
    int m_currentPhase;         ///< Текущий номер фазы (0-3)
    Phase2Widget *m_phase2;     ///< Указатель на фазу 2 (для showSecretInput)
    Phase3Widget *m_phase3;     ///< Указатель на фазу 3 (для startPhase)

    void setupUI();         ///< Настройка интерфейса
    void goToNextPhase();   ///< Переход к следующей фазе
};

#endif //WINDOWCANNOTCLOSE_MAINWINDOW_H
