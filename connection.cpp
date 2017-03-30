#include "connection.h"

Connection::Connection(std::string ip = "127.0.0.1", int port=1234){
#if defined (WIN32)
   WSADATA WSAData;
   WSAStartup(MAKEWORD(2,2), &WSAData);
#endif

   int sock_err;

   m_sin.sin_addr.s_addr = inet_addr(ip.c_str());
   m_sin.sin_family = AF_INET;
   m_sin.sin_port = htons(port);

   m_sock = socket(AF_INET, SOCK_STREAM, 0);
   if(m_sock != INVALID_SOCKET){
        sock_err=bind(m_sock,(SOCKADDR*)&m_sin,m_sizeSin);
        qDebug() << "Binding";
        if(sock_err != SOCKET_ERROR){
            sock_err=listen(m_sock,1);
            qDebug() << "Listening";
            if(sock_err != SOCKET_ERROR){
                qDebug() << "Waiting for client";
                m_csock = accept(m_sock,(SOCKADDR*)&m_csin,&m_sizeCsin);
                qDebug() << "Client connected";
            }
            else
                qDebug() << "Listening failed";
        }
        else
            qDebug() << "Binding failed";

   }
   else
        qDebug() << "Socket opening failed";
}

int Connection::send(int row, int column, bool state){
    RamiProtocol::Params params;
    int socket_err;
    params.column=column;
    params.row=row;
    params.state=state;
    auto toSend = RamiProtocol::encrypt(params);
    socket_err = ::send(m_csock,(char *)&toSend,sizeof(toSend),0);
    if(socket_err == SOCKET_ERROR){

#ifdef _WIN32
        socket_err=WSAGetLastError();
#endif
        qDebug() << "Problem sending socket";
        qDebug() << "error code:" << socket_err;
    }
    return socket_err;
}

int Connection::send(Params params){
    return Connection::send(params.row,params.column,params.state);
}

Connection::Params Connection::receive(){
    Params result;
    int socket_err;
    socket_err = ::recv(m_sock,(char *)&result, sizeof(result),0);
    if(socket_err == SOCKET_ERROR){
#ifdef _WIN32
        socket_err = WSAGetLastError();
#endif
        qDebug() << "Problem receiving socket";
        qDebug() << "error code:" << socket_err;
    }
//    qDebug() << "received:" << result;
    return result;
}

Connection::~Connection(){
#if defined (WIN32)
    WSACleanup();
#endif
}
