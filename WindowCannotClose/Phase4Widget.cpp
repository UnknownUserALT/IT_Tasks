/**
 * @file Phase4Widget.cpp
 * @brief Реализация четвёртой фазы: "Финальный босс"
 *
 * Окно "сходит с ума": скачет, меняет размер, заголовок мигает.
 * Разгадка: нажать Пробел для остановки и победы.
 */

#include "Phase4Widget.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

/**
 * @brief Конструктор финальной фазы
 *
 * Инициализирует таймеры для "сумасшедшего" режима.
 */
Phase4Widget::Phase4Widget(QWidget *parent)
    : QWidget(parent), m_titleIndex(0) {

    // Заголовок последовательно показывает: Н А Ж М И   П Р О Б Е Л
    m_titleChars = QStringList() << "Н" << "А" << "Ж" << "М" << "И" << " " << "П" << "Р" << "О" << "Б" << "Е" << "Л";

    // Таймеры для эффектов
    m_jumpTimer = new QTimer(this);
    m_resizeTimer = new QTimer(this);
    m_titleTimer = new QTimer(this);

    connect(m_jumpTimer, &QTimer::timeout, this, &Phase4Widget::jumpWindow);
    connect(m_resizeTimer, &QTimer::timeout, this, &Phase4Widget::resizeWindow);
    connect(m_titleTimer, &QTimer::timeout, this, &Phase4Widget::flashTitle);

    setupUI();
}

/**
 * @brief Настройка интерфейса финальной фазы
 */
void Phase4Widget::setupUI() {
    auto *layout = new QVBoxLayout(this);

    // Сообщение "Финальный босс"
    m_messageLabel = new QLabel("☠️ ФИНАЛЬНЫЙ БОСС ☠️\n\nПопробуй меня поймать!", this);
    m_messageLabel->setAlignment(Qt::AlignCenter);
    m_messageLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: darkred;");
    m_messageLabel->setWordWrap(true);

    // Кнопка выхода (появляется после нажатия Пробела)
    m_exitButton = new QPushButton("Закрыть по-настоящему", this);
    m_exitButton->setFixedSize(250, 60);
    m_exitButton->hide();
    m_exitButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  border-radius: 8px;"
        "}"
    );

    // При нажатии - завершаем приложение
    connect(m_exitButton, &QPushButton::clicked, this, []() {
        qApp->quit();
    });

    layout->addWidget(m_messageLabel);
    layout->addWidget(m_exitButton, 0, Qt::AlignCenter);
    setLayout(layout);
}

/**
 * @brief Обработчик показа виджета
 *
 * Автоматически запускает "сумасшедший" режим при отображении.
 */
void Phase4Widget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    startCrazyMode();
}

/**
 * @brief Запускает "сумасшедший" режим
 *
 * - Скачки окна: каждые 300-500 мс
 * - Изменение размера: каждые 500 мс
 * - Мигание заголовка: каждые 200 мс
 */
void Phase4Widget::startCrazyMode() {
    // Скачки окна каждые 300-500 мс
    int jumpInterval = QRandomGenerator::global()->bounded(300, 501);
    m_jumpTimer->start(jumpInterval);

    // Изменение размера каждые 500 мс
    m_resizeTimer->start(500);

    // Мигание заголовка каждые 200 мс
    m_titleTimer->start(200);
}

/**
 * @brief Останавливает все таймеры
 */
void Phase4Widget::stopCrazyMode() {
    m_jumpTimer->stop();
    m_resizeTimer->stop();
    m_titleTimer->stop();
}

/**
 * @brief Перемещает окно в случайную точку экрана
 */
void Phase4Widget::jumpWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    int maxX = screenGeometry.width() - width();
    int maxY = screenGeometry.height() - height();

    int newX = QRandomGenerator::global()->bounded(screenGeometry.x(), maxX);
    int newY = QRandomGenerator::global()->bounded(screenGeometry.y(), maxY);

    window()->move(newX, newY);
}

/**
 * @brief Изменяет размер окна случайным образом
 *
 * Размеры в диапазоне: ширина 200-800, высота 150-600
 */
void Phase4Widget::resizeWindow() {
    int newWidth = QRandomGenerator::global()->bounded(200, 801);
    int newHeight = QRandomGenerator::global()->bounded(150, 601);
    window()->resize(newWidth, newHeight);
}

/**
 * @brief Мигает заголовком окна
 *
 * Последовательно показывает символы: Н А Ж М И   П Р О Б Е Л
 */
void Phase4Widget::flashTitle() {
    QString title = m_titleChars[m_titleIndex];
    window()->setWindowTitle(title);

    m_titleIndex = (m_titleIndex + 1) % m_titleChars.size();
}

/**
 * @brief Показывает победный экран
 *
 * Останавливает таймеры, возвращает нормальный размер,
 * показывает поздравление и кнопку выхода.
 */
void Phase4Widget::showVictory() {
    stopCrazyMode();

    // Возвращаем нормальный размер и позицию
    window()->resize(800, 600);
    window()->setWindowTitle("Победа!");

    // Показываем поздравление
    m_messageLabel->setText("🎉 ПОБЕДА! 🎉\n\nТы прошёл все фазы!\n🏆 Ты настоящий чемпион! 🏆");
    m_messageLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: green;");

    // Показываем кнопку выхода
    m_exitButton->show();
}

/**
 * @brief Обработчик нажатия клавиш
 *
 * Пробел останавливает "сумасшедший" режим и показывает победу.
 */
void Phase4Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        showVictory();
        return;
    }

    QWidget::keyPressEvent(event);
}
