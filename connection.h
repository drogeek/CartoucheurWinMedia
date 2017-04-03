#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include "ramiProtocol.h"

class Connection : public QObject
{

    Q_OBJECT
public:
    Connection();
    void receive();
    void setSocket(QSharedPointer<QTcpSocket> socket);

public slots:
    void send(int row, int column, bool state);
signals:
    void commandReceived(QVariantMap params);
private:
    QSharedPointer<QTcpSocket> m_sock;
};

#endif // CONNECTION_H
