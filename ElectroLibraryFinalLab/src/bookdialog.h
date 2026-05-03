// BookDialog — диалог добавления и редактирования книги.
// Переиспользуется в двух режимах:
// — Создание: пустые поля, заголовок «Новая книга» (по умолчанию из .ui)
// — Редактирование: вызов setBook() заполняет поля и меняет заголовок.
// Кнопка «Сохранить» заблокирована, пока не заполнены обязательные поля (title, author).
// При сохранении — валидация с QMessageBox; accept()/reject() для возврата результата.

#pragma once
#include <QDialog>
#include "book.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BookDialog; }
QT_END_NAMESPACE

class BookDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BookDialog(QWidget *parent = nullptr);
    ~BookDialog() override;

    /// Заполняет поля диалога данными книги (режим редактирования)
    void setBook(const Book &b);

    /// Возвращает книгу с данными из полей формы
    Book getBook() const;

private slots:
    /// Проверяет обязательные поля и включает/выключает кнопку «Сохранить»
    void validateFields();

    /// Финальная валидация при нажатии «Сохранить» (ISBN, обязательные поля)
    void onSaveClicked();

private:
    Ui::BookDialog *ui;
};
