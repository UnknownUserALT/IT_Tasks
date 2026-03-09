// mainwindow.cpp — минимальный каркас
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QDate>
#include <QMessageBox>
#include <QFormLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    setWindowTitle("Финансовый трекер");
    resize(750, 550);
    setupUI();
}

void MainWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // === Группа: Новый расход ===
    QGroupBox *inputGroup = new QGroupBox("Новый расход");
    QHBoxLayout *inputLayout = new QHBoxLayout();

    categoryCombo = new QComboBox();
    categoryCombo->addItems({
        "Еда", "Транспорт", "Жильё",
        "Развлечения", "Здоровье", "Одежда", "Другое"
    });
    categoryCombo->setMinimumWidth(120);

    descriptionEdit = new QLineEdit();
    descriptionEdit->setPlaceholderText("Описание...");
    descriptionEdit->setMaxLength(100);

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(0.01, 999999.99);
    amountSpin->setPrefix("₽ ");
    amountSpin->setDecimals(2);
    amountSpin->setValue(0.01);

    addBtn = new QPushButton("Добавить");
    addBtn->setStyleSheet(
        "background-color: #27ae60; color: white; font-weight: bold; padding: 6px 16px;");

    inputLayout->addWidget(new QLabel("Категория:"));
    inputLayout->addWidget(categoryCombo);
    inputLayout->addWidget(new QLabel("Описание:"));
    inputLayout->addWidget(descriptionEdit, 1);
    inputLayout->addWidget(new QLabel("Сумма:"));
    inputLayout->addWidget(amountSpin);
    inputLayout->addWidget(addBtn);

    inputGroup->setLayout(inputLayout);
    // === Таблица ===
    table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(
        {"Дата", "Категория", "Описание", "Сумма"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::Stretch);  // Описание тянется
    table->setSelectionBehavior(
        QAbstractItemView::SelectRows);
    table->setEditTriggers(
        QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);

    mainLayout->addWidget(table, 1);  // 1 = stretch factor

    mainLayout->addWidget(inputGroup);

    // === Фильтр + кнопка удаления ===
    QHBoxLayout *controlRow = new QHBoxLayout();

    controlRow->addWidget(new QLabel("Фильтр:"));
    filterCombo = new QComboBox();
    filterCombo->addItem("Все");
    filterCombo->addItems({
        "Еда", "Транспорт", "Жильё",
        "Развлечения", "Здоровье", "Одежда", "Другое"
    });
    controlRow->addWidget(filterCombo);

    controlRow->addStretch();  // пружина

    deleteBtn = new QPushButton("Удалить выбранное");
    deleteBtn->setStyleSheet("background-color: #e74c3c; color: white; padding: 6px 12px;");
    controlRow->addWidget(deleteBtn);

    exportBtn = new QPushButton("Экспорт в CSV");
    exportBtn->setStyleSheet("background-color: #3498db; color: white; padding: 6px 12px;");
    controlRow->addWidget(exportBtn);

    mainLayout->addLayout(controlRow);

    // === Итоги ===
    QGroupBox *summaryGroup = new QGroupBox("Итоги");
    QHBoxLayout *summaryLayout = new QHBoxLayout();

    totalLabel = new QLabel("Потрачено: 0.00 ₽");
    budgetLabel = new QLabel(
        "Бюджет: " + QString::number(budget, 'f', 2)
        + " ₽");
    remainLabel = new QLabel("Остаток: 0.00 ₽");

    totalLabel->setStyleSheet("font-size: 14px;");
    budgetLabel->setStyleSheet("font-size: 14px;");
    remainLabel->setStyleSheet("font-size: 14px; font-weight: bold;");

    summaryLayout->addWidget(totalLabel);
    summaryLayout->addWidget(budgetLabel);
    summaryLayout->addWidget(remainLabel);

    summaryGroup->setLayout(summaryLayout);
    mainLayout->addWidget(summaryGroup);


    connect(deleteBtn, &QPushButton::clicked,
            this, &MainWindow::deleteSelected);
    connect(exportBtn, &QPushButton::clicked,
            this, &MainWindow::exportToCSV);
    connect(addBtn, &QPushButton::clicked,
           this, &MainWindow::addExpense);
    connect(descriptionEdit, &QLineEdit::returnPressed,
            this, &MainWindow::addExpense);
    connect(filterCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::filterByCategory);
    connect(table, &QTableWidget::cellDoubleClicked,
            this, &MainWindow::editExpense);
}

// === Реализация EditExpenseDialog ===
EditExpenseDialog::EditExpenseDialog(const Expense &expense, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Редактировать запись");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    // Категория
    categoryCombo = new QComboBox();
    categoryCombo->addItems({
        "Еда", "Транспорт", "Жильё",
        "Развлечения", "Здоровье", "Одежда", "Другое"
    });
    categoryCombo->setCurrentText(expense.category);
    formLayout->addRow("Категория:", categoryCombo);

    // Описание
    descriptionEdit = new QLineEdit(expense.description);
    descriptionEdit->setMaxLength(100);
    formLayout->addRow("Описание:", descriptionEdit);

    // Сумма
    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(0.01, 999999.99);
    amountSpin->setPrefix("₽ ");
    amountSpin->setDecimals(2);
    amountSpin->setValue(expense.amount);
    formLayout->addRow("Сумма:", amountSpin);

    layout->addLayout(formLayout);

    // Кнопки OK/Cancel
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted,
            this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);

    layout->addWidget(buttonBox);
}

Expense EditExpenseDialog::getExpense() const {
    Expense e;
    e.category = categoryCombo->currentText();
    e.description = descriptionEdit->text().trimmed();
    e.amount = amountSpin->value();
    e.date = "";  // Дата сохраняется из оригинальной записи
    return e;
}

void MainWindow::addExpense() {
    QString desc = descriptionEdit->text().trimmed();
    double amount = amountSpin->value();

    if (desc.isEmpty()) {
        QMessageBox::warning(this,
            "Ошибка", "Введите описание расхода.");
        descriptionEdit->setFocus();
        return;
    }

    Expense e;
    e.category = categoryCombo->currentText();
    e.description = desc;
    e.amount = amount;
    e.date = QDate::currentDate().toString("dd.MM.yyyy");

    expenses.append(e);

    // Сброс формы
    descriptionEdit->clear();
    amountSpin->setValue(0.01);
    descriptionEdit->setFocus();

    refreshTable();
    updateSummary();
}
void MainWindow::editExpense(int row, int column) {
    Q_UNUSED(column);

    // Проверяем, что строка валидна и есть индекс в filteredIndexes
    if (row < 0 || row >= filteredIndexes.size()) {
        return;
    }

    // Получаем индекс в массиве expenses
    int expenseIndex = filteredIndexes[row];
    if (expenseIndex < 0 || expenseIndex >= expenses.size()) {
        return;
    }

    // Создаём диалог с текущими значениями
    EditExpenseDialog dialog(expenses[expenseIndex], this);

    if (dialog.exec() == QDialog::Accepted) {
        // Обновляем запись
        Expense updated = dialog.getExpense();
        if (updated.description.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Описание не может быть пустым.");
            return;
        }

        // Сохраняем оригинальную дату
        updated.date = expenses[expenseIndex].date;
        expenses[expenseIndex] = updated;

        refreshTable();
        updateSummary();
    }
}
void MainWindow::exportToCSV() {
    if (expenses.isEmpty()) {
        QMessageBox::information(this, "Экспорт",
            "Нет данных для экспорта.");
        return;
    }

    // Выбираем файл для сохранения
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Экспорт расходов в CSV",
        QDir::homePath() + "/expenses.csv",
        "CSV файлы (*.csv);;Все файлы (*)"
    );

    if (fileName.isEmpty()) {
        return;  // Пользователь отменил выбор
    }

    // Создаём и открываем файл
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка",
            "Не удалось открыть файл для записи: " + fileName);
        return;
    }

    // Записываем данные в CSV формате
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);  // UTF-8 кодировка

    // Заголовок
    out << "Дата;Категория;Описание;Сумма\n";

    // Данные
    for (const auto &e : expenses) {
        out << e.date << ";"
            << e.category << ";"
            << e.description << ";"
            << QString::number(e.amount, 'f', 2) << "\n";
    }

    file.close();

    QMessageBox::information(this, "Экспорт",
        "Данные успешно экспортированы в файл:\n" + fileName);
}
void MainWindow::deleteSelected() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this,
            "Удаление",
            "Выделите строку для удаления.");
        return;
    }

    // Находим запись по дате+описанию
    QString date = table->item(row, 0)->text();
    QString desc = table->item(row, 2)->text();

    for (int i = 0; i < expenses.size(); ++i) {
        if (expenses[i].date == date
            && expenses[i].description == desc) {
            expenses.removeAt(i);
            break;
            }
    }
    refreshTable();
    updateSummary();
}
void MainWindow::updateSummary() {
    double total = 0.0;
    for (const auto &e : expenses)
        total += e.amount;

    double remain = budget - total;

    totalLabel->setText("Потрачено: "
        + QString::number(total, 'f', 2) + " ₽");
    remainLabel->setText("Остаток: "
        + QString::number(remain, 'f', 2) + " ₽");

    // Цветовая индикация
    if (remain < 0) {
        remainLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #e74c3c;");
    } else if (remain < budget * 0.2) {
        remainLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #f39c12;");
    } else {
        remainLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #27ae60;");
    }
}
void MainWindow::filterByCategory(
        const QString &category) {
    Q_UNUSED(category);
    refreshTable();
}
void MainWindow::refreshTable() {
    // Запоминаем текущий фильтр
    QString filter = filterCombo
        ? filterCombo->currentText() : "Все";

    table->setRowCount(0);
    filteredIndexes.clear();

    for (int i = 0; i < expenses.size(); ++i) {
        const auto &e = expenses[i];

        if (filter != "Все" && e.category != filter)
            continue;

        int row = table->rowCount();
        table->insertRow(row);
        filteredIndexes.append(i);  // сохраняем оригинальный индекс

        table->setItem(row, 0,
            new QTableWidgetItem(e.date));
        table->setItem(row, 1,
            new QTableWidgetItem(e.category));
        table->setItem(row, 2,
            new QTableWidgetItem(e.description));

        auto *amountItem = new QTableWidgetItem(
            QString::number(e.amount, 'f', 2) + " ₽");
        amountItem->setTextAlignment(
            Qt::AlignRight | Qt::AlignVCenter);
        table->setItem(row, 3, amountItem);
    }
}
