#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include <QtCore>
#include <QQmlContext>
#include <QTableView>
#include "cartridgemodel.h"
#define __WINMEDIA_DEBUG


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

#ifdef __linux__
    const QString DRIVER = "ODBC Driver 13 for SQL Server";
#elif _WIN32
    const QString DRIVER = "SQL Server";
#endif

    //TODO: to be replaced by data provided in a file
    const QString SERVER = "193.253.53.24";
    const QString PORT = "1437";
    const QString USER = "WinBizz";
    const QString PASSWORD = "WinBizz2012";


#ifdef __WINMEDIA_DEBUG
    QStringList drivers = QSqlDatabase::drivers();
    QStringList::const_iterator constIter;
    qDebug() << "Available drivers:";
    for(constIter=drivers.constBegin(); constIter != drivers.constEnd(); constIter++){
        qDebug() << (*constIter).toLocal8Bit().constData();
    }
#endif

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(
                "Driver={"+DRIVER+"};\
                Server="+SERVER+","+PORT+";\
                Uid="+USER+";\
                Pwd="+PASSWORD+";"
    );

    if(db.open()){
        qDebug() << "Opening of DB successful";
//    QTableView view;
//    view.setModel(&model);
//    view.show();

        qmlRegisterType<CartridgeModel>("fr.univtln.jguiennet299",1,0,"CartridgeModel");
    }
    else
        qDebug() << db.lastError();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
