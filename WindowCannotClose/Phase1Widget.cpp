/**
 * @file Phase1Widget.cpp
 * @brief Реализация первой фазы: "Убегающая кнопка"
 *
 * Игрок должен нажать кнопку "ВЫХОД", но она убегает от курсора мыши.
 * Разгадка: использовать Tab для фокуса + Enter для нажатия.
 */

#include "Phase1Widget.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QEvent>

/**
 * @brief Класс убегающей кнопки
 *
 * Кнопка перемещается в случайную позицию при наведении курсора мыши.
 * Убегает только от мыши, не от клавиатурного фокуса.
 */
class RunningButton : public QPushButton {
public:
    explicit RunningButton(QWidget *parent = nullptr) : QPushButton(parent) {}

protected:
    /**
     * @brief Обработчик наведения курсора на кнопку
     *
     * При наведении кнопка перемещается в случайную позицию внутри окна.
     * Новая позиция гарантированно находится на расстоянии минимум 100px
     * от текущей позиции, чтобы перемещение было заметным.
     */
    void enterEvent(QEnterEvent *event) override {
        Q_UNUSED(event)
        // Убегаем при наведении мыши
        if (parentWidget()) {
            int maxWidth = parentWidget()->width() - width();
            int maxHeight = parentWidget()->height() - height();

            int currentX = x();
            int currentY = y();

            int newX, newY;
            int attempts = 0;
            const int minDistance = 100;

            // Генерируем новую позицию пока не найдём на расстоянии >= 100px
            do {
                newX = QRandomGenerator::global()->bounded(maxWidth);
                newY = QRandomGenerator::global()->bounded(maxHeight);
                attempts++;
            } while (attempts < 100 && qAbs(newX - currentX) < minDistance && qAbs(newY - currentY) < minDistance);

            move(newX, newY);
        }
    }
};

Phase1Widget::Phase1Widget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Phase1Widget::setupUI() {
    // Инструкция в центре окна
    m_instructionLabel = new QLabel("Нажми ВЫХОД, чтобы закрыть", this);
    m_instructionLabel->setAlignment(Qt::AlignCenter);
    m_instructionLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    // Кнопка ВЫХОД с ярким стилем
    m_exitButton = new RunningButton(this);
    m_exitButton->setText("ВЫХОД");
    m_exitButton->setFixedSize(150, 60);
    m_exitButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #d32f2f;"      // Красный фон
        "  color: white;"                   // Белый текст
        "  font-size: 20px;"                // Крупный шрифт
        "  font-weight: bold;"
        "  border: 3px solid #b71c1c;"
        "  border-radius: 8px;"
        "}"
        "QPushButton:focus {"               // Жёлтая рамка при фокусе
        "  background-color: #f44336;"
        "  border: 3px solid #ffff00;"
        "}"
    );
    m_exitButton->setFocusPolicy(Qt::StrongFocus);  // Кнопка получает фокус от Tab

    // При нажатии кнопки переходим к следующей фазе
    connect(m_exitButton, &QPushButton::clicked, this, &Phase1Widget::phaseCompleted);

    // Подсказка внизу экрана (еле заметная)
    m_hintLabel = new QLabel("Подсказка: мышка — не единственный способ управления...", this);
    m_hintLabel->setAlignment(Qt::AlignCenter);
    m_hintLabel->setStyleSheet("color: gray; font-size: 12px;");

    // Абсолютное позиционирование кнопки (не в лейауте!)
    m_exitButton->move(325, 250);

    // Лейаут только для лейблов (инструкции и подсказки)
    auto *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(m_instructionLabel);
    layout->addStretch();
    layout->addWidget(m_hintLabel);
    layout->setContentsMargins(20, 20, 20, 40);
}
