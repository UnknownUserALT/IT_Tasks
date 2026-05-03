// Реализация BookFilterProxyModel.
// invalidateFilter() вызывается при изменении критериев — прокси пересчитывает видимые строки.

#include "bookfilterproxymodel.h"

BookFilterProxyModel::BookFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {}

void BookFilterProxyModel::setStatusFilter(const QString &status)
{
    m_statusFilter = status;
    invalidateFilter();  // Принудительно пересчитать, какие строки проходят фильтр
}

void BookFilterProxyModel::setSearchText(const QString &text)
{
    m_searchText = text;
    invalidateFilter();
}

bool BookFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    // Вспомогательная лямбда для получения индекса ячейки по номеру колонки
    auto idx = [&](int col) {
        return sourceModel()->index(sourceRow, col, sourceParent);
    };

    // Фильтр по статусу (колонка 6 = status)
    if (!m_statusFilter.isEmpty()) {
        QString status = sourceModel()->data(idx(6)).toString();
        if (status != m_statusFilter)
            return false;
    }

    // Поиск по тексту: ищем в названии (колонка 1) И в авторе (колонка 2)
    // Строка проходит, если текст найден хотя бы в одном из двух полей
    if (!m_searchText.isEmpty()) {
        QString title  = sourceModel()->data(idx(1)).toString();
        QString author = sourceModel()->data(idx(2)).toString();
        if (!title.contains(m_searchText, Qt::CaseInsensitive)
            && !author.contains(m_searchText, Qt::CaseInsensitive))
            return false;
    }

    return true;
}
