#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include <QtCore>
#include <QSocketNotifier>
#include <QQmlContext>
#include <QTcpServer>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSharedPointer>
#include <QTimer>
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

//TODO: to be replaced by data provided in a file
//    const QString SERVER = "193.253.53.24";
//    const QString PORT = "1437";
//    const QString USER = "WinBizz";
//    const QString PASSWORD = "WinBizz2012";
    const QString SERVER = "127.0.0.1";
    const QString PORT = "1433";
    const QString USER = "test";
    const QString PASSWORD = "test";
    const QString NAME = "CartridgeApplication";
    const QString IP = "127.0.0.1";

    QTcpServer server;
    server.listen(QHostAddress("127.0.0.1"),1234);
    qDebug() << server.errorString();


#ifdef __WINMEDIA_DEBUG
    //Show available drivers
    QStringList drivers = QSqlDatabase::drivers();
    QStringList::const_iterator constIter;
    qDebug() << "Available drivers:";
    for(constIter=drivers.constBegin(); constIter != drivers.constEnd(); constIter++){
        qDebug() << (*constIter).toLocal8Bit().constData();
    }

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

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(
                "Driver={"+DRIVER+"};\
                Server="+SERVER+","+PORT+";\
                Uid="+USER+";\
                Pwd="+PASSWORD+";"
    );

    QTimer timer;
    if(db.open()){
        qDebug() << "Opening of DB successful";
        //Record ourselves in the Computer table on the DB
        QSqlQuery query;
        qDebug() << QString("SELECT Ip \
                            FROM [Winmedia].[dbo].[Computer] \
                            WHERE Name = '%1'").arg(NAME);
        auto result=query.exec(QString("SELECT Name,Ip \
                                FROM [Winmedia].[dbo].[Computer] \
                                WHERE Name = '%1'").arg(NAME));
        if(!result){
            qDebug() << query.lastError();
        }
        else{
            query.next();
            if(!query.isValid()){
                qDebug() << "Can't find you in the DB, adding you";
                QSqlQuery insert;
                auto result = insert.exec(QString(
                                     "INSERT INTO [WinMedia].[dbo].[Computer] (Name,Ip,Modify,X,Y,Command,Properties) \
                                     VALUES ('"+NAME+"','"+IP+"',getutcdate(),0,0,'','')")
                                    );
                if(!result){
                    qDebug() << insert.lastError();
                }
            }
            else{
                auto updateModify = [NAME](){
                    QSqlQuery update;
                    auto result = update.exec(
                                "UPDATE [WinMedia].[dbo].[Computer] \
                                SET modify=getutcdate() \
                                WHERE name = '"+NAME+"'"
                            );
                    if(!result){
                        qDebug() << update.lastError();
                    }
                };
                updateModify();
                QObject::connect(&timer,&QTimer::timeout, updateModify);
                timer.start(5000);
            }
        }
    }
    else
        qDebug() << db.lastError();

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
