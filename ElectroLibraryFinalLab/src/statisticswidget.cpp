// Реализация StatisticsWidget.
// Рисуем столбчатую диаграмму «с нуля» через QPainter.
// QPainter создаётся только внутри paintEvent (нельзя хранить как член класса).

#include "statisticswidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

StatisticsWidget::StatisticsWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(240);
    setMouseTracking(true);  // Получать mouseMoveEvent даже без нажатой кнопки
}

void StatisticsWidget::setYearCounts(const QMap<int, int> &yearCounts)
{
    m_data = yearCounts;
    update();  // Запланировать перерисовку (вызовет paintEvent)
}

void StatisticsWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_data.isEmpty()) return;

    // Отступы диаграммы от краёв виджета (совпадают с paintEvent)
    const int marginLeft   = 60;
    const int marginRight  = 20;
    const int marginTop    = 30;
    const int marginBottom = 40;
    const int chartW = width() - marginLeft - marginRight;
    const int n = m_data.size();
    if (n == 0) return;

    // Определяем, над каким столбцом находится курсор
    const double barW = static_cast<double>(chartW) / n;
    int mouseX = event->pos().x() - marginLeft;
    int idx = static_cast<int>(mouseX / barW);

    // Курсор вне области диаграммы — сбрасываем подсветку
    if (idx < 0 || idx >= n || event->pos().y() < marginTop || event->pos().y() > height() - marginBottom)
        idx = -1;

    // Перерисовываем только если подсвеченный столбец изменился
    if (idx != m_hoveredBar) {
        m_hoveredBar = idx;
        update();
    }
}

void StatisticsWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);  // Сглаживание (обязательно по ТЗ)
    p.fillRect(rect(), palette().base());      // Фон из палитры (работает с тёмной темой)

    // При пустых данных — надпись по центру
    if (m_data.isEmpty()) {
        p.drawText(rect(), Qt::AlignCenter, tr("Нет данных"));
        return;
    }

    // Отступы для области рисования диаграммы
    const int marginLeft   = 60;
    const int marginRight  = 20;
    const int marginTop    = 30;
    const int marginBottom = 40;

    const int chartW = width()  - marginLeft - marginRight;   // Ширина области диаграммы
    const int chartH = height() - marginTop  - marginBottom;  // Высота области диаграммы
    if (chartW <= 0 || chartH <= 0) return;

    const int n = m_data.size();
    const QList<int> years = m_data.keys();

    // Находим максимальное значение для масштабирования высоты столбцов
    int maxVal = 1;
    for (int v : m_data.values())
        maxVal = qMax(maxVal, v);

    // ── Горизонтальная сетка (4–5 делений) ──
    int divisions = 4;
    if (maxVal < 4) divisions = maxVal;
    p.setPen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));  // Светло-серая пунктирная линия
    QFont smallFont = p.font();
    smallFont.setPointSize(8);
    p.setFont(smallFont);

    for (int i = 0; i <= divisions; ++i) {
        int val = qRound(static_cast<double>(maxVal) * i / divisions);
        int y = marginTop + chartH - static_cast<int>(static_cast<double>(val) / maxVal * chartH);
        p.drawLine(marginLeft, y, marginLeft + chartW, y);

        // Подписи на левой оси (значения)
        p.setPen(palette().text().color());
        p.drawText(0, y - 8, marginLeft - 8, 16, Qt::AlignRight | Qt::AlignVCenter, QString::number(val));
        p.setPen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));
    }

    // ── Столбцы диаграммы ──
    const double barFullW = static_cast<double>(chartW) / n;  // Полная ширина ячейки (столбец + отступы)
    const double barW = barFullW * 0.7;                        // Ширина самого столбца
    const double gap  = barFullW * 0.15;                       // Отступ слева от столбца

    // Цвета из палитры (корректно работают со светлой/тёмной темой)
    QColor barColor = palette().highlight().color();
    QColor hoverColor = barColor.lighter(130);  // Подсвеченный столбец — светлее

    for (int i = 0; i < n; ++i) {
        int year = years[i];
        int val  = m_data[year];
        int barH = static_cast<int>(static_cast<double>(val) / maxVal * chartH);

        double x = marginLeft + i * barFullW + gap;
        int y = marginTop + chartH - barH;

        // Рисуем столбец со скруглёнными углами
        QRectF barRect(x, y, barW, barH);
        p.setPen(Qt::NoPen);
        p.setBrush(i == m_hoveredBar ? hoverColor : barColor);  // Подсветка при наведении
        p.drawRoundedRect(barRect, 3, 3);

        // Подпись сверху столбца (количество книг)
        p.setPen(palette().text().color());
        p.drawText(QRectF(x, y - 18, barW, 16), Qt::AlignCenter, QString::number(val));

        // Подпись снизу столбца (год)
        p.drawText(QRectF(x, marginTop + chartH + 4, barW, 20), Qt::AlignCenter, QString::number(year));
    }

    // ── Оси координат ──
    p.setPen(QPen(palette().text().color(), 2));
    p.drawLine(marginLeft, marginTop, marginLeft, marginTop + chartH);                 // Вертикальная ось (Y)
    p.drawLine(marginLeft, marginTop + chartH, marginLeft + chartW, marginTop + chartH); // Горизонтальная ось (X)
}
