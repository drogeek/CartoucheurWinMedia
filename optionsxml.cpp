#include "optionsxml.h"

const QString OptionsXML::APPPORT = "app/port";
<<<<<<< HEAD
const QString OptionsXML::APPIP = "app/ip";
=======
>>>>>>> b7753a0d111129fe449d0a13add242eb4a5148ba
OptionsXML::OptionsXML()
{
    readConfig();
}

void OptionsXML::readConfig(){
    bool ok;
    qint16 port = m_options.value(OptionsXML::APPPORT).toInt(&ok);
    if(ok){
        setPort(port);
    }
    else
        qDebug() << "the port should be an integer";
<<<<<<< HEAD
    setIp(m_options.value(OptionsXML::APPIP).toString());
=======
>>>>>>> b7753a0d111129fe449d0a13add242eb4a5148ba
}

void OptionsXML::persistConfig(){
    m_options.setValue(OptionsXML::APPPORT,m_port);
<<<<<<< HEAD
    m_options.setValue(OptionsXML::APPIP,m_ip);
=======
>>>>>>> b7753a0d111129fe449d0a13add242eb4a5148ba
    emit configChanged();
}

QList<QString> OptionsXML::getLocalIps(){
    QList<QString> res;
    for(auto interface : QNetworkInterface::allInterfaces()){
        if(!interface.flags().testFlag(QNetworkInterface::IsLoopBack) &&
                interface.flags().testFlag(QNetworkInterface::IsUp)){
            for(auto networkAdressEntry : interface.addressEntries()){
                auto ip = networkAdressEntry.ip();
                if(ip.protocol() == QAbstractSocket::IPv4Protocol){
                    res << ip.toString();
                }
            }
        }
    }
<<<<<<< HEAD
    if(!res.contains(m_ip))
        res.prepend(m_ip);
    else{
        res.swap(0,res.indexOf(m_ip));
    }

=======
>>>>>>> b7753a0d111129fe449d0a13add242eb4a5148ba
    return res;
}
