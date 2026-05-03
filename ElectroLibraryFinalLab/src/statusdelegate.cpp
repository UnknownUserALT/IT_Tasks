// Реализация StatusDelegate.
// Используется как setItemDelegateForColumn(6, new StatusDelegate(this)) в MainWindow.

#include "statusdelegate.h"
#include <QComboBox>

StatusDelegate::StatusDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *StatusDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                                      const QModelIndex &) const
{
    // Создаём комбобокс с тремя вариантами; data — внутреннее значение для БД
    auto *combo = new QComboBox(parent);
    combo->addItem(tr("К прочтению"), "to_read");
    combo->addItem(tr("Читаю"),       "reading");
    combo->addItem(tr("Прочитано"),    "done");
    return combo;
}

void StatusDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *combo = qobject_cast<QComboBox *>(editor);
    if (!combo) return;

    // Ищем текущее значение из модели в списке data комбобокса
    QString value = index.data(Qt::EditRole).toString();
    int idx = combo->findData(value);
    combo->setCurrentIndex(idx >= 0 ? idx : 0);
}

void StatusDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    auto *combo = qobject_cast<QComboBox *>(editor);
    if (!combo) return;

    // Записываем в модель внутреннее значение (to_read/reading/done), а не русский текст
    model->setData(index, combo->currentData().toString());
}

QString StatusDelegate::displayText(const QVariant &value, const QLocale &) const
{
    // Преобразование внутренних значений в русские для отображения в таблице
    QString status = value.toString();
    if (status == "to_read") return tr("К прочтению");
    if (status == "reading") return tr("Читаю");
    if (status == "done")    return tr("Прочитано");
    return status;
}
