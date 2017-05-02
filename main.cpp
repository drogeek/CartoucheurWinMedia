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
#define __WINMEDIA_DEBUG

typedef unsigned int uint;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QString LOCALIP = "127.0.0.1";

    QCoreApplication::setOrganizationName("WinMedia");
    QCoreApplication::setOrganizationDomain("winmedia.org");
    QCoreApplication::setApplicationName("Cartridge");
    OptionsXML options;

    for(auto interface : QNetworkInterface::allInterfaces()){
        if(!interface.flags().testFlag(QNetworkInterface::IsLoopBack) &&
                interface.flags().testFlag(QNetworkInterface::IsUp)){
            for(auto networkAdressEntry : interface.addressEntries()){
                auto ip = networkAdressEntry.ip();
                if(ip.protocol() == QAbstractSocket::IPv4Protocol){
                    qDebug() << ip;
                }
            }
        }
    }

    QTcpServer server;
    server.listen(QHostAddress(LOCALIP),options.getPort());
    QObject::connect(&options,&OptionsXML::portChanged,[&LOCALIP,&options,&server](){
        server.listen(QHostAddress(LOCALIP),options.getPort());
    });
    qDebug() << server.errorString();

    QQmlApplicationEngine engine;
    ClientNotifier notifier;

    QObject::connect(&server,&QTcpServer::newConnection,[&server,&notifier,&engine](){
        qDebug() << "New connection";
        QSharedPointer<QTcpSocket> socket(server.nextPendingConnection());
        notifier.setSocket(socket);
        QObject::connect(&(*socket),&QTcpSocket::disconnected, [&notifier](){
            notifier.setConnected(false);
        });
        engine.rootContext()->setContextProperty("Notifier", &notifier);
    });

    qmlRegisterType<CartridgeModel>("org.winmedia.guiennet",1,0,"CartridgeModel");
    qmlRegisterType<PanelModel>("org.winmedia.guiennet",1,0,"PanelModel");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.rootContext()->setContextProperty("Options", &options);
    QObject* rootObject=engine.rootObjects()[0];
//    QObject::connect(rootObject,SIGNAL(playerCommand(int,int,bool)),&notifier,SLOT(sendRami(int,int,bool)));

    return app.exec();
}
