#include "clientnotifier.h"

ClientNotifier::ClientNotifier()
    :AbstractNotifier()
{
}

ClientNotifier::ClientNotifier(QSharedPointer<QTcpSocket> socket)
    :AbstractNotifier(socket)
{
}

void ClientNotifier::query(QString target,QJsonValue value){
    qDebug() << "query received in client notifier";
}

void ClientNotifier::notification(QString target,QJsonValue value){
    qDebug() << "notification received in client notifier";
}
