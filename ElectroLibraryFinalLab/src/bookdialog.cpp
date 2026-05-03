// Реализация BookDialog.
// Форма описана в bookdialog.ui (Qt Designer); логика — здесь.

#include "bookdialog.h"
#include "ui_bookdialog.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QDate>

BookDialog::BookDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BookDialog)
{
    ui->setupUi(this);

    // Максимальный год — текущий (динамически, не захардкожен)
    ui->spinYear->setMaximum(QDate::currentDate().year());

    // ISBN-валидатор: только цифры и дефисы, начинается и заканчивается цифрой
    // Точная проверка количества цифр (10 или 13) — в onSaveClicked()
    auto *isbnValidator = new QRegularExpressionValidator(
        QRegularExpression(R"(\d[\d\-]{8,16}\d)"), this);
    ui->editIsbn->setValidator(isbnValidator);

    // Предзаполненные жанры (8–10 штук), editable=true позволяет ввести свой
    ui->comboGenre->addItems({
        tr("Художественная литература"), tr("Научная фантастика"),
        tr("Фэнтези"), tr("Детектив"), tr("Научная литература"),
        tr("Техническая литература"), tr("Программирование"),
        tr("История"), tr("Биография"), tr("Философия")
    });
    ui->comboGenre->setCurrentIndex(-1);  // Ничего не выбрано по умолчанию

    // Статусы: отображаемый текст на русском, data — внутреннее значение для БД
    ui->comboStatus->addItem(tr("К прочтению"), "to_read");
    ui->comboStatus->addItem(tr("Читаю"),       "reading");
    ui->comboStatus->addItem(tr("Прочитано"),    "done");

    // Валидация: кнопка «Сохранить» разблокируется, только когда заполнены title и author
    connect(ui->editTitle, &QLineEdit::textChanged, this, &BookDialog::validateFields);
    connect(ui->editAuthor, &QLineEdit::textChanged, this, &BookDialog::validateFields);

    // Кнопки: «Сохранить» → валидация + accept(), «Отмена» → reject()
    connect(ui->btnSave, &QPushButton::clicked, this, &BookDialog::onSaveClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

BookDialog::~BookDialog()
{
    delete ui;
}

void BookDialog::validateFields()
{
    // Кнопка активна только при заполненных обязательных полях
    bool ok = !ui->editTitle->text().trimmed().isEmpty()
           && !ui->editAuthor->text().trimmed().isEmpty();
    ui->btnSave->setEnabled(ok);
}

void BookDialog::onSaveClicked()
{
    // Финальная валидация перед accept()
    if (ui->editTitle->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка валидации"),
                             tr("Название книги не может быть пустым."));
        return;
    }
    if (ui->editAuthor->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка валидации"),
                             tr("Автор книги не может быть пустым."));
        return;
    }

    // Проверка ISBN: если указан, должен содержать ровно 10 или 13 цифр
    QString isbn = ui->editIsbn->text().trimmed();
    if (!isbn.isEmpty()) {
        QString digitsOnly = isbn;
        digitsOnly.remove('-');
        int digitCount = digitsOnly.length();
        if (digitCount != 10 && digitCount != 13) {
            QMessageBox::warning(this, tr("Ошибка валидации"),
                                 tr("ISBN должен содержать 10 или 13 цифр (сейчас: %1).").arg(digitCount));
            return;
        }
    }
    accept();  // Закрывает диалог с результатом QDialog::Accepted
}

void BookDialog::setBook(const Book &b)
{
    // Переключение в режим редактирования
    setWindowTitle(tr("Редактирование книги"));
    ui->editTitle->setText(b.title);
    ui->editAuthor->setText(b.author);
    ui->spinYear->setValue(b.year > 0 ? b.year : QDate::currentDate().year());
    ui->editIsbn->setText(b.isbn);

    // Ищем жанр в списке; если нет — устанавливаем как свободный текст
    int gIdx = ui->comboGenre->findText(b.genre);
    if (gIdx >= 0)
        ui->comboGenre->setCurrentIndex(gIdx);
    else
        ui->comboGenre->setEditText(b.genre);

    // Устанавливаем статус по внутреннему значению (data, а не текст)
    int sIdx = ui->comboStatus->findData(b.status);
    if (sIdx >= 0)
        ui->comboStatus->setCurrentIndex(sIdx);

    ui->spinRating->setValue(b.rating);
    ui->spinPages->setValue(b.pages);
    ui->editNotes->setPlainText(b.notes);
}

Book BookDialog::getBook() const
{
    // Собираем структуру Book из полей формы
    Book b;
    b.title  = ui->editTitle->text().trimmed();
    b.author = ui->editAuthor->text().trimmed();
    b.year   = ui->spinYear->value();
    b.isbn   = ui->editIsbn->text().trimmed();
    b.genre  = ui->comboGenre->currentText().trimmed();
    b.status = ui->comboStatus->currentData().toString();  // Внутреннее значение, не русский текст
    b.rating = ui->spinRating->value();
    b.pages  = ui->spinPages->value();
    b.notes  = ui->editNotes->toPlainText().trimmed();
    return b;
}
