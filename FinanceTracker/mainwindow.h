#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QVector>
#include <QDialog>
#include <QDialogButtonBox>

struct Expense {
    QString category;
    QString description;
    double amount;
    QString date;  // для простоты храним как строку
};

// Диалоговое окно для редактирования записи
class EditExpenseDialog : public QDialog {
    Q_OBJECT
public:
    EditExpenseDialog(const Expense &expense, QWidget *parent = nullptr);

    Expense getExpense() const;

private:
    QComboBox *categoryCombo;
    QLineEdit *descriptionEdit;
    QDoubleSpinBox *amountSpin;
};

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addExpense();
    void deleteSelected();
    void editExpense(int row, int column);
    void exportToCSV();
    void updateSummary();
    void filterByCategory(const QString &category);

private:
    void setupUI();
    void refreshTable();

    // Данные
    QVector<Expense> expenses;
    QVector<int> filteredIndexes;  // индексы отображаемых записей (для фильтрации)
    double budget = 50000.0;

    // Виджеты ввода
    QComboBox *categoryCombo;
    QLineEdit *descriptionEdit;
    QDoubleSpinBox *amountSpin;
    QPushButton *addBtn;
    QPushButton *deleteBtn;
    QPushButton *exportBtn;

    // Таблица
    QTableWidget *table;

    // Фильтр
    QComboBox *filterCombo;

    // Итоги
    QLabel *totalLabel;
    QLabel *budgetLabel;
    QLabel *remainLabel;
};

#endif
