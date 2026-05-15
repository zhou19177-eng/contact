/****************************************************************************
** Meta object code from reading C++ file 'chatdialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../chatdialog.h"
#include <QtNetwork/QSslError>
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10ChatDialogE_t {};
} // unnamed namespace

template <> constexpr inline auto ChatDialog::qt_create_metaobjectdata<qt_meta_tag_ZN10ChatDialogE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChatDialog",
        "slot_loading_chat_user",
        "",
        "slot_side_chat",
        "slot_side_contact",
        "slot_text_changed",
        "str",
        "slot_loading_contact_user",
        "slot_switch_apply_friend_page",
        "slot_friend_info_page",
        "std::shared_ptr<UserInfo>",
        "user_info",
        "slot_apply_friend",
        "std::shared_ptr<AddFriendApply>",
        "apply",
        "slot_add_auth_friend",
        "std::shared_ptr<AuthInfo>",
        "auth_info",
        "slot_auth_rsp",
        "std::shared_ptr<AuthRsp>",
        "auth_rsp",
        "slot_jump_chat_item",
        "std::shared_ptr<SearchInfo>",
        "si",
        "SetSelectChatItem",
        "uid",
        "SetSelectChatPage",
        "slot_jump_chat_item_from_infopage",
        "ui",
        "slot_item_clicked",
        "QListWidgetItem*",
        "item",
        "slot_text_chat_msg",
        "std::shared_ptr<TextChatMsg>",
        "msg",
        "slot_append_send_chat_msg",
        "std::shared_ptr<TextChatData>"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'slot_loading_chat_user'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_side_chat'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_side_contact'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_text_changed'
        QtMocHelpers::SlotData<void(const QString &)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Slot 'slot_loading_contact_user'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_switch_apply_friend_page'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_friend_info_page'
        QtMocHelpers::SlotData<void(std::shared_ptr<UserInfo>)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'slot_apply_friend'
        QtMocHelpers::SlotData<void(std::shared_ptr<AddFriendApply>)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 13, 14 },
        }}),
        // Slot 'slot_add_auth_friend'
        QtMocHelpers::SlotData<void(std::shared_ptr<AuthInfo>)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Slot 'slot_auth_rsp'
        QtMocHelpers::SlotData<void(std::shared_ptr<AuthRsp>)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 19, 20 },
        }}),
        // Slot 'slot_jump_chat_item'
        QtMocHelpers::SlotData<void(std::shared_ptr<SearchInfo>)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 23 },
        }}),
        // Slot 'SetSelectChatItem'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 25 },
        }}),
        // Slot 'SetSelectChatItem'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'SetSelectChatPage'
        QtMocHelpers::SlotData<void(int)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 25 },
        }}),
        // Slot 'SetSelectChatPage'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'slot_jump_chat_item_from_infopage'
        QtMocHelpers::SlotData<void(std::shared_ptr<UserInfo>)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 28 },
        }}),
        // Slot 'slot_item_clicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 30, 31 },
        }}),
        // Slot 'slot_text_chat_msg'
        QtMocHelpers::SlotData<void(std::shared_ptr<TextChatMsg>)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 33, 34 },
        }}),
        // Slot 'slot_append_send_chat_msg'
        QtMocHelpers::SlotData<void(std::shared_ptr<TextChatData>)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 2 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChatDialog, qt_meta_tag_ZN10ChatDialogE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChatDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChatDialogE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChatDialogE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10ChatDialogE_t>.metaTypes,
    nullptr
} };

void ChatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChatDialog *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->slot_loading_chat_user(); break;
        case 1: _t->slot_side_chat(); break;
        case 2: _t->slot_side_contact(); break;
        case 3: _t->slot_text_changed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->slot_loading_contact_user(); break;
        case 5: _t->slot_switch_apply_friend_page(); break;
        case 6: _t->slot_friend_info_page((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<UserInfo>>>(_a[1]))); break;
        case 7: _t->slot_apply_friend((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<AddFriendApply>>>(_a[1]))); break;
        case 8: _t->slot_add_auth_friend((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<AuthInfo>>>(_a[1]))); break;
        case 9: _t->slot_auth_rsp((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<AuthRsp>>>(_a[1]))); break;
        case 10: _t->slot_jump_chat_item((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<SearchInfo>>>(_a[1]))); break;
        case 11: _t->SetSelectChatItem((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->SetSelectChatItem(); break;
        case 13: _t->SetSelectChatPage((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->SetSelectChatPage(); break;
        case 15: _t->slot_jump_chat_item_from_infopage((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<UserInfo>>>(_a[1]))); break;
        case 16: _t->slot_item_clicked((*reinterpret_cast<std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 17: _t->slot_text_chat_msg((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<TextChatMsg>>>(_a[1]))); break;
        case 18: _t->slot_append_send_chat_msg((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<TextChatData>>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *ChatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChatDialogE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ChatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 19;
    }
    return _id;
}
QT_WARNING_POP
