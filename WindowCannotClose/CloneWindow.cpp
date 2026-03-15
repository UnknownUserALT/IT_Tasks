/**
 * @file CloneWindow.cpp
 * @brief Реализация окна-клона для третьей фазы
 *
 * Клоны появляются при закрытии крестиком.
 * Особое окно (зелёный фон) содержит кнопку победы.
 */

#include "CloneWindow.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QScreen>
#include <QGuiApplication>

// Инициализация статического флага победы
// Общий для всех клонов - предотвращает создание новых при победе
bool CloneWindow::m_victoryAchieved = false;

/**
 * @brief Конструктор окна-клона
 * @param isSpecial true для особого окна (с кнопкой победы и зелёным фоном)
 * @param parent Родительский виджет
 *
 * Окно позиционируется случайно на экране.
 */
CloneWindow::CloneWindow(bool isSpecial, QWidget *parent)
    : QWidget(parent), m_isSpecial(isSpecial) {

    setWindowFlags(Qt::Window);  // Независимое окно
    resize(300, 150);
    setupUI();

    // Случайная позиция на экране
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    int maxX = screenGeometry.width() - width();
    int maxY = screenGeometry.height() - height();

    int newX = QRandomGenerator::global()->bounded(screenGeometry.x(), maxX);
    int newY = QRandomGenerator::global()->bounded(screenGeometry.y(), maxY);

    move(newX, newY);
}

/**
 * @brief Настройка интерфейса окна-клона
 */
void CloneWindow::setupUI() {
    auto *layout = new QVBoxLayout(this);

    // Дразнящий текст (выбирается случайно)
    QStringList teasers = {
        "Не это окно!",
        "Попробуй другое!",
        "Ты не закроешь меня!",
        "Ха-ха-ха!",
        "Всё бесполезно!",
        "Закрывай — будет больше!",
        "Это не тот дроид...",
        "Ничего не выйдет!"
    };

    QString text = teasers[QRandomGenerator::global()->bounded(teasers.size())];
    m_label = new QLabel(text, this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setWordWrap(true);
    layout->addWidget(m_label);

    // Особое окно имеет кнопку победы и другой фон
    if (m_isSpecial) {
        m_button = new QPushButton("Сдаюсь, ты победил!", this);
        m_button->setStyleSheet(
            "QPushButton {"
            "  background-color: #4CAF50;"
            "  color: white;"
            "  font-size: 14px;"
            "  font-weight: bold;"
            "  padding: 8px;"
            "  border-radius: 4px;"
            "}"
        );

        // При нажатии кнопки - победа (без создания клонов!)
        connect(m_button, &QPushButton::clicked, this, [this]() {
            m_victoryAchieved = true;  // Блокируем создание клонов
            if (m_victoryCallback) {
                m_victoryCallback();
            }
            close();
        });

        layout->addWidget(m_button);

        // Особое окно имеет зеленоватый фон
        setStyleSheet("QWidget { background-color: #e8f5e9; }");
    } else {
        m_button = nullptr;
        setStyleSheet("QWidget { background-color: white; }");
    }

    setLayout(layout);
}

/**
 * @brief Устанавливает callback для создания новых клонов при закрытии
 * @param callback Функция, принимающая bool (особый ли новый клон)
 */
void CloneWindow::setSpawnCallback(std::function<void(bool)> callback) {
    m_spawnCallback = callback;
}

/**
 * @brief Устанавливает callback для победы
 * @param callback Функция, вызываемая при нажатии кнопки в особом окне
 */
void CloneWindow::setVictoryCallback(std::function<void()> callback) {
    m_victoryCallback = callback;
}

/**
 * @brief Сбрасывает флаг победы
 *
 * Вызывается при старте новой игры для сброса состояния.
 */
void CloneWindow::resetVictoryFlag() {
    m_victoryAchieved = false;
}

/**
 * @brief Обработчик закрытия окна
 *
 * Если победа достигнута - просто закрываемся.
 * Иначе создаём 2 новых клона:
 * - Обычное окно → 2 обычных клона
 * - Особое окно → 1 особый + 1 обычный
 */
void CloneWindow::closeEvent(QCloseEvent *event) {
    // Если победа достигнута - не создаём клоны, просто закрываемся
    if (m_victoryAchieved) {
        deleteLater();
        event->accept();
        return;
    }

    // При закрытии порождаем 2 новых окна
    if (m_spawnCallback) {
        if (m_isSpecial) {
            // Особое окно: один клон особый, один обычный
            m_spawnCallback(true);   // Особый
            m_spawnCallback(false);  // Обычный
        } else {
            // Обычное окно: оба клона обычные
            m_spawnCallback(false);
            m_spawnCallback(false);
        }
    }

    // Удаляем это окно
    deleteLater();
    event->accept();
}
