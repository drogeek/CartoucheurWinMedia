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
        m_port = port;
        emit portChanged();
    }
    else
        qDebug() << "the port should be an integer";
}

void OptionsXML::persistConfig(){
    m_options.setValue(OptionsXML::APPPORT,m_port);
    emit portChanged();
}
