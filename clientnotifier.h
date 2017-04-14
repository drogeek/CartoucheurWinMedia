#ifndef CLIENTNOTIFIER_H
#define CLIENTNOTIFIER_H

#include "abstractnotifier.h"

class ClientNotifier : public AbstractNotifier
{
    Q_OBJECT
public:
    ClientNotifier();
    ClientNotifier(QSharedPointer<QTcpSocket> socket);

    void notification(QString target,QJsonValue value) override;
    void query(QString target,QJsonValue value) override;
};

#endif // CLIENTNOTIFIER_H
