#ifndef CONNECTION_H
#define CONNECTION_H

#ifdef _WIN32

    #include <winsock2.h>
    typedef int socklen_t;

#elif __linux__

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>

    // Define, qui nous serviront par la suite
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close (s)

    // De même
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;

#endif

#include <iostream>
#include <string>
#include <QObject>
#include <QDebug>
#include "ramiProtocol.h"


class Connection : public QObject
{
    typedef struct{
        int row;
        int column;
        bool state;
    }Params;

    Q_OBJECT
public:
    Connection(std::string ip, int port);
    ~Connection();
    int send(Params params);
    Params receive();

public slots:
    int send(int row, int column, bool state);
private:
    SOCKET m_sock;
    SOCKADDR_IN m_sin;
    int m_sizeSin = sizeof(m_sin);

    SOCKET m_csock;
    SOCKADDR_IN m_csin;
    int m_sizeCsin = sizeof(m_csin);
};

#endif // CONNECTION_H
