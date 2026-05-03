// BookFilterProxyModel — прокси-модель для фильтрации таблицы книг.
// Наследует QSortFilterProxyModel и переопределяет filterAcceptsRow
// для поиска одновременно по title и author, а также фильтрации по статусу.
// Устанавливается между QSqlTableModel и QTableView: view->setModel(proxy).

#pragma once
#include <QSortFilterProxyModel>

class BookFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit BookFilterProxyModel(QObject *parent = nullptr);

    /// Устанавливает фильтр по статусу ("to_read", "reading", "done" или "" для всех)
    void setStatusFilter(const QString &status);

    /// Устанавливает текст для поиска по названию и автору
    void setSearchText(const QString &text);

protected:
    /// Определяет, должна ли строка sourceRow быть видимой.
    /// Проверяет совпадение по статусу (колонка 6) и тексту (колонки 1, 2).
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_statusFilter;  // Текущий фильтр по статусу
    QString m_searchText;    // Текущий поисковый запрос
};
