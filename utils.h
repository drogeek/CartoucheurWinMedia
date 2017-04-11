#ifndef UTILS_H
#define UTILS_H
#include <QString>

class Utils
{
public:
    static Utils& getInstance();
    void setServerIp(QString ip){ m_server_ip = ip;}
    void setServerPort(quint32 port){ m_server_port = port;}
    void setWinmediaIp(QString ip){ m_winmedia_ip = ip;}
    void setWinmediaPort(qint32 port){ m_winmedia_port = port;}
    QString getServerIp(){ return m_server_ip; }
    qint32 getServerPort(){ return m_server_port; }
    QString getWinmediaIp(){ return m_winmedia_ip; }
    qint32 getWinmediaPort(){ return m_winmedia_port; }

private:
    Utils();
    static Utils m_instance;

    QString m_server_ip;
    qint32 m_server_port;
    QString m_winmedia_ip;
    qint32 m_winmedia_port;
};

#endif // UTILS_H
