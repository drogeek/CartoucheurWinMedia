#ifndef PANELMODEL_H
#define PANELMODEL_H

#include <QTcpSocket>
#include <QAbstractListModel>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostAddress>
#include "clientnotifier.h"

class PanelModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ClientNotifier* notifier READ notifier WRITE setNotifier)

    static const QString QUERY;
public:
    explicit PanelModel(QObject *parent = 0);

    enum RoleNames {
        ID=Qt::UserRole,
        NAME=Qt::UserRole+1
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Getters/Setters
    ClientNotifier* notifier(){ return m_notifier; }
    void setNotifier(ClientNotifier* notifier){
        qDebug() << "notifier added to Panel";
        m_notifier = notifier;
        connect(m_notifier, &ClientNotifier::connectedChanged,[this](){
            if(m_notifier->connected())
                sendQuery();
        });
        connect(m_notifier,&ClientNotifier::newQuery,this,&PanelModel::listFromJson,Qt::UniqueConnection);
    }

protected:
    virtual QHash<int,QByteArray> roleNames() const override;

private:
    QHash<int,QByteArray> m_roleNames;
    QList<QHash<RoleNames,QVariant>> m_data;
    ClientNotifier* m_notifier;

    void listFromJson(QString target,QJsonValue value);
    void sendQuery();
};

#endif // PANELMODEL_H
