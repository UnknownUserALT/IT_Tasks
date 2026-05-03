// StatisticsWidget — пользовательский виджет для отрисовки столбчатой диаграммы
// «Количество книг по годам» через QPainter (без Qt Charts).
// Поддерживает:
// — адаптивность к размеру окна (пересчёт при resize);
// — подсветку столбца при наведении мыши (setMouseTracking + mouseMoveEvent);
// — сетку и подписи осей;
// — использование цветов из текущей палитры (для корректной работы с тёмной темой).

#pragma once
#include <QWidget>
#include <QMap>

class StatisticsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatisticsWidget(QWidget *parent = nullptr);

    /// Устанавливает данные для диаграммы: {год -> количество книг}
    void setYearCounts(const QMap<int, int> &yearCounts);

protected:
    /// Отрисовка диаграммы — вызывается автоматически при update() и resize
    void paintEvent(QPaintEvent *event) override;

    /// Отслеживание позиции мыши для подсветки столбца
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QMap<int, int> m_data;     // Данные: год → количество книг
    int m_hoveredBar = -1;     // Индекс столбца под курсором (-1 = нет)
};
