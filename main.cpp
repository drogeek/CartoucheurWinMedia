#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QtCore>
#include <QSocketNotifier>
#include <QQmlContext>
#include <QTcpServer>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSharedPointer>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <fstream>
#include "panelmodel.h"
#include "cartridgemodel.h"
#include "ramiProtocol.h"
#include "connection.h"
#include "clientnotifier.h"
#define __WINMEDIA_DEBUG

enum class Machine{
    NAME,ADRESS,PORT
};

typedef unsigned int uint;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QString serverAdress = "127.0.0.1";
    qint32 serverPort = 1337;
    QString LOCALIP = "127.0.0.1";
    qint32 LOCALPORT = 1234;

    QDomDocument doc("settings");
    QFile file("settings.conf");
    if(!file.open(QIODevice::ReadWrite))
        qDebug() << "settings.conf not found";
    if(!doc.setContent(&file)){
        qDebug() << "malformed settings.conf";
    }
    file.close();

    auto machines = doc.elementsByTagName("machine");
    for(int i=0; i<machines.size(); i++){
        QDomElement elem = machines.item(i).toElement();
        auto children = elem.childNodes();
        if(children.item((uint)Machine::NAME).toElement().text() == "delegate"){
            serverAdress = children.item((uint)Machine::ADRESS).toElement().text();
            serverPort = children.item((uint)Machine::PORT).toElement().text().toInt();
        }
    }



    QTcpServer server;
    server.listen(QHostAddress(LOCALIP),LOCALPORT);
    qDebug() << server.errorString();


#ifdef __WINMEDIA_DEBUG
    //Test the encryption and decryption protocole for RAMI cartridge
    RamiProtocol::Params params;
    params.column=1;
    params.row=6;
    params.state=0;
    RamiProtocol::Data data=RamiProtocol::encrypt(params);
    RamiProtocol::print(data);
    auto result = RamiProtocol::decrypt(data);
    RamiProtocol::print(result);
#endif

    QQmlApplicationEngine engine;

    /* Notifier */
    ClientNotifier notifier;
    qDebug() << &notifier;

    QSharedPointer<QTcpSocket> notifierSock(new QTcpSocket());
    notifierSock->connectToHost(QHostAddress(serverAdress),serverPort);
    QObject::connect(&(*notifierSock),&QTcpSocket::connected,[&notifier,notifierSock,&engine](){
        notifier.setSocket(notifierSock);
        engine.rootContext()->setContextProperty("Notifier", &notifier);
    });

    /* Rami server*/
    Connection connection;
    QObject::connect(&server,&QTcpServer::newConnection,[&server,&connection,&engine](){
        qDebug() << "New connection";
        QSharedPointer<QTcpSocket> socket(server.nextPendingConnection());
        connection.setSocket(socket);
        QObject::connect(&(*socket),&QTcpSocket::readyRead, &connection, &Connection::receive);
        //TODO: handle the disconnection
        QObject::connect(&(*socket),&QTcpSocket::disconnected, &connection, &Connection::disconnect);
    });

    qmlRegisterType<CartridgeModel>("org.winmedia.guiennet",1,0,"CartridgeModel");
    qmlRegisterType<PanelModel>("org.winmedia.guiennet",1,0,"PanelModel");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.rootContext()->setContextProperty("Connection", &connection);
    QObject* rootObject=engine.rootObjects()[0];
    QObject::connect(rootObject,SIGNAL(playerCommand(int,int,bool)),&connection,SLOT(send(int,int,bool)));

    return app.exec();
}
