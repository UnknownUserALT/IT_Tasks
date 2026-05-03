/********************************************************************************
** Form generated from reading UI file 'bookdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOKDIALOG_H
#define UI_BOOKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BookDialog
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *labelTitle;
    QLineEdit *editTitle;
    QLabel *labelAuthor;
    QLineEdit *editAuthor;
    QLabel *labelYear;
    QSpinBox *spinYear;
    QLabel *labelIsbn;
    QLineEdit *editIsbn;
    QLabel *labelGenre;
    QComboBox *comboGenre;
    QLabel *labelStatus;
    QComboBox *comboStatus;
    QLabel *labelRating;
    QSpinBox *spinRating;
    QLabel *labelPages;
    QSpinBox *spinPages;
    QLabel *labelNotes;
    QPlainTextEdit *editNotes;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSave;
    QPushButton *btnCancel;

    void setupUi(QDialog *BookDialog)
    {
        if (BookDialog->objectName().isEmpty())
            BookDialog->setObjectName("BookDialog");
        BookDialog->setMinimumSize(QSize(450, 500));
        mainLayout = new QVBoxLayout(BookDialog);
        mainLayout->setObjectName("mainLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelTitle = new QLabel(BookDialog);
        labelTitle->setObjectName("labelTitle");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelTitle);

        editTitle = new QLineEdit(BookDialog);
        editTitle->setObjectName("editTitle");

        formLayout->setWidget(0, QFormLayout::FieldRole, editTitle);

        labelAuthor = new QLabel(BookDialog);
        labelAuthor->setObjectName("labelAuthor");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelAuthor);

        editAuthor = new QLineEdit(BookDialog);
        editAuthor->setObjectName("editAuthor");

        formLayout->setWidget(1, QFormLayout::FieldRole, editAuthor);

        labelYear = new QLabel(BookDialog);
        labelYear->setObjectName("labelYear");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelYear);

        spinYear = new QSpinBox(BookDialog);
        spinYear->setObjectName("spinYear");
        spinYear->setMinimum(1);
        spinYear->setMaximum(2026);
        spinYear->setValue(2024);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinYear);

        labelIsbn = new QLabel(BookDialog);
        labelIsbn->setObjectName("labelIsbn");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelIsbn);

        editIsbn = new QLineEdit(BookDialog);
        editIsbn->setObjectName("editIsbn");

        formLayout->setWidget(3, QFormLayout::FieldRole, editIsbn);

        labelGenre = new QLabel(BookDialog);
        labelGenre->setObjectName("labelGenre");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelGenre);

        comboGenre = new QComboBox(BookDialog);
        comboGenre->setObjectName("comboGenre");
        comboGenre->setEditable(true);

        formLayout->setWidget(4, QFormLayout::FieldRole, comboGenre);

        labelStatus = new QLabel(BookDialog);
        labelStatus->setObjectName("labelStatus");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelStatus);

        comboStatus = new QComboBox(BookDialog);
        comboStatus->setObjectName("comboStatus");

        formLayout->setWidget(5, QFormLayout::FieldRole, comboStatus);

        labelRating = new QLabel(BookDialog);
        labelRating->setObjectName("labelRating");

        formLayout->setWidget(6, QFormLayout::LabelRole, labelRating);

        spinRating = new QSpinBox(BookDialog);
        spinRating->setObjectName("spinRating");
        spinRating->setMinimum(0);
        spinRating->setMaximum(5);

        formLayout->setWidget(6, QFormLayout::FieldRole, spinRating);

        labelPages = new QLabel(BookDialog);
        labelPages->setObjectName("labelPages");

        formLayout->setWidget(7, QFormLayout::LabelRole, labelPages);

        spinPages = new QSpinBox(BookDialog);
        spinPages->setObjectName("spinPages");
        spinPages->setMinimum(0);
        spinPages->setMaximum(5000);

        formLayout->setWidget(7, QFormLayout::FieldRole, spinPages);

        labelNotes = new QLabel(BookDialog);
        labelNotes->setObjectName("labelNotes");

        formLayout->setWidget(8, QFormLayout::LabelRole, labelNotes);

        editNotes = new QPlainTextEdit(BookDialog);
        editNotes->setObjectName("editNotes");
        editNotes->setMaximumHeight(100);

        formLayout->setWidget(8, QFormLayout::FieldRole, editNotes);


        mainLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(verticalSpacer);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        btnSave = new QPushButton(BookDialog);
        btnSave->setObjectName("btnSave");
        btnSave->setEnabled(false);

        buttonLayout->addWidget(btnSave);

        btnCancel = new QPushButton(BookDialog);
        btnCancel->setObjectName("btnCancel");

        buttonLayout->addWidget(btnCancel);


        mainLayout->addLayout(buttonLayout);


        retranslateUi(BookDialog);

        QMetaObject::connectSlotsByName(BookDialog);
    } // setupUi

    void retranslateUi(QDialog *BookDialog)
    {
        BookDialog->setWindowTitle(QCoreApplication::translate("BookDialog", "\320\235\320\276\320\262\320\260\321\217 \320\272\320\275\320\270\320\263\320\260", nullptr));
        labelTitle->setText(QCoreApplication::translate("BookDialog", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 *", nullptr));
        editTitle->setPlaceholderText(QCoreApplication::translate("BookDialog", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\275\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\272\320\275\320\270\320\263\320\270", nullptr));
        labelAuthor->setText(QCoreApplication::translate("BookDialog", "\320\220\320\262\321\202\320\276\321\200 *", nullptr));
        editAuthor->setPlaceholderText(QCoreApplication::translate("BookDialog", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\260\320\262\321\202\320\276\321\200\320\260", nullptr));
        labelYear->setText(QCoreApplication::translate("BookDialog", "\320\223\320\276\320\264", nullptr));
        labelIsbn->setText(QCoreApplication::translate("BookDialog", "ISBN", nullptr));
        editIsbn->setPlaceholderText(QCoreApplication::translate("BookDialog", "10 \320\270\320\273\320\270 13 \321\206\320\270\321\204\321\200", nullptr));
        labelGenre->setText(QCoreApplication::translate("BookDialog", "\320\226\320\260\320\275\321\200", nullptr));
        labelStatus->setText(QCoreApplication::translate("BookDialog", "\320\241\321\202\320\260\321\202\321\203\321\201", nullptr));
        labelRating->setText(QCoreApplication::translate("BookDialog", "\320\240\320\265\320\271\321\202\320\270\320\275\320\263", nullptr));
        labelPages->setText(QCoreApplication::translate("BookDialog", "\320\241\321\202\321\200\320\260\320\275\320\270\321\206", nullptr));
        labelNotes->setText(QCoreApplication::translate("BookDialog", "\320\227\320\260\320\274\320\265\321\202\320\272\320\270", nullptr));
        btnSave->setText(QCoreApplication::translate("BookDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        btnCancel->setText(QCoreApplication::translate("BookDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BookDialog: public Ui_BookDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKDIALOG_H
