/****************************************************************************
** Meta object code from reading C++ file 'Terrain.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "Terrain.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Terrain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Terrain_t {
    QByteArrayData data[6];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Terrain_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Terrain_t qt_meta_stringdata_Terrain = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Terrain"
QT_MOC_LITERAL(1, 8, 15), // "menuToggleWater"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "menuToggleDraw"
QT_MOC_LITERAL(4, 40, 14), // "menuPutTexture"
QT_MOC_LITERAL(5, 55, 17) // "menuSelectObjects"

    },
    "Terrain\0menuToggleWater\0\0menuToggleDraw\0"
    "menuPutTexture\0menuSelectObjects"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Terrain[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Terrain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Terrain *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->menuToggleWater(); break;
        case 1: _t->menuToggleDraw(); break;
        case 2: _t->menuPutTexture(); break;
        case 3: _t->menuSelectObjects(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Terrain::staticMetaObject = { {
    QMetaObject::SuperData::link<GameObj::staticMetaObject>(),
    qt_meta_stringdata_Terrain.data,
    qt_meta_data_Terrain,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Terrain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Terrain::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Terrain.stringdata0))
        return static_cast<void*>(this);
    return GameObj::qt_metacast(_clname);
}

int Terrain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GameObj::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
