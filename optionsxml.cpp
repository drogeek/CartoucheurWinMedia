#include "optionsxml.h"

const QString OptionsXML::APPPORT = "app/port";
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
}

void OptionsXML::persistConfig(){
    m_options.setValue(OptionsXML::APPPORT,m_port);
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
    return res;
}
