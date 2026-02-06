/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../qtreeView/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata0[301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 15), // "onAddNewProject"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 28), // "onCustomContextMenuRequested"
QT_MOC_LITERAL(4, 57, 3), // "pos"
QT_MOC_LITERAL(5, 61, 14), // "onCloseProject"
QT_MOC_LITERAL(6, 76, 15), // "onRenameProject"
QT_MOC_LITERAL(7, 92, 13), // "onSaveProject"
QT_MOC_LITERAL(8, 106, 21), // "onOpenProjectLocation"
QT_MOC_LITERAL(9, 128, 14), // "onNewSchematic"
QT_MOC_LITERAL(10, 143, 8), // "onNewPcb"
QT_MOC_LITERAL(11, 152, 17), // "onAddExistingFile"
QT_MOC_LITERAL(12, 170, 22), // "onCloseAllProjectFiles"
QT_MOC_LITERAL(13, 193, 15), // "onSaveProjectAs"
QT_MOC_LITERAL(14, 209, 10), // "onSaveFile"
QT_MOC_LITERAL(15, 220, 12), // "onSaveFileAs"
QT_MOC_LITERAL(16, 233, 23), // "onRemoveFileFromProject"
QT_MOC_LITERAL(17, 257, 11), // "onCloseFile"
QT_MOC_LITERAL(18, 269, 12), // "onRenameFile"
QT_MOC_LITERAL(19, 282, 18) // "onOpenFileLocation"

    },
    "MainWindow\0onAddNewProject\0\0"
    "onCustomContextMenuRequested\0pos\0"
    "onCloseProject\0onRenameProject\0"
    "onSaveProject\0onOpenProjectLocation\0"
    "onNewSchematic\0onNewPcb\0onAddExistingFile\0"
    "onCloseAllProjectFiles\0onSaveProjectAs\0"
    "onSaveFile\0onSaveFileAs\0onRemoveFileFromProject\0"
    "onCloseFile\0onRenameFile\0onOpenFileLocation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x08 /* Private */,
       3,    1,  100,    2, 0x08 /* Private */,
       5,    0,  103,    2, 0x08 /* Private */,
       6,    0,  104,    2, 0x08 /* Private */,
       7,    0,  105,    2, 0x08 /* Private */,
       8,    0,  106,    2, 0x08 /* Private */,
       9,    0,  107,    2, 0x08 /* Private */,
      10,    0,  108,    2, 0x08 /* Private */,
      11,    0,  109,    2, 0x08 /* Private */,
      12,    0,  110,    2, 0x08 /* Private */,
      13,    0,  111,    2, 0x08 /* Private */,
      14,    0,  112,    2, 0x08 /* Private */,
      15,    0,  113,    2, 0x08 /* Private */,
      16,    0,  114,    2, 0x08 /* Private */,
      17,    0,  115,    2, 0x08 /* Private */,
      18,    0,  116,    2, 0x08 /* Private */,
      19,    0,  117,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onAddNewProject(); break;
        case 1: _t->onCustomContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: _t->onCloseProject(); break;
        case 3: _t->onRenameProject(); break;
        case 4: _t->onSaveProject(); break;
        case 5: _t->onOpenProjectLocation(); break;
        case 6: _t->onNewSchematic(); break;
        case 7: _t->onNewPcb(); break;
        case 8: _t->onAddExistingFile(); break;
        case 9: _t->onCloseAllProjectFiles(); break;
        case 10: _t->onSaveProjectAs(); break;
        case 11: _t->onSaveFile(); break;
        case 12: _t->onSaveFileAs(); break;
        case 13: _t->onRemoveFileFromProject(); break;
        case 14: _t->onCloseFile(); break;
        case 15: _t->onRenameFile(); break;
        case 16: _t->onOpenFileLocation(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
