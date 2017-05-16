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
#include <QNetworkInterface>
#include <fstream>
#include "panelmodel.h"
#include "cartridgemodel.h"
#include "ramiProtocol.h"
#include "connection.h"
#include "clientnotifier.h"
#include "optionsxml.h"
#include "statekeeper.h"
#define __WINMEDIA_DEBUG

typedef unsigned int uint;

void connectToDelegate(QSharedPointer<QTcpServer> server, ClientNotifier* notifier, OptionsXML* options);
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

<<<<<<< HEAD
//    QString LOCALIP = "127.0.0.1";
=======
    QString LOCALIP = "127.0.0.1";
>>>>>>> b7753a0d111129fe449d0a13add242eb4a5148ba

    QCoreApplication::setOrganizationName("WinMedia");
    QCoreApplication::setOrganizationDomain("winmedia.org");
    QCoreApplication::setApplicationName("Cartridge");
    OptionsXML options;

    ClientNotifier notifier;
    StateKeeper statekeeper;
    QQmlApplicationEngine engine;

    QSharedPointer<QTcpServer> server(new QTcpServer());
    connectToDelegate(server, &notifier, &options);
<<<<<<< HEAD
    QObject::connect(&options,&OptionsXML::configChanged,[&options,&server,&notifier](){
=======
    QObject::connect(&options,&OptionsXML::configChanged,[&LOCALIP,&options,&server,&notifier](){
>>>>>>> b7753a0d111129fe449d0a13add242eb4a5148ba
        if (server->isListening()){
            notifier.disconnect();
            server->close();
            server = QSharedPointer<QTcpServer>(new QTcpServer());
        }
        connectToDelegate(server, &notifier, &options);
    });

    qmlRegisterType<CartridgeModel>("org.winmedia.guiennet",1,0,"CartridgeModel");
    qmlRegisterType<PanelModel>("org.winmedia.guiennet",1,0,"PanelModel");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.rootContext()->setContextProperty("Options", &options);
    engine.rootContext()->setContextProperty("Notifier", &notifier);
    engine.rootContext()->setContextProperty("StateKeeper", &statekeeper);

    return app.exec();
}


void connectToDelegate(QSharedPointer<QTcpServer> server, ClientNotifier* notifier, OptionsXML* options){
<<<<<<< HEAD
    bool serverResp = server->listen(QHostAddress(options->getIp()),options->getPort());
=======
    bool serverResp = server->listen(QHostAddress("127.0.0.1"),options->getPort());
>>>>>>> b7753a0d111129fe449d0a13add242eb4a5148ba
        if(serverResp){
            QObject::connect(&(*server),&QTcpServer::newConnection,[server,notifier](){
                qDebug() << "New connection";
                QSharedPointer<QTcpSocket> socket(server->nextPendingConnection());
                notifier->setSocket(socket);
                QObject::connect(&(*socket),&QTcpSocket::disconnected, notifier, &ClientNotifier::disconnect);
            });

        }
        else
            qDebug() << server->errorString();
}
