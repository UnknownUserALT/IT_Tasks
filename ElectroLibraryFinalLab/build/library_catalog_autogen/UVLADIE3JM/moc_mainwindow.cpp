/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/mainwindow.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MainWindow_t {
    uint offsetsAndSizes[40];
    char stringdata0[11];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[13];
    char stringdata5[20];
    char stringdata6[5];
    char stringdata7[22];
    char stringdata8[6];
    char stringdata9[13];
    char stringdata10[13];
    char stringdata11[14];
    char stringdata12[16];
    char stringdata13[5];
    char stringdata14[14];
    char stringdata15[8];
    char stringdata16[23];
    char stringdata17[17];
    char stringdata18[18];
    char stringdata19[21];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 9),  // "onAddBook"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 10),  // "onEditBook"
        QT_MOC_LITERAL(33, 12),  // "onRemoveBook"
        QT_MOC_LITERAL(46, 19),  // "onSearchTextChanged"
        QT_MOC_LITERAL(66, 4),  // "text"
        QT_MOC_LITERAL(71, 21),  // "onStatusFilterChanged"
        QT_MOC_LITERAL(93, 5),  // "index"
        QT_MOC_LITERAL(99, 12),  // "onImportJson"
        QT_MOC_LITERAL(112, 12),  // "onExportJson"
        QT_MOC_LITERAL(125, 13),  // "onFetchByIsbn"
        QT_MOC_LITERAL(139, 15),  // "onFetchFinished"
        QT_MOC_LITERAL(155, 4),  // "data"
        QT_MOC_LITERAL(160, 13),  // "onFetchFailed"
        QT_MOC_LITERAL(174, 7),  // "message"
        QT_MOC_LITERAL(182, 22),  // "onBookSelectionChanged"
        QT_MOC_LITERAL(205, 16),  // "onAddFetchedBook"
        QT_MOC_LITERAL(222, 17),  // "refreshStatistics"
        QT_MOC_LITERAL(240, 20)   // "updateStatusBarCount"
    },
    "MainWindow",
    "onAddBook",
    "",
    "onEditBook",
    "onRemoveBook",
    "onSearchTextChanged",
    "text",
    "onStatusFilterChanged",
    "index",
    "onImportJson",
    "onExportJson",
    "onFetchByIsbn",
    "onFetchFinished",
    "data",
    "onFetchFailed",
    "message",
    "onBookSelectionChanged",
    "onAddFetchedBook",
    "refreshStatistics",
    "updateStatusBarCount"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   98,    2, 0x08,    1 /* Private */,
       3,    0,   99,    2, 0x08,    2 /* Private */,
       4,    0,  100,    2, 0x08,    3 /* Private */,
       5,    1,  101,    2, 0x08,    4 /* Private */,
       7,    1,  104,    2, 0x08,    6 /* Private */,
       9,    0,  107,    2, 0x08,    8 /* Private */,
      10,    0,  108,    2, 0x08,    9 /* Private */,
      11,    0,  109,    2, 0x08,   10 /* Private */,
      12,    1,  110,    2, 0x08,   11 /* Private */,
      14,    1,  113,    2, 0x08,   13 /* Private */,
      16,    0,  116,    2, 0x08,   15 /* Private */,
      17,    0,  117,    2, 0x08,   16 /* Private */,
      18,    0,  118,    2, 0x08,   17 /* Private */,
      19,    0,  119,    2, 0x08,   18 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariantMap,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSizes,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onAddBook'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditBook'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRemoveBook'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onStatusFilterChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onImportJson'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExportJson'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFetchByIsbn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFetchFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'onFetchFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onBookSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddFetchedBook'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshStatistics'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateStatusBarCount'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAddBook(); break;
        case 1: _t->onEditBook(); break;
        case 2: _t->onRemoveBook(); break;
        case 3: _t->onSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onStatusFilterChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onImportJson(); break;
        case 6: _t->onExportJson(); break;
        case 7: _t->onFetchByIsbn(); break;
        case 8: _t->onFetchFinished((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        case 9: _t->onFetchFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->onBookSelectionChanged(); break;
        case 11: _t->onAddFetchedBook(); break;
        case 12: _t->refreshStatistics(); break;
        case 13: _t->updateStatusBarCount(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
