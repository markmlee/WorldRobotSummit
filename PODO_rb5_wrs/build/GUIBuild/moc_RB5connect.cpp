/****************************************************************************
** Meta object code from reading C++ file 'RB5connect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../rb5_gui/LAN/RB5connect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RB5connect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RB5connect_t {
    QByteArrayData data[14];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RB5connect_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RB5connect_t qt_meta_stringdata_RB5connect = {
    {
QT_MOC_LITERAL(0, 0, 10), // "RB5connect"
QT_MOC_LITERAL(1, 11, 17), // "RB5_CMD_Connected"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 20), // "RB5_CMD_Disconnected"
QT_MOC_LITERAL(4, 51, 18), // "RB5_DATA_Connected"
QT_MOC_LITERAL(5, 70, 21), // "RB5_DATA_Disconnected"
QT_MOC_LITERAL(6, 92, 12), // "CMDConnected"
QT_MOC_LITERAL(7, 105, 15), // "CMDDisconnected"
QT_MOC_LITERAL(8, 121, 13), // "DATAConnected"
QT_MOC_LITERAL(9, 135, 16), // "DATADisconnected"
QT_MOC_LITERAL(10, 152, 7), // "CMDread"
QT_MOC_LITERAL(11, 160, 8), // "DATAread"
QT_MOC_LITERAL(12, 169, 11), // "DATArequest"
QT_MOC_LITERAL(13, 181, 14) // "onReadyCmdRead"

    },
    "RB5connect\0RB5_CMD_Connected\0\0"
    "RB5_CMD_Disconnected\0RB5_DATA_Connected\0"
    "RB5_DATA_Disconnected\0CMDConnected\0"
    "CMDDisconnected\0DATAConnected\0"
    "DATADisconnected\0CMDread\0DATAread\0"
    "DATArequest\0onReadyCmdRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RB5connect[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    0,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   78,    2, 0x09 /* Protected */,
       7,    0,   79,    2, 0x09 /* Protected */,
       8,    0,   80,    2, 0x09 /* Protected */,
       9,    0,   81,    2, 0x09 /* Protected */,
      10,    0,   82,    2, 0x09 /* Protected */,
      11,    0,   83,    2, 0x09 /* Protected */,
      12,    0,   84,    2, 0x09 /* Protected */,
      13,    0,   85,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

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

void RB5connect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RB5connect *_t = static_cast<RB5connect *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RB5_CMD_Connected(); break;
        case 1: _t->RB5_CMD_Disconnected(); break;
        case 2: _t->RB5_DATA_Connected(); break;
        case 3: _t->RB5_DATA_Disconnected(); break;
        case 4: _t->CMDConnected(); break;
        case 5: _t->CMDDisconnected(); break;
        case 6: _t->DATAConnected(); break;
        case 7: _t->DATADisconnected(); break;
        case 8: _t->CMDread(); break;
        case 9: _t->DATAread(); break;
        case 10: _t->DATArequest(); break;
        case 11: _t->onReadyCmdRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RB5connect::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RB5connect::RB5_CMD_Connected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RB5connect::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RB5connect::RB5_CMD_Disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RB5connect::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RB5connect::RB5_DATA_Connected)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RB5connect::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RB5connect::RB5_DATA_Disconnected)) {
                *result = 3;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject RB5connect::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RB5connect.data,
      qt_meta_data_RB5connect,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RB5connect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RB5connect::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RB5connect.stringdata0))
        return static_cast<void*>(const_cast< RB5connect*>(this));
    return QObject::qt_metacast(_clname);
}

int RB5connect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void RB5connect::RB5_CMD_Connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void RB5connect::RB5_CMD_Disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void RB5connect::RB5_DATA_Connected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void RB5connect::RB5_DATA_Disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
struct qt_meta_stringdata_CMDClient_t {
    QByteArrayData data[4];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CMDClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CMDClient_t qt_meta_stringdata_CMDClient = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CMDClient"
QT_MOC_LITERAL(1, 10, 10), // "CMD_READED"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8) // "ReadData"

    },
    "CMDClient\0CMD_READED\0\0ReadData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMDClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CMDClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMDClient *_t = static_cast<CMDClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->CMD_READED(); break;
        case 1: _t->ReadData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CMDClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CMDClient::CMD_READED)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CMDClient::staticMetaObject = {
    { &RBTCPClient::staticMetaObject, qt_meta_stringdata_CMDClient.data,
      qt_meta_data_CMDClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CMDClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMDClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CMDClient.stringdata0))
        return static_cast<void*>(const_cast< CMDClient*>(this));
    return RBTCPClient::qt_metacast(_clname);
}

int CMDClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RBTCPClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CMDClient::CMD_READED()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_DATAClient_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DATAClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DATAClient_t qt_meta_stringdata_DATAClient = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DATAClient"
QT_MOC_LITERAL(1, 11, 11), // "DATA_READED"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 8) // "ReadData"

    },
    "DATAClient\0DATA_READED\0\0ReadData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DATAClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void DATAClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DATAClient *_t = static_cast<DATAClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DATA_READED(); break;
        case 1: _t->ReadData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DATAClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DATAClient::DATA_READED)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject DATAClient::staticMetaObject = {
    { &RBTCPClient::staticMetaObject, qt_meta_stringdata_DATAClient.data,
      qt_meta_data_DATAClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DATAClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DATAClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DATAClient.stringdata0))
        return static_cast<void*>(const_cast< DATAClient*>(this));
    return RBTCPClient::qt_metacast(_clname);
}

int DATAClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RBTCPClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DATAClient::DATA_READED()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
