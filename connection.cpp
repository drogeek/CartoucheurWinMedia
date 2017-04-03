#include "connection.h"

Connection::Connection(){
    emit disconnected();
}

//TODO: verify errors?
void Connection::send(int row, int column, bool state){
    RamiProtocol::Params params;
    params.column=column;
    params.row=row;
    params.state=state;
    auto toSend = RamiProtocol::encrypt(params);
    m_sock->write((char *)&toSend);
    qDebug() << "Command sent";
}

void Connection::receive(){
    QVariantMap result;
    QByteArray str = m_sock->readAll();
    for(auto it = str.begin(); it != str.end(); it+=3){
        std::string subStr;
        subStr+=*it;
        subStr+=*(it+1);
        subStr+=*(it+2);
        auto tmp = RamiProtocol::decrypt(subStr);
        result["column"]=tmp.column;
        result["row"]=tmp.row;
        result["state"]=tmp.state;
        qDebug() << "Command received";
        emit commandReceived(result);
    }
}

void Connection::setSocket(QSharedPointer<QTcpSocket> socket){
    m_sock = socket;
    emit connected();
}

void Connection::disconnect(){
    qDebug() << "disconnected";
    m_sock->disconnectFromHost();
//    m_sock.clear();
    emit disconnected();
}
