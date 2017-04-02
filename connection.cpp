#include "connection.h"

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
    auto tmp = (RamiProtocol::decrypt(str.constData()));
    result["column"]=tmp.column;
    result["row"]=tmp.row;
    result["state"]=tmp.state;
    qDebug() << "Command received";
    emit commandReceived(result);
}
