#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QtCore>
#include <QQmlContext>
#include <QTableView>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Driver={SQL Server}; Server=193.253.53.24,1437; Uid=WinBizz; Pwd=WinBizz2012;");
    QSqlQueryModel model;
    if(db.open()){
        model.setQuery(
        "SELECT Performer,Title,Position,Cartridge.Duration\
        FROM [Winmedia].[dbo].[Cartridge],[Winmedia].[dbo].[Media],[WinMedia].[dbo].[Panel]\
        WHERE Cartridge.Media = Media.IMedia\
        AND Cartridge.Panel = Panel.IPanel\
        AND Panel.IPanel = 1"
        );
//        qDebug() << model.data(model.index(1,0)).toInt();
//        while(query.next()){
//            for(int i=0;i<4;i++)
//                qDebug() << query.value(i).toString();
//        }
    }
    QTableView view;
    view.setModel(&model);
    view.show();

//    QQmlApplicationEngine engine;
//    engine.rootContext()->setContextProperty("myModel",&model);
//    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
