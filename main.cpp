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
#include <fstream>
#include "panelmodel.h"
#include "cartridgemodel.h"
#include "ramiProtocol.h"
#include "connection.h"
#define __WINMEDIA_DEBUG

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

#ifdef __linux__
    const QString DRIVER = "ODBC Driver 13 for SQL Server";
#elif _WIN32
    const QString DRIVER = "SQL Server";
#endif

    const QString WINMEDIA = "127.0.0.1";
    const QString WINPORT = "1337";
    const QString SERVER = "127.0.0.1";
    const QString SERVPORT = "1337";
    const qint32 LOCALPORT = 1234;
    const QString LOCALIP = "127.0.0.1";
    Utils& util = Utils::getInstance();
    util.setServerIp("127.0.0.1");
    util.setServerPort(1337);

    std::string fileStr = "\\\\";

    std::ofstream file;
    file.open(fileStr);
    if(file.is_open()){
        qDebug() << QString(fileStr.data()) << " exists";
    }
    else
        qDebug() << QString(fileStr.data()) << " doesn't exist";
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

    qmlRegisterType<CartridgeModel>("org.winmedia.guiennet",1,0,"CartridgeModel");
    qmlRegisterType<PanelModel>("org.winmedia.guiennet",1,0,"PanelModel");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    //TODO: handle multiple connections?
    Connection connection;
    QObject::connect(&server,&QTcpServer::newConnection,[&server,&connection](){
        qDebug() << "New connection";
        QSharedPointer<QTcpSocket> socket(server.nextPendingConnection());
        connection.setSocket(socket);
        QObject::connect(&(*socket),&QTcpSocket::readyRead, &connection, &Connection::receive);
        //TODO: handle the disconnection
        QObject::connect(&(*socket),&QTcpSocket::disconnected, &connection, &Connection::disconnect);
    });
    QObject* rootObject=engine.rootObjects()[0];
    engine.rootContext()->setContextProperty("Connection", &connection);
    QObject::connect(rootObject,SIGNAL(playerCommand(int,int,bool)),&connection,SLOT(send(int,int,bool)));

    return app.exec();
}
