/**
 * @file MainWindow.cpp
 * @brief Реализация главного окна игры-головоломки
 *
 * Главное окно содержит QStackedWidget с 4 фазами.
 * Каждая фаза - отдельный виджет с уникальной механикой сопротивления закрытию.
 */

#include "MainWindow.h"
#include "Phase1Widget.h"
#include "Phase2Widget.h"
#include "Phase3Widget.h"
#include "Phase4Widget.h"
#include <QCloseEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QRandomGenerator>
#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_currentPhase(0), m_phase2(nullptr), m_phase3(nullptr) {
    setupUI();
    setMinimumSize(800, 600);
    resize(800, 600);
}

void MainWindow::setupUI() {
    m_stack = new QStackedWidget(this);

    // Фаза 1: Убегающая кнопка
    auto *phase1 = new Phase1Widget(this);
    m_stack->addWidget(phase1);
    connect(phase1, &Phase1Widget::phaseCompleted, this, &MainWindow::goToNextPhase);

    // Фаза 2: Убегающее окно
    m_phase2 = new Phase2Widget(this);
    m_stack->addWidget(m_phase2);
    connect(m_phase2, &Phase2Widget::phaseCompleted, this, &MainWindow::goToNextPhase);

    // Фаза 3: Размножение окон
    m_phase3 = new Phase3Widget(this);
    m_stack->addWidget(m_phase3);
    connect(m_phase3, &Phase3Widget::phaseCompleted, this, &MainWindow::goToNextPhase);

    // Фаза 4: Финальный босс
    auto *phase4 = new Phase4Widget(this);
    m_stack->addWidget(phase4);

    setCentralWidget(m_stack);
}

void MainWindow::goToNextPhase() {
    m_currentPhase++;
    m_stack->setCurrentIndex(m_currentPhase);

    // Если перешли на фазу 3 - запускаем размножение окон
    if (m_currentPhase == 2 && m_phase3) {
        m_phase3->startPhase();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Фаза 4 (после нажатия пробела): разрешаем закрытие приложения
    if (m_currentPhase == 3) {
        event->accept();
        return;
    }

    // Фаза 2: окно убегает от закрытия в случайную точку экрана
    if (m_currentPhase == 1) {
        event->ignore();

        // Получаем размеры экрана для расчёта новой позиции
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();

        int maxX = screenGeometry.width() - width();
        int maxY = screenGeometry.height() - height();

        int newX = QRandomGenerator::global()->bounded(screenGeometry.x(), maxX);
        int newY = QRandomGenerator::global()->bounded(screenGeometry.y(), maxY);

        move(newX, newY);
        return;
    }

    // Фазы 1 и 3: просто игнорируем закрытие
    event->ignore();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Ctrl+Q (или Cmd+Q на Mac) для фазы 2 - показать скрытое поле ввода
    if (event->key() == Qt::Key_Q) {
        if (event->modifiers() & Qt::ControlModifier ||
            event->modifiers() & Qt::MetaModifier) {  // Mac: Cmd работает как Ctrl
            if (m_currentPhase == 1 && m_phase2) {
                m_phase2->showSecretInput();
            }
            return;
        }
    }

    QMainWindow::keyPressEvent(event);
}
