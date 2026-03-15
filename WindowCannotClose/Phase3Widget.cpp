/**
 * @file Phase3Widget.cpp
 * @brief Реализация третьей фазы: "Размножение"
 *
 * При входе создаётся 5 окон-клонов. При закрытии крестиком появляются 2 новых.
 * Особое окно (зелёный фон) содержит кнопку победы.
 */

#include "Phase3Widget.h"
#include "CloneWindow.h"
#include <QVBoxLayout>
#include <QRandomGenerator>

Phase3Widget::Phase3Widget(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);

    // Текст в главном окне
    m_label = new QLabel("Ты думал, это конец?\n\nПопробуй найти правильное окно...", this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet("font-size: 24px; font-weight: bold;");
    m_label->setWordWrap(true);

    layout->addWidget(m_label);
    setLayout(layout);
}

Phase3Widget::~Phase3Widget() {
    cleanup();
}

/**
 * @brief Запускает третью фазу
 *
 * Создаёт 5 начальных окон, одно из которых - особое.
 * Также сбрасывает флаг победы на случай повторного прохождения.
 */
void Phase3Widget::startPhase() {
    // Сбрасываем флаг победы для новой игры
    CloneWindow::resetVictoryFlag();

    // Создаём 5 начальных окон
    for (int i = 0; i < 5; ++i) {
        bool isSpecial = (i == 2);  // Третье окно - особенное (зелёное)
        spawnClone(isSpecial);
    }
}

/**
 * @brief Создаёт новое окно-клон
 * @param isSpecial true для особого окна (с кнопкой победы)
 *
 * Окно позиционируется случайно. При закрытии будет создавать новые клоны.
 * Максимум 20 окон одновременно для защиты от переполнения.
 */
void Phase3Widget::spawnClone(bool isSpecial) {
    // Максимум 20 окон одновременно (защита от зависания)
    if (m_clones.size() >= 20) {
        return;
    }

    auto *clone = new CloneWindow(isSpecial, this);

    // Callback для размножения при закрытии окна крестиком
    clone->setSpawnCallback([this](bool makeSpecial) {
        spawnClone(makeSpecial);  // Создаём новый клон
    });

    // Callback для победы при нажатии кнопки в особом окне
    clone->setVictoryCallback([this]() {
        onVictory();
    });

    m_clones.append(clone);
    clone->show();
}

/**
 * @brief Обработчик победы в третьей фазе
 *
 * Закрывает все окна-клоны и переходит к следующей фазе.
 */
void Phase3Widget::onVictory() {
    cleanup();
    emit phaseCompleted();
}

/**
 * @brief Закрывает все окна-клоны
 *
 * Вызывается при победе или в деструкторе.
 * Окна удаляются через deleteLater().
 */
void Phase3Widget::cleanup() {
    // Закрываем все клоны
    for (auto *clone : m_clones) {
        if (clone) {
            clone->close();
        }
    }
    m_clones.clear();
}
