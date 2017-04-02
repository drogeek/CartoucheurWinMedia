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
    Connection(){ qDebug() << "Connection object created"; }
    void receive();
    void setSocket(QTcpSocket* socket) { m_sock = socket; }

public slots:
    void send(int row, int column, bool state);
signals:
    void commandReceived(QVariantMap params);
private:
    QTcpSocket* m_sock;
};

#endif // CONNECTION_H
