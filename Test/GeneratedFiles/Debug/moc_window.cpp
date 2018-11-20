/****************************************************************************
** Meta object code from reading C++ file 'window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "pch.h"
#include "../../window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Window_t {
    QByteArrayData data[13];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Window_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Window_t qt_meta_stringdata_Window = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Window"
QT_MOC_LITERAL(1, 7, 20), // "markProductAvailable"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "QInAppProduct*"
QT_MOC_LITERAL(4, 44, 21), // "handleErrorGracefully"
QT_MOC_LITERAL(5, 66, 17), // "handleTransaction"
QT_MOC_LITERAL(6, 84, 18), // "QInAppTransaction*"
QT_MOC_LITERAL(7, 103, 16), // "purchaseMultiple"
QT_MOC_LITERAL(8, 120, 15), // "purchaseLimited"
QT_MOC_LITERAL(9, 136, 14), // "restoreLimited"
QT_MOC_LITERAL(10, 151, 11), // "restoreOnce"
QT_MOC_LITERAL(11, 163, 12), // "purchaseOnce"
QT_MOC_LITERAL(12, 176, 13) // "openUWPWindow"

    },
    "Window\0markProductAvailable\0\0"
    "QInAppProduct*\0handleErrorGracefully\0"
    "handleTransaction\0QInAppTransaction*\0"
    "purchaseMultiple\0purchaseLimited\0"
    "restoreLimited\0restoreOnce\0purchaseOnce\0"
    "openUWPWindow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Window[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a /* Public */,
       4,    1,   62,    2, 0x0a /* Public */,
       5,    1,   65,    2, 0x0a /* Public */,
       7,    0,   68,    2, 0x0a /* Public */,
       8,    0,   69,    2, 0x0a /* Public */,
       9,    0,   70,    2, 0x0a /* Public */,
      10,    0,   71,    2, 0x0a /* Public */,
      11,    0,   72,    2, 0x0a /* Public */,
      12,    0,   73,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Window *_t = static_cast<Window *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->markProductAvailable((*reinterpret_cast< QInAppProduct*(*)>(_a[1]))); break;
        case 1: _t->handleErrorGracefully((*reinterpret_cast< QInAppProduct*(*)>(_a[1]))); break;
        case 2: _t->handleTransaction((*reinterpret_cast< QInAppTransaction*(*)>(_a[1]))); break;
        case 3: _t->purchaseMultiple(); break;
        case 4: _t->purchaseLimited(); break;
        case 5: _t->restoreLimited(); break;
        case 6: _t->restoreOnce(); break;
        case 7: _t->purchaseOnce(); break;
        case 8: _t->openUWPWindow(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Window::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Window.data,
    qt_meta_data_Window,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Window.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
