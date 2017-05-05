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

void connectToWin(QSharedPointer<QTcpServer> server, ClientNotifier* notifier, OptionsXML* options);
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QString LOCALIP = "127.0.0.1";

    QCoreApplication::setOrganizationName("WinMedia");
    QCoreApplication::setOrganizationDomain("winmedia.org");
    QCoreApplication::setApplicationName("Cartridge");
    OptionsXML options;

    ClientNotifier notifier;
    QQmlApplicationEngine engine;

    QSharedPointer<QTcpServer> server(new QTcpServer());
    connectToWin(server, &notifier, &options);
    QObject::connect(&options,&OptionsXML::configChanged,[&LOCALIP,&options,&server,&notifier](){
        if (server->isListening()){
            notifier.disconnect();
            server->close();
            server = QSharedPointer<QTcpServer>(new QTcpServer());
        }
        connectToWin(server, &notifier, &options);
    });

    qmlRegisterType<CartridgeModel>("org.winmedia.guiennet",1,0,"CartridgeModel");
    qmlRegisterType<PanelModel>("org.winmedia.guiennet",1,0,"PanelModel");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.rootContext()->setContextProperty("Options", &options);
    engine.rootContext()->setContextProperty("Notifier", &notifier);

    return app.exec();
}


void connectToWin(QSharedPointer<QTcpServer> server, ClientNotifier* notifier, OptionsXML* options){
    bool serverResp = server->listen(QHostAddress("127.0.0.1"),options->getPort());
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
