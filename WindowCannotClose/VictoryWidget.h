#ifndef WINDOWCANNOTCLOSE_VICTORYWIDGET_H
#define WINDOWCANNOTCLOSE_VICTORYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class VictoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit VictoryWidget(QWidget *parent = nullptr);

signals:
    void exitRequested();

private:
    QLabel *m_label;
    QPushButton *m_exitButton;

    void setupUI();
};

#endif //WINDOWCANNOTCLOSE_VICTORYWIDGET_H
