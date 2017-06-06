#include "optionsxml.h"

const QString OptionsXML::APPPORT = "app/port";
const QString OptionsXML::APPIP = "app/ip";
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
    setIp(m_options.value(OptionsXML::APPIP).toString());
}

void OptionsXML::persistConfig(){
    m_options.setValue(OptionsXML::APPPORT,m_port);
    m_options.setValue(OptionsXML::APPIP,m_ip);
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
    if(!res.contains(m_ip))
        res.prepend(m_ip);
    else{
        res.swap(0,res.indexOf(m_ip));
    }

    return res;
}
