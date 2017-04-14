#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QTcpSocket>
#include <QSharedPointer>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>
#include <QSharedPointer>

/* To use, subclass this class, implement "query" and "notification" methods
 * without forgetting to call wrapWithType to wrap the message if you send it
 * on the network via m_sock */
class AbstractNotifier: public QObject
{
    Q_OBJECT

    struct NotifType{
        const QByteArray CELLHASCHANGED = "CELL";
        const QByteArray LOOPHASCHANGED = "LOOP";
        const QByteArray TABHASCHANGED = "TAB";
    };
protected:
    QSharedPointer<QTcpSocket> m_sock;


    static const QString CMDFILE;
    static const NotifType notifType;
    static const QString JSON_TYPE;
    static const QString JSON_DATA;

    virtual void notification(QJsonValue value) = 0;
    virtual void query(QJsonValue value) = 0;
public:
    static const QString NOTIFY;
    static const QString DB;
    AbstractNotifier();
    AbstractNotifier(QSharedPointer<QTcpSocket> sock);
    void send(QJsonValue value, QString type);
    void setSocket(QSharedPointer<QTcpSocket> sock);
    virtual ~AbstractNotifier(){}

signals:
    newNotification(QJsonValue value);
    newQuery(QJsonValue value);

private slots:
    void parse();
private:
    QJsonDocument wrapWithType(QJsonValue data,QString type);

};

#endif // NOTIFIER_H
