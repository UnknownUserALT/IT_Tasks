/**
 * @file Phase2Widget.cpp
 * @brief Реализация второй фазы: "Убегающее окно"
 *
 * Окно убегает при нажатии крестика.
 * Разгадка: Ctrl+Q показывает скрытое поле → ввести "quit".
 */

#include "Phase2Widget.h"
#include <QVBoxLayout>
#include <QResizeEvent>

Phase2Widget::Phase2Widget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Phase2Widget::setupUI() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Текст-провокация в центре окна
    m_textLabel = new QLabel("Ладно, просто закрой окно", this);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setStyleSheet("font-size: 32px; font-weight: bold;");

    // Скрытое поле ввода (появляется по Ctrl+Q)
    m_secretInput = new QLineEdit(this);
    m_secretInput->setPlaceholderText("Введите magic word...");
    m_secretInput->hide();
    m_secretInput->setStyleSheet("font-size: 16px; padding: 5px;");

    // Сообщение об ошибке при неверном вводе
    m_errorLabel = new QLabel("", this);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setStyleSheet("color: red; font-size: 14px;");
    m_errorLabel->hide();

    // Скрываем ошибку при начале нового ввода
    connect(m_secretInput, &QLineEdit::textChanged, this, [this]() {
        if (m_errorLabel->isVisible()) {
            m_errorLabel->hide();
        }
    });

    // При нажатии Enter проверяем введённое слово
    connect(m_secretInput, &QLineEdit::returnPressed, this, [this]() {
        if (m_secretInput->text().toLower() == "quit") {
            emit phaseCompleted();  // Правильное слово → следующая фаза
        } else {
            m_secretInput->clear();
            m_errorLabel->setText("Не-а, попробуй ещё");
            m_errorLabel->show();
        }
    });

    // Еле заметная подсказка в правом нижнем углу
    // Цвет текста почти совпадает с фоном (#f0f0f0 на белом)
    m_hintLabel = new QLabel("Подсказка: Ctrl+Q", this);
    m_hintLabel->setStyleSheet(
        "color: #f0f0f0;"  // Почти белый на белом фоне
        "font-size: 10px;"
    );
    m_hintLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    mainLayout->addWidget(m_textLabel);
    mainLayout->addWidget(m_secretInput, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_errorLabel);

    setLayout(mainLayout);

    // Подсказка позиционируется абсолютно (в правый нижний угол)
    m_hintLabel->setParent(this);
}

/**
 * @brief Показывает скрытое поле ввода
 *
 * Вызывается из MainWindow при нажатии Ctrl+Q.
 * Поле получает фокус для немедленного ввода.
 */
void Phase2Widget::showSecretInput() {
    m_secretInput->show();
    m_secretInput->setFocus();
}

/**
 * @brief Обработчик изменения размера окна
 *
 * Перемещает подсказку в правый нижний угол при изменении размера.
 */
void Phase2Widget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (m_hintLabel) {
        m_hintLabel->move(width() - m_hintLabel->width() - 10,
                          height() - m_hintLabel->height() - 10);
    }
}
