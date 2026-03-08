#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QProgressBar>
#include <QPushButton>
#include <QLCDNumber>
#include <QTimer>
#include <QTextEdit>
#include <QTime>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Пульт управления резервуаром");
    window.resize(600, 650);
    // === Создаём виджеты ===
    QLabel *titleLabel = new QLabel("РЕЗЕРВУАР A-1");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    QLabel *statusLabel = new QLabel("Статус: НОРМА");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(
        "font-size: 14px; padding: 6px; background-color: #27ae60; color: white; border-radius: 4px;");

    QLabel *sliderLabel = new QLabel("Клапан подачи:");
    QSlider *inputSlider = new QSlider(Qt::Horizontal);
    inputSlider->setRange(0, 100);
    inputSlider->setValue(0);
    QLabel *drainSliderLabel = new QLabel("Клапан сброса:");
    QSlider *drainSlider = new QSlider(Qt::Horizontal);
    drainSlider->setRange(0, 100);
    drainSlider->setValue(0);
    QLabel *levelLabel = new QLabel("Уровень:");
    QProgressBar *levelBar = new QProgressBar();
    levelBar->setRange(0, 100);
    levelBar->setValue(0);
    QLCDNumber *lcdDisplay = new QLCDNumber();
    lcdDisplay->setDigitCount(4);
    lcdDisplay->setMinimumHeight(60);
    QPushButton *drainBtn = new QPushButton("Сброс (слив)");
    QPushButton *resetBtn = new QPushButton("Аварийный сброс");
    resetBtn->setStyleSheet(
        "background-color: #e74c3c; color: white; font-weight: bold; padding: 8px;");

    QTextEdit *logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    logTextEdit->setMaximumHeight(150);
    logTextEdit->setStyleSheet("font-family: monospace; font-size: 11px;");

    // === Компоновка ===
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleLabel);
    QHBoxLayout *sliderRow = new QHBoxLayout();
    sliderRow->addWidget(sliderLabel);
    sliderRow->addWidget(inputSlider);
    mainLayout->addLayout(sliderRow);
    QHBoxLayout *drainSliderRow = new QHBoxLayout();
    drainSliderRow->addWidget(drainSliderLabel);
    drainSliderRow->addWidget(drainSlider);
    mainLayout->addLayout(drainSliderRow);
    QHBoxLayout *levelRow = new QHBoxLayout();
    levelRow->addWidget(levelLabel);
    levelRow->addWidget(levelBar);
    mainLayout->addLayout(levelRow);
    mainLayout->addWidget(lcdDisplay);
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addWidget(drainBtn);
    btnRow->addWidget(resetBtn);
    mainLayout->addLayout(btnRow);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(logTextEdit);

    window.setLayout(mainLayout);


    //Связываем сгиналы и слоты
    double currentLevel = 0.0;
    bool reached80 = false;
    bool reached90 = false;

    // Лямбда-функция для добавления записи в журнал
    auto addLog = [&](const QString &message) {
        QString timestamp = QTime::currentTime().toString("hh:mm:ss");
        logTextEdit->append(QString("[%1] %2").arg(timestamp, message));
    };

    addLog("Система запущена");
    QTimer *simTimer = new QTimer(&window);
    simTimer->setInterval(100); // каждые 100 мс
    QObject::connect(simTimer, &QTimer::timeout, [&]() {
        double inflow = inputSlider->value() * 0.2;
        double leak = 10.0;
        double drain = drainSlider->value() * 0.15;
        currentLevel += (inflow - leak - drain) * 0.1;
        if (currentLevel < 0) currentLevel = 0;
        if (currentLevel > 100) currentLevel = 100;
        levelBar->setValue(static_cast<int>(currentLevel));
        if (currentLevel > 90) {
            statusLabel->setText("!!! КРИТИЧЕСКИЙ УРОВЕНЬ !!!");
            statusLabel->setStyleSheet(
                "font-size: 14px; padding: 6px; background-color: #e74c3c; color: white; font-weight: bold; border-radius: 4px;");
            levelBar->setStyleSheet(
                "QProgressBar::chunk { background-color: #e74c3c; }");
        } else if (currentLevel > 80) {
            statusLabel->setText("ВНИМАНИЕ: высокий уровень");
            statusLabel->setStyleSheet(
                "font-size: 14px; padding: 6px; background-color: #f39c12; color: white; border-radius: 4px;");
            levelBar->setStyleSheet(
                "QProgressBar::chunk { background-color: #f39c12; }");
        } else {
            statusLabel->setText("Статус: НОРМА");
            statusLabel->setStyleSheet(
                "font-size: 14px; padding: 6px; background-color: #27ae60; color: white; border-radius: 4px;");
            levelBar->setStyleSheet("");
        }
        lcdDisplay->display(static_cast<int>(currentLevel));

        // Логирование достижения порогов
        if (currentLevel >= 80 && !reached80) {
            addLog("Уровень достиг 80%");
            reached80 = true;
        }
        if (currentLevel >= 90 && !reached90) {
            addLog("!!! Уровень достиг 90% - КРИТИЧЕСКИЙ !!!");
            reached90 = true;
        }
        if (currentLevel < 80) {
            reached80 = false;
        }
        if (currentLevel < 90) {
            reached90 = false;
        }
    });
    simTimer->start();

    QObject::connect(drainBtn, &QPushButton::clicked, [=]() {
        inputSlider->setValue(0);
        drainSlider->setValue(0);
        addLog("Оператор нажал: Сброс (слив)");
    });
    QObject::connect(resetBtn, &QPushButton::clicked, [=]() {
        inputSlider->setValue(0);
        drainSlider->setValue(0);
        levelBar->setValue(0);
        lcdDisplay->display(0);
        addLog("!!! АВАРИЙНЫЙ СБРОС - резервуар опустошен !!!");
    });
    window.show();
    return app.exec();
}
