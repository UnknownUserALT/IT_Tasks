// StatusDelegate — делегат для столбца «Статус» в QTableView.
// Решает две задачи:
// 1. displayText() — отображает русские названия вместо сырых to_read/reading/done
// 2. createEditor() — при редактировании показывает QComboBox вместо текстового поля,
//    чтобы пользователь не мог ввести произвольный (невалидный) статус.

#pragma once
#include <QStyledItemDelegate>

class StatusDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StatusDelegate(QObject *parent = nullptr);

    /// Создаёт QComboBox с тремя вариантами статуса
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    /// Устанавливает текущий статус в комбобоксе при открытии редактора
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    /// Записывает выбранный статус обратно в модель (внутреннее значение, не русское)
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    /// Преобразует внутреннее значение в русское для отображения в таблице
    QString displayText(const QVariant &value, const QLocale &locale) const override;
};
