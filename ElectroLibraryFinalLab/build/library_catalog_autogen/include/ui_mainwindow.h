/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *centralLayout;
    QTabWidget *tabWidget;
    QWidget *tabLibrary;
    QVBoxLayout *libraryMainLayout;
    QHBoxLayout *searchLayout;
    QLineEdit *editSearch;
    QComboBox *comboStatusFilter;
    QSplitter *librarySplitter;
    QTableView *tableBooks;
    QWidget *detailsPanel;
    QVBoxLayout *detailsLayout;
    QLabel *lblDetailTitle;
    QLabel *lblDetailAuthor;
    QLabel *lblDetailInfo;
    QLabel *lblDetailNotes;
    QLabel *lblDetailCover;
    QSpacerItem *detailsSpacer;
    QWidget *tabStatistics;
    QVBoxLayout *statsMainLayout;
    QHBoxLayout *statsCardsLayout;
    QGroupBox *groupTotal;
    QVBoxLayout *vboxLayout;
    QLabel *lblStatTotal;
    QGroupBox *groupRead;
    QVBoxLayout *vboxLayout1;
    QLabel *lblStatRead;
    QGroupBox *groupReading;
    QVBoxLayout *vboxLayout2;
    QLabel *lblStatReading;
    QGroupBox *groupRating;
    QVBoxLayout *vboxLayout3;
    QLabel *lblStatRating;
    QLabel *lblChartTitle;
    QWidget *tabImport;
    QVBoxLayout *importMainLayout;
    QHBoxLayout *isbnSearchLayout;
    QLabel *lblIsbn;
    QLineEdit *editIsbn;
    QPushButton *btnFetch;
    QGroupBox *groupResult;
    QFormLayout *resultLayout;
    QLabel *label;
    QLabel *lblResultTitle;
    QLabel *label1;
    QLabel *lblResultAuthor;
    QLabel *label2;
    QLabel *lblResultDate;
    QLabel *label3;
    QLabel *lblResultPages;
    QLabel *label4;
    QLabel *lblResultGenre;
    QLabel *label5;
    QLabel *lblResultCover;
    QPushButton *btnAddToLibrary;
    QSpacerItem *importSpacer;
    QMenuBar *menuBar;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setMinimumSize(QSize(900, 600));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        centralLayout = new QVBoxLayout(centralWidget);
        centralLayout->setObjectName("centralLayout");
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName("tabWidget");
        tabLibrary = new QWidget();
        tabLibrary->setObjectName("tabLibrary");
        libraryMainLayout = new QVBoxLayout(tabLibrary);
        libraryMainLayout->setObjectName("libraryMainLayout");
        searchLayout = new QHBoxLayout();
        searchLayout->setObjectName("searchLayout");
        editSearch = new QLineEdit(tabLibrary);
        editSearch->setObjectName("editSearch");

        searchLayout->addWidget(editSearch);

        comboStatusFilter = new QComboBox(tabLibrary);
        comboStatusFilter->setObjectName("comboStatusFilter");

        searchLayout->addWidget(comboStatusFilter);


        libraryMainLayout->addLayout(searchLayout);

        librarySplitter = new QSplitter(tabLibrary);
        librarySplitter->setObjectName("librarySplitter");
        librarySplitter->setOrientation(Qt::Horizontal);
        tableBooks = new QTableView(librarySplitter);
        tableBooks->setObjectName("tableBooks");
        tableBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableBooks->setSelectionMode(QAbstractItemView::SingleSelection);
        tableBooks->setSortingEnabled(true);
        tableBooks->setAlternatingRowColors(true);
        librarySplitter->addWidget(tableBooks);
        detailsPanel = new QWidget(librarySplitter);
        detailsPanel->setObjectName("detailsPanel");
        detailsLayout = new QVBoxLayout(detailsPanel);
        detailsLayout->setObjectName("detailsLayout");
        detailsLayout->setContentsMargins(0, 0, 0, 0);
        lblDetailTitle = new QLabel(detailsPanel);
        lblDetailTitle->setObjectName("lblDetailTitle");
        lblDetailTitle->setWordWrap(true);
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        lblDetailTitle->setFont(font);

        detailsLayout->addWidget(lblDetailTitle);

        lblDetailAuthor = new QLabel(detailsPanel);
        lblDetailAuthor->setObjectName("lblDetailAuthor");

        detailsLayout->addWidget(lblDetailAuthor);

        lblDetailInfo = new QLabel(detailsPanel);
        lblDetailInfo->setObjectName("lblDetailInfo");
        lblDetailInfo->setWordWrap(true);

        detailsLayout->addWidget(lblDetailInfo);

        lblDetailNotes = new QLabel(detailsPanel);
        lblDetailNotes->setObjectName("lblDetailNotes");
        lblDetailNotes->setWordWrap(true);

        detailsLayout->addWidget(lblDetailNotes);

        lblDetailCover = new QLabel(detailsPanel);
        lblDetailCover->setObjectName("lblDetailCover");
        lblDetailCover->setAlignment(Qt::AlignCenter);

        detailsLayout->addWidget(lblDetailCover);

        detailsSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        detailsLayout->addItem(detailsSpacer);

        librarySplitter->addWidget(detailsPanel);

        libraryMainLayout->addWidget(librarySplitter);

        tabWidget->addTab(tabLibrary, QString());
        tabStatistics = new QWidget();
        tabStatistics->setObjectName("tabStatistics");
        statsMainLayout = new QVBoxLayout(tabStatistics);
        statsMainLayout->setObjectName("statsMainLayout");
        statsCardsLayout = new QHBoxLayout();
        statsCardsLayout->setObjectName("statsCardsLayout");
        groupTotal = new QGroupBox(tabStatistics);
        groupTotal->setObjectName("groupTotal");
        vboxLayout = new QVBoxLayout(groupTotal);
        vboxLayout->setObjectName("vboxLayout");
        lblStatTotal = new QLabel(groupTotal);
        lblStatTotal->setObjectName("lblStatTotal");
        lblStatTotal->setAlignment(Qt::AlignCenter);
        QFont font1;
        font1.setPointSize(20);
        font1.setBold(true);
        lblStatTotal->setFont(font1);

        vboxLayout->addWidget(lblStatTotal);


        statsCardsLayout->addWidget(groupTotal);

        groupRead = new QGroupBox(tabStatistics);
        groupRead->setObjectName("groupRead");
        vboxLayout1 = new QVBoxLayout(groupRead);
        vboxLayout1->setObjectName("vboxLayout1");
        lblStatRead = new QLabel(groupRead);
        lblStatRead->setObjectName("lblStatRead");
        lblStatRead->setAlignment(Qt::AlignCenter);
        lblStatRead->setFont(font1);

        vboxLayout1->addWidget(lblStatRead);


        statsCardsLayout->addWidget(groupRead);

        groupReading = new QGroupBox(tabStatistics);
        groupReading->setObjectName("groupReading");
        vboxLayout2 = new QVBoxLayout(groupReading);
        vboxLayout2->setObjectName("vboxLayout2");
        lblStatReading = new QLabel(groupReading);
        lblStatReading->setObjectName("lblStatReading");
        lblStatReading->setAlignment(Qt::AlignCenter);
        lblStatReading->setFont(font1);

        vboxLayout2->addWidget(lblStatReading);


        statsCardsLayout->addWidget(groupReading);

        groupRating = new QGroupBox(tabStatistics);
        groupRating->setObjectName("groupRating");
        vboxLayout3 = new QVBoxLayout(groupRating);
        vboxLayout3->setObjectName("vboxLayout3");
        lblStatRating = new QLabel(groupRating);
        lblStatRating->setObjectName("lblStatRating");
        lblStatRating->setAlignment(Qt::AlignCenter);
        lblStatRating->setFont(font1);

        vboxLayout3->addWidget(lblStatRating);


        statsCardsLayout->addWidget(groupRating);


        statsMainLayout->addLayout(statsCardsLayout);

        lblChartTitle = new QLabel(tabStatistics);
        lblChartTitle->setObjectName("lblChartTitle");
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        lblChartTitle->setFont(font2);
        lblChartTitle->setAlignment(Qt::AlignCenter);

        statsMainLayout->addWidget(lblChartTitle);

        tabWidget->addTab(tabStatistics, QString());
        tabImport = new QWidget();
        tabImport->setObjectName("tabImport");
        importMainLayout = new QVBoxLayout(tabImport);
        importMainLayout->setObjectName("importMainLayout");
        isbnSearchLayout = new QHBoxLayout();
        isbnSearchLayout->setObjectName("isbnSearchLayout");
        lblIsbn = new QLabel(tabImport);
        lblIsbn->setObjectName("lblIsbn");

        isbnSearchLayout->addWidget(lblIsbn);

        editIsbn = new QLineEdit(tabImport);
        editIsbn->setObjectName("editIsbn");

        isbnSearchLayout->addWidget(editIsbn);

        btnFetch = new QPushButton(tabImport);
        btnFetch->setObjectName("btnFetch");

        isbnSearchLayout->addWidget(btnFetch);


        importMainLayout->addLayout(isbnSearchLayout);

        groupResult = new QGroupBox(tabImport);
        groupResult->setObjectName("groupResult");
        resultLayout = new QFormLayout(groupResult);
        resultLayout->setObjectName("resultLayout");
        label = new QLabel(groupResult);
        label->setObjectName("label");

        resultLayout->setWidget(0, QFormLayout::LabelRole, label);

        lblResultTitle = new QLabel(groupResult);
        lblResultTitle->setObjectName("lblResultTitle");
        lblResultTitle->setWordWrap(true);

        resultLayout->setWidget(0, QFormLayout::FieldRole, lblResultTitle);

        label1 = new QLabel(groupResult);
        label1->setObjectName("label1");

        resultLayout->setWidget(1, QFormLayout::LabelRole, label1);

        lblResultAuthor = new QLabel(groupResult);
        lblResultAuthor->setObjectName("lblResultAuthor");

        resultLayout->setWidget(1, QFormLayout::FieldRole, lblResultAuthor);

        label2 = new QLabel(groupResult);
        label2->setObjectName("label2");

        resultLayout->setWidget(2, QFormLayout::LabelRole, label2);

        lblResultDate = new QLabel(groupResult);
        lblResultDate->setObjectName("lblResultDate");

        resultLayout->setWidget(2, QFormLayout::FieldRole, lblResultDate);

        label3 = new QLabel(groupResult);
        label3->setObjectName("label3");

        resultLayout->setWidget(3, QFormLayout::LabelRole, label3);

        lblResultPages = new QLabel(groupResult);
        lblResultPages->setObjectName("lblResultPages");

        resultLayout->setWidget(3, QFormLayout::FieldRole, lblResultPages);

        label4 = new QLabel(groupResult);
        label4->setObjectName("label4");

        resultLayout->setWidget(4, QFormLayout::LabelRole, label4);

        lblResultGenre = new QLabel(groupResult);
        lblResultGenre->setObjectName("lblResultGenre");

        resultLayout->setWidget(4, QFormLayout::FieldRole, lblResultGenre);

        label5 = new QLabel(groupResult);
        label5->setObjectName("label5");

        resultLayout->setWidget(5, QFormLayout::LabelRole, label5);

        lblResultCover = new QLabel(groupResult);
        lblResultCover->setObjectName("lblResultCover");
        lblResultCover->setAlignment(Qt::AlignCenter);

        resultLayout->setWidget(5, QFormLayout::FieldRole, lblResultCover);


        importMainLayout->addWidget(groupResult);

        btnAddToLibrary = new QPushButton(tabImport);
        btnAddToLibrary->setObjectName("btnAddToLibrary");
        btnAddToLibrary->setEnabled(false);

        importMainLayout->addWidget(btnAddToLibrary);

        importSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        importMainLayout->addItem(importSpacer);

        tabWidget->addTab(tabImport, QString());

        centralLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        MainWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(toolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\232\320\260\321\202\320\260\320\273\320\276\320\263 \321\215\320\273\320\265\320\272\321\202\321\200\320\276\320\275\320\275\320\276\320\271 \320\261\320\270\320\261\320\273\320\270\320\276\321\202\320\265\320\272\320\270", nullptr));
        editSearch->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272 \320\277\320\276 \320\275\320\260\320\267\320\262\320\260\320\275\320\270\321\216 \320\270\320\273\320\270 \320\260\320\262\321\202\320\276\321\200\321\203...", nullptr));
        lblDetailTitle->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \320\272\320\275\320\270\320\263\321\203", nullptr));
        lblDetailAuthor->setText(QString());
        lblDetailInfo->setText(QString());
        lblDetailNotes->setText(QString());
        lblDetailCover->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabLibrary), QCoreApplication::translate("MainWindow", "\320\221\320\270\320\261\320\273\320\270\320\276\321\202\320\265\320\272\320\260", nullptr));
        groupTotal->setTitle(QCoreApplication::translate("MainWindow", "\320\222\321\201\320\265\320\263\320\276 \320\272\320\275\320\270\320\263", nullptr));
        lblStatTotal->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        groupRead->setTitle(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\321\207\320\270\321\202\320\260\320\275\320\276", nullptr));
        lblStatRead->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        groupReading->setTitle(QCoreApplication::translate("MainWindow", "\320\247\320\270\321\202\320\260\321\216", nullptr));
        lblStatReading->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        groupRating->setTitle(QCoreApplication::translate("MainWindow", "\320\241\321\200\320\265\320\264\320\275\320\270\320\271 \321\200\320\265\320\271\321\202\320\270\320\275\320\263", nullptr));
        lblStatRating->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        lblChartTitle->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\272\320\275\320\270\320\263 \320\277\320\276 \320\263\320\276\320\264\320\260\320\274", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabStatistics), QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        lblIsbn->setText(QCoreApplication::translate("MainWindow", "ISBN:", nullptr));
        editIsbn->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 ISBN (\320\275\320\260\320\277\321\200\320\270\320\274\320\265\321\200, 9780134685991)", nullptr));
        btnFetch->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\271\321\202\320\270", nullptr));
        groupResult->setTitle(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202 \320\277\320\276\320\270\321\201\320\272\320\260", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265:", nullptr));
        lblResultTitle->setText(QCoreApplication::translate("MainWindow", "\342\200\224", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "\320\220\320\262\321\202\320\276\321\200:", nullptr));
        lblResultAuthor->setText(QCoreApplication::translate("MainWindow", "\342\200\224", nullptr));
        label2->setText(QCoreApplication::translate("MainWindow", "\320\224\320\260\321\202\320\260 \320\277\321\203\320\261\320\273\320\270\320\272\320\260\321\206\320\270\320\270:", nullptr));
        lblResultDate->setText(QCoreApplication::translate("MainWindow", "\342\200\224", nullptr));
        label3->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\321\200\320\260\320\275\320\270\321\206:", nullptr));
        lblResultPages->setText(QCoreApplication::translate("MainWindow", "\342\200\224", nullptr));
        label4->setText(QCoreApplication::translate("MainWindow", "\320\226\320\260\320\275\321\200:", nullptr));
        lblResultGenre->setText(QCoreApplication::translate("MainWindow", "\342\200\224", nullptr));
        label5->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\320\273\320\276\320\266\320\272\320\260:", nullptr));
        lblResultCover->setText(QString());
        btnAddToLibrary->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\262 \320\261\320\270\320\261\320\273\320\270\320\276\321\202\320\265\320\272\321\203", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabImport), QCoreApplication::translate("MainWindow", "\320\230\320\274\320\277\320\276\321\200\321\202 \320\270\320\267 OpenLibrary", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\237\320\260\320\275\320\265\320\273\321\214 \320\270\320\275\321\201\321\202\321\200\321\203\320\274\320\265\320\275\321\202\320\276\320\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
