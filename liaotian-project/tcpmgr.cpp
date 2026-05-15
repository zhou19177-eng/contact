#include "tcpmgr.h"
#include <QAbstractSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include "usermgr.h"
TcpMgr::TcpMgr():_host(""),_port(0),_b_recv_pending(false),_message_id(0),_message_len(0)
{

    QObject::connect(&_socket, &QTcpSocket::connected, [&]() {
        qDebug() << "Connected to server!";
        // 连接建立后发送消息
        emit sig_con_success(true);
    });

    QObject::connect(&_socket, &QTcpSocket::readyRead, [&]() {
        // 当有数据可读时，读取所有数据
        // 读取所有数据并追加到缓冲区
        _buffer.append(_socket.readAll());

        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_0);

        forever {
            //先解析头部
            if(!_b_recv_pending){
                // 检查缓冲区中的数据是否足够解析出一个消息头（消息ID + 消息长度）
                if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2)) {
                    return; // 数据不够，等待更多数据
                }

                // 预读取消息ID和消息长度，但不从缓冲区中移除
                stream >> _message_id >> _message_len;

                //将buffer 中的前四个字节移除
                _buffer = _buffer.mid(sizeof(quint16) * 2);

                // 输出读取的数据
                qDebug() << "Message ID:" << _message_id << ", Length:" << _message_len;

            }

            //buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
            if(_buffer.size() < _message_len){
                _b_recv_pending = true;
                return;
            }

            _b_recv_pending = false;
            // 读取消息体
            QByteArray messageBody = _buffer.mid(0, _message_len);
            qDebug() << "receive body msg is " << messageBody ;
            handleMsg(ReqId(_message_id), _message_len, messageBody);

            _buffer = _buffer.mid(_message_len);
        }

    });

    //5.15 之后版本
    QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
        qDebug() << "Error:" << _socket.errorString();
    });

    // 处理错误（适用于Qt 5.15之前的版本）
    // QObject::connect(&_socket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
    //                  [&](QTcpSocket::SocketError socketError) {
    //                      qDebug() << "Error:" << _socket.errorString() ;
    //                      switch (socketError) {
    //                      case QTcpSocket::ConnectionRefusedError:
    //                          qDebug() << "Connection Refused!";
    //                          emit sig_con_success(false);
    //                          break;
    //                      case QTcpSocket::RemoteHostClosedError:
    //                          qDebug() << "Remote Host Closed Connection!";
    //                          break;
    //                      case QTcpSocket::HostNotFoundError:
    //                          qDebug() << "Host Not Found!";
    //                          emit sig_con_success(false);
    //                          break;
    //                      case QTcpSocket::SocketTimeoutError:
    //                          qDebug() << "Connection Timeout!";
    //                          emit sig_con_success(false);
    //                          break;
    //                      case QTcpSocket::NetworkError:
    //                          qDebug() << "Network Error!";
    //                          break;
    //                      default:
    //                          qDebug() << "Other Error!";
    //                          break;
    //                      }
    //                  });

    // 处理连接断开
    QObject::connect(&_socket, &QTcpSocket::disconnected, [&]() {
        qDebug() << "Disconnected from server.";
    });

    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
    initHandlers();

}
TcpMgr::~TcpMgr(){

}
// void TcpMgr::initHandlers()
// {
//     qDebug() << "还没有发出偶111";
//     //auto self = shared_from_this();
//     _handlers.insert(ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data) {
//     qDebug() << "还没有发出偶111";
//     Q_UNUSED(len);
//     qDebug() << "handle id is " << id << " data is " << data;
//     // 将QByteArray转换为QJsonDocument
//     QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
//     qDebug() << "还没有发出偶111";
//     // 检查转换是否成功
//     if(jsonDoc.isNull()){
//         qDebug() << "Failed to create QJsonDocument.";
//         return;
//     }

//     QJsonObject jsonObj = jsonDoc.object();

//     if(!jsonObj.contains("error")){
//         int err = ErrorCodes::ERR_JSON;
//         qDebug() << "Login Failed, err is Json Parse Err" << err;
//         emit sig_login_failed(err);
//         return;
//     }

//     int err = jsonObj["error"].toInt();
//     if(err != ErrorCodes::SUCCESS){
//         qDebug() << "Login Failed, err is " << err;
//         emit sig_login_failed(err);
//         return;
//     }

//     auto uid = jsonObj["uid"].toInt();
//     auto name = jsonObj["name"].toString();
//     auto nick = jsonObj["nick"].toString();
//     auto icon = jsonObj["icon"].toString();
//     auto sex = jsonObj["sex"].toInt();
//     auto desc = jsonObj["desc"].toString();
//     auto user_info = std::make_shared<UserInfo>(uid, name, nick, icon, sex,"",desc);

//     UserMgr::GetInstance()->SetUserInfo(user_info);
//     UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());
//     if(jsonObj.contains("apply_list")){
//         UserMgr::GetInstance()->AppendApplyList(jsonObj["apply_list"].toArray());
//     }

//     //添加好友列表
//     if (jsonObj.contains("friend_list")) {
//         UserMgr::GetInstance()->AppendFriendList(jsonObj["friend_list"].toArray());
//     }

//     // UserMgr::GetInstance()->SetUid(jsonObj["uid"].toInt());
//     // UserMgr::GetInstance()->SetName(jsonObj["name"].toString());
//     // UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());
//     qDebug() << "还没有发出偶111";
//     emit sig_swich_chatdlg();

// });
// }



void TcpMgr::initHandlers()
{
    qDebug() << ">>> [DEBUG] TcpMgr::initHandlers initialized";

    // 注册登录回包的处理器
    _handlers.insert(ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data) {
        qDebug() << ">>> [DEBUG] Received ID_CHAT_LOGIN_RSP. Data len:" << len;

        // 1. 解析 JSON
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug() << ">>> [ERROR] Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        // 打印完整的 JSON 内容，看看服务端到底发了什么
        qDebug() << ">>> [DEBUG] JSON Content:" << QString(jsonDoc.toJson(QJsonDocument::Compact));

        // 2. 检查 Error 字段
        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug() << ">>> [ERROR] Login Failed, Missing 'error' field";
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug() << ">>> [ERROR] Login Failed, Server returned error:" << err;
            emit sig_login_failed(err);
            return;
        }

        // 3. 安全解析用户信息 (使用 .value 防止字段缺失)
        // 注意：确保这里的字段名和数据库/服务端发送的一致
        int uid = jsonObj.value("uid").toInt();
        QString name = jsonObj.value("name").toString();
        QString nick = jsonObj.value("nick").toString();
        QString icon = jsonObj.value("icon").toString();
        int sex = jsonObj.value("sex").toInt();
        QString desc = jsonObj.value("desc").toString();
        QString token = jsonObj.value("token").toString();

        qDebug() << ">>> [DEBUG] Parsed User: uid=" << uid << " name=" << name << " token=" << token;

        // 4. 构建 UserInfo 对象
        // 请确认 UserInfo 的构造函数参数顺序！这里假设是 (uid, name, nick, icon, sex, mobile, desc)
        // 这里的 "" 传的是 mobile 还是什么？请检查 UserInfo 定义
        auto user_info = std::make_shared<UserInfo>(uid, name, nick, icon, sex, "", desc);

        // 5. 存入单例
        UserMgr::GetInstance()->SetUserInfo(user_info);
        UserMgr::GetInstance()->SetToken(token);

        // 6. 处理列表 (防御性检查)
        if(jsonObj.contains("apply_list")){
            UserMgr::GetInstance()->AppendApplyList(jsonObj["apply_list"].toArray());
        }

        if (jsonObj.contains("friend_list")) {
            UserMgr::GetInstance()->AppendFriendList(jsonObj["friend_list"].toArray());
        }

        qDebug() << ">>> [DEBUG] Everything OK. Emitting sig_swich_chatdlg...";
        emit sig_swich_chatdlg();
    });

    _handlers.insert(ID_SEARCH_USER_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug()<< "handle id is "<< id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if(jsonDoc.isNull()){
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err ;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug() << "Login Failed, err is " << err ;
            emit sig_login_failed(err);
            return;
        }

        auto search_info = std::make_shared<SearchInfo>(jsonObj["uid"].toInt(),
                                                        jsonObj["name"].toString(), jsonObj["nick"].toString(),
                                                        jsonObj["desc"].toString(), jsonObj["sex"].toInt(), jsonObj["icon"].toString());

        emit sig_user_search(search_info);
    });

    _handlers.insert(ID_ADD_FRIEND_RSP, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Add Friend Failed, err is Json Parse Err" << err;
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Add Friend Failed, err is " << err;
            return;
        }

        qDebug() << "Add Friend Success " ;
    });

    _handlers.insert(ID_NOTIFY_ADD_FRIEND_REQ, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err;

            emit sig_user_search(nullptr);
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Login Failed, err is " << err;
            emit sig_user_search(nullptr);
            return;
        }

        int from_uid = jsonObj["applyuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString desc = jsonObj["desc"].toString();
        QString icon = jsonObj["icon"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();

        auto apply_info = std::make_shared<AddFriendApply>(
            from_uid, name, desc,
            icon, nick, sex);

        emit sig_friend_apply(apply_info);
    });

    _handlers.insert(ID_NOTIFY_AUTH_FRIEND_REQ, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Auth Friend Failed, err is " << err;
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Auth Friend Failed, err is " << err;
            return;
        }

        int from_uid = jsonObj["fromuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString nick = jsonObj["nick"].toString();
        QString icon = jsonObj["icon"].toString();
        int sex = jsonObj["sex"].toInt();

        auto auth_info = std::make_shared<AuthInfo>(from_uid,name,
                                                    nick, icon, sex);

        emit sig_add_auth_friend(auth_info);
    });

    _handlers.insert(ID_AUTH_FRIEND_RSP, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Auth Friend Failed, err is Json Parse Err" << err;
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Auth Friend Failed, err is " << err;
            return;
        }

        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();
        auto uid = jsonObj["uid"].toInt();
        auto rsp = std::make_shared<AuthRsp>(uid, name, nick, icon, sex);
        emit sig_auth_rsp(rsp);

        qDebug() << "Auth Friend Success " ;
    });
    _handlers.insert(ID_NOTIFY_TEXT_CHAT_MSG_REQ, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Notify Chat Msg Failed, err is Json Parse Err" << err;
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Notify Chat Msg Failed, err is " << err;
            return;
        }

        qDebug() << "Receive Text Chat Notify Success " ;
        auto msg_ptr = std::make_shared<TextChatMsg>(jsonObj["fromuid"].toInt(),
                                                     jsonObj["touid"].toInt(),jsonObj["text_array"].toArray());
        emit sig_text_chat_msg(msg_ptr);
    });

}



void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto find_iter =  _handlers.find(id);
    if(find_iter == _handlers.end()){
        qDebug()<< "not found id ["<< id << "] to handle";
        return ;
    }

    find_iter.value()(id,len,data);
}
void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug()<< "receive tcp connect signal";
    // 尝试连接到服务器
    qDebug() << "Connecting to server...";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toUInt());
    _socket.connectToHost(si.Host, _port);
}
void TcpMgr::slot_send_data(ReqId reqId, QByteArray dataBytes)
{
    uint16_t id = reqId;

    // 计算长度（使用网络字节序转换）
    quint16 len = static_cast<quint16>(dataBytes.length());

    // 创建一个QByteArray用于存储要发送的所有数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    // 设置数据流使用网络字节序
    out.setByteOrder(QDataStream::BigEndian);

    // 写入ID和长度
    out << id << len;

    // 添加字符串数据
    block.append(dataBytes);

    // 发送数据
    _socket.write(block);
    qDebug() << "tcp mgr send byte data is " << block ;
}
