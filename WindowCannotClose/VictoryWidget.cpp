#include "VictoryWidget.h"

VictoryWidget::VictoryWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void VictoryWidget::setupUI() {
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    m_label = new QLabel("ПОБЕДА!\n\nТы прошёл все фазы!", this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet("font-size: 32px; font-weight: bold; color: green;");
    m_label->setWordWrap(true);

    m_exitButton = new QPushButton("ВЫХОД", this);
    m_exitButton->setFixedSize(200, 60);
    m_exitButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "}"
    );

    connect(m_exitButton, &QPushButton::clicked, this, &VictoryWidget::exitRequested);

    layout->addWidget(m_label);
    layout->addWidget(m_exitButton, 0, Qt::AlignCenter);

    setLayout(layout);
}
