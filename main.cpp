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
#include <QTimer>
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

void connectToDelegate(QSharedPointer<QTcpServer> server, ClientNotifier* notifier, OptionsXML* options, QTimer* keepAlive);
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

//    QString LOCALIP = "127.0.0.1";

    QCoreApplication::setOrganizationName("WinMedia");
    QCoreApplication::setOrganizationDomain("winmedia.org");
    QCoreApplication::setApplicationName("Cartridge");
    OptionsXML options;

    ClientNotifier notifier;
    StateKeeper statekeeper;
    QTimer keepAlive;
    QQmlApplicationEngine engine;

    QSharedPointer<QTcpServer> server(new QTcpServer());
    connectToDelegate(server, &notifier, &options, &keepAlive);
    QObject::connect(&options,&OptionsXML::configChanged,[&options,&server,&notifier,&keepAlive](){
        if (server->isListening()){
            notifier.disconnect();
            server->close();
            server = QSharedPointer<QTcpServer>(new QTcpServer());
        }
        connectToDelegate(server, &notifier, &options, &keepAlive);
    });

    qmlRegisterType<CartridgeModel>("org.winmedia.guiennet",1,0,"CartridgeModel");
    qmlRegisterType<PanelModel>("org.winmedia.guiennet",1,0,"PanelModel");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.rootContext()->setContextProperty("Options", &options);
    engine.rootContext()->setContextProperty("Notifier", &notifier);
    engine.rootContext()->setContextProperty("StateKeeper", &statekeeper);

    return app.exec();
}

void connectToDelegate(QSharedPointer<QTcpServer> server, ClientNotifier* notifier, OptionsXML* options, QTimer* keepAlive){
    bool serverResp = server->listen(QHostAddress(options->getIp()),options->getPort());
    const uint TIMEOUT = 5000;
    if(serverResp){
        QObject::connect(&(*server),&QTcpServer::newConnection,[server,notifier,keepAlive](){
            qDebug() << "New connection";
            QSharedPointer<QTcpSocket> socket(server->nextPendingConnection());
            notifier->setSocket(socket);
            auto startKeepAliveTimer = [keepAlive](){
                keepAlive->start(TIMEOUT);
            };
            QObject::connect(&(*socket),&QTcpSocket::connected, startKeepAliveTimer);
            QObject::connect(&(*socket),&QTcpSocket::readyRead, startKeepAliveTimer);
            QObject::connect(keepAlive, &QTimer::timeout, [notifier](){
                //we send an heartbeat to notify we are still connected
                notifier->send("","","");
            });
            QObject::connect(&(*socket),&QTcpSocket::disconnected, notifier, &ClientNotifier::disconnect);
        });

    }
    else
        qDebug() << server->errorString();
}
