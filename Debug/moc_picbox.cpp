/****************************************************************************
** Meta object code from reading C++ file 'picbox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../picbox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'picbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PicBox_t {
    QByteArrayData data[10];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PicBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PicBox_t qt_meta_stringdata_PicBox = {
    {
QT_MOC_LITERAL(0, 0, 6), // "PicBox"
QT_MOC_LITERAL(1, 7, 7), // "openImg"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 8), // "clearImg"
QT_MOC_LITERAL(4, 25, 7), // "saveImg"
QT_MOC_LITERAL(5, 33, 8), // "resetImg"
QT_MOC_LITERAL(6, 42, 7), // "testImg"
QT_MOC_LITERAL(7, 50, 11), // "stretchImg1"
QT_MOC_LITERAL(8, 62, 11), // "stretchImg2"
QT_MOC_LITERAL(9, 74, 11) // "stretchImg5"

    },
    "PicBox\0openImg\0\0clearImg\0saveImg\0"
    "resetImg\0testImg\0stretchImg1\0stretchImg2\0"
    "stretchImg5"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PicBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
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

void PicBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PicBox *_t = static_cast<PicBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openImg(); break;
        case 1: _t->clearImg(); break;
        case 2: _t->saveImg(); break;
        case 3: _t->resetImg(); break;
        case 4: _t->testImg(); break;
        case 5: _t->stretchImg1(); break;
        case 6: _t->stretchImg2(); break;
        case 7: _t->stretchImg5(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject PicBox::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_PicBox.data,
      qt_meta_data_PicBox,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PicBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PicBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PicBox.stringdata0))
        return static_cast<void*>(const_cast< PicBox*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int PicBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
