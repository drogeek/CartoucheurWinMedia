#include "abstractnotifier.h"


const QString AbstractNotifier::CMDFILE = "WINMEDIA.TMP";

const QString AbstractNotifier::NOTIFY = "NOTIFY";
const QString AbstractNotifier::DB = "DB";
const QString AbstractNotifier::JSON_TYPE = "type";
const QString AbstractNotifier::JSON_DATA = "data";

const AbstractNotifier::NotifType AbstractNotifier::notifType;

AbstractNotifier::AbstractNotifier(){
    connect(this, &AbstractNotifier::newNotification,this,&AbstractNotifier::notification);
    connect(this,&AbstractNotifier::newQuery,this,&AbstractNotifier::query);
}

AbstractNotifier::AbstractNotifier(QSharedPointer<QTcpSocket> sock)
    : AbstractNotifier()
{
    setSocket(sock);
}

void AbstractNotifier::setSocket(QSharedPointer<QTcpSocket> sock){
    m_sock = sock;
    connect(&(*m_sock),&QTcpSocket::connected,[](){ qDebug() << "Notifier connected"; });
    connect(&(*m_sock),&QTcpSocket::readyRead,this,&AbstractNotifier::parse);
}

void AbstractNotifier::parse(){
    QByteArray arr=m_sock->readAll();
    auto jsonDoc = QJsonDocument::fromJson(arr);
    auto jsonObj = jsonDoc.object();
    if(jsonObj[JSON_TYPE].toString() == NOTIFY){
        emit newNotification(jsonObj[JSON_DATA]);
    }
    else if(jsonObj[JSON_TYPE].toString() == DB){
        emit newQuery(jsonObj[JSON_DATA]);
    }
}

QJsonDocument AbstractNotifier::wrapWithType(QJsonValue data,QString type){
    QJsonDocument result;
    QJsonObject wrapper;
    wrapper.insert(JSON_TYPE,type);
    wrapper.insert(JSON_DATA,data);
    result.setObject(wrapper);
    return result;
}

void AbstractNotifier::send(QJsonValue value, QString type){
    auto result = wrapWithType(value.toArray(),type);
    m_sock->write(result.toJson());
}
