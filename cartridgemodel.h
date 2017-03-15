#ifndef CARTRIDGEMODEL_H
#define CARTRIDGEMODEL_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QDebug>
#include <QModelIndex>
#include "datapuller.h"

class CartridgeModel : public QAbstractListModel
{
    Q_OBJECT
    static const QString QUERY;
public:
    enum RoleNames{
        PERFORMER = Qt::UserRole,
        TITLE = Qt::UserRole+1,
        DURATION = Qt::UserRole+2,
        ID = Qt::UserRole+3
    };

    explicit CartridgeModel(QObject *parent = 0);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int,QByteArray> roleNames() const override;

public slots:
    void fitToDimension(int x, int y);

private:
    /*
     * Attributes
     */
    QList<QHash<RoleNames,QVariant>> m_data;
    QHash<int,QByteArray> m_roleNames;
    DataPuller m_updater;

    /*
     * Methods
     */
    void fillHolesInList(int maxPosition);
    void listFromSQL();
};

#endif // CARTRIDGEMODEL_H
