/****************************************************************************
** Meta object code from reading C++ file 'ntiss.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ntiss.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ntiss.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ntiss_t {
    QByteArrayData data[18];
    char stringdata0[322];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ntiss_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ntiss_t qt_meta_stringdata_ntiss = {
    {
QT_MOC_LITERAL(0, 0, 5), // "ntiss"
QT_MOC_LITERAL(1, 6, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 23), // "on_pushButton_6_clicked"
QT_MOC_LITERAL(4, 53, 31), // "on_comboBox_currentIndexChanged"
QT_MOC_LITERAL(5, 85, 5), // "index"
QT_MOC_LITERAL(6, 91, 10), // "paintEvent"
QT_MOC_LITERAL(7, 102, 12), // "QPaintEvent*"
QT_MOC_LITERAL(8, 115, 5), // "event"
QT_MOC_LITERAL(9, 121, 23), // "on_btn_draw_src_clicked"
QT_MOC_LITERAL(10, 145, 22), // "on_btn_draw_ec_clicked"
QT_MOC_LITERAL(11, 168, 17), // "compute_src_graph"
QT_MOC_LITERAL(12, 186, 16), // "compute_ec_graph"
QT_MOC_LITERAL(13, 203, 23), // "on_btn_zoom_out_clicked"
QT_MOC_LITERAL(14, 227, 22), // "on_btn_zoom_in_clicked"
QT_MOC_LITERAL(15, 250, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(16, 274, 23), // "on_pushButton_7_clicked"
QT_MOC_LITERAL(17, 298, 23) // "on_pushButton_3_clicked"

    },
    "ntiss\0on_pushButton_clicked\0\0"
    "on_pushButton_6_clicked\0"
    "on_comboBox_currentIndexChanged\0index\0"
    "paintEvent\0QPaintEvent*\0event\0"
    "on_btn_draw_src_clicked\0on_btn_draw_ec_clicked\0"
    "compute_src_graph\0compute_ec_graph\0"
    "on_btn_zoom_out_clicked\0on_btn_zoom_in_clicked\0"
    "on_pushButton_2_clicked\0on_pushButton_7_clicked\0"
    "on_pushButton_3_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ntiss[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    1,   81,    2, 0x08 /* Private */,
       6,    1,   84,    2, 0x08 /* Private */,
       9,    0,   87,    2, 0x08 /* Private */,
      10,    0,   88,    2, 0x08 /* Private */,
      11,    0,   89,    2, 0x08 /* Private */,
      12,    0,   90,    2, 0x08 /* Private */,
      13,    0,   91,    2, 0x08 /* Private */,
      14,    0,   92,    2, 0x08 /* Private */,
      15,    0,   93,    2, 0x08 /* Private */,
      16,    0,   94,    2, 0x08 /* Private */,
      17,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
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

void ntiss::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ntiss *_t = static_cast<ntiss *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_pushButton_6_clicked(); break;
        case 2: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        case 4: _t->on_btn_draw_src_clicked(); break;
        case 5: _t->on_btn_draw_ec_clicked(); break;
        case 6: _t->compute_src_graph(); break;
        case 7: _t->compute_ec_graph(); break;
        case 8: _t->on_btn_zoom_out_clicked(); break;
        case 9: _t->on_btn_zoom_in_clicked(); break;
        case 10: _t->on_pushButton_2_clicked(); break;
        case 11: _t->on_pushButton_7_clicked(); break;
        case 12: _t->on_pushButton_3_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject ntiss::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ntiss.data,
      qt_meta_data_ntiss,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ntiss::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ntiss::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ntiss.stringdata0))
        return static_cast<void*>(const_cast< ntiss*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ntiss::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
