#include "optionsxml.h"

const QString OptionsXML::m_fileName = "settings.conf";
OptionsXML::OptionsXML()
{
    QDomDocument m_doc(m_fileName.split('.').at(0));
    OptionsXML::readFromFile();
}

void OptionsXML::readFromFile(){
    qDebug() << QString("reading from %1").arg(m_fileName);
    QFile file(OptionsXML::m_fileName);
    if(!file.open(QIODevice::ReadWrite))
        qDebug() << file.errorString();
    if(!m_doc.setContent(&file)){
        qDebug() << "malformed settings.conf";
    }
    file.close();

    auto machines = m_doc.elementsByTagName("machine");
    for(int i=0; i<machines.size(); i++){
        QDomElement elem = machines.item(i).toElement();
        QDomElement delegateTest = elem.elementsByTagName("name").at(0).toElement();
        if(delegateTest.text() == "delegate"){
            QDomElement currentSib = delegateTest.nextSibling().toElement();
            while(!currentSib.isNull()){
                if(currentSib.tagName() == "ip"){
                    OptionsXML::setServerIp(currentSib.text());
                }
                else if(currentSib.tagName() == "port"){
                    OptionsXML::setServerPort(currentSib.text().toInt());
                }
                currentSib = currentSib.nextSibling().toElement();
            }
        }
    }
    emit configChanged();
}

void OptionsXML::writeToFile(){
    qDebug() << QString("writting to %1").arg(m_fileName);
    qDebug() << "server ip:" << m_serverIp;
    qDebug() << "server port:" << m_serverPort;
    QFile file(OptionsXML::m_fileName);
    if(!file.open(QIODevice::WriteOnly))
        qDebug() << file.errorString();
    auto machines = m_doc.elementsByTagName("machine");
    for(int i=0; i<machines.size(); i++){
        QDomElement elem = machines.item(i).toElement();
        QDomElement delegateTest = elem.elementsByTagName("name").at(0).toElement();
        if(delegateTest.text() == "delegate"){
            QDomElement currentSib = delegateTest.nextSibling().toElement();
            while(!currentSib.isNull()){
                if(currentSib.tagName() == "ip"){
                    currentSib.firstChild().setNodeValue(OptionsXML::serverIp());
                }
                else if(currentSib.tagName() == "port"){
                    currentSib.firstChild().setNodeValue(QString("%1").arg(OptionsXML::serverPort()));
                }
                currentSib = currentSib.nextSibling().toElement();
            }
        }
    }
    QTextStream stream(&file);
    stream << m_doc.toString();
    file.close();
    emit configChanged();

}
