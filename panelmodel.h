#ifndef PANELMODEL_H
#define PANELMODEL_H

#include <QTcpSocket>
#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostAddress>
#include "utils.h"

class PanelModel : public QAbstractListModel
{
    Q_OBJECT

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

protected:
    virtual QHash<int,QByteArray> roleNames() const override;

private:
    QHash<int,QByteArray> m_roleNames;
    QList<QHash<RoleNames,QVariant>> m_data;
    QTcpSocket m_sock;

    void listFromJson();
    void sendQuery();
};

#endif // PANELMODEL_H
