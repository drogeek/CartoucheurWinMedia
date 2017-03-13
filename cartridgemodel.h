#ifndef CARTRIDGEMODEL_H
#define CARTRIDGEMODEL_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QDebug>
#include <QModelIndex>
#include "datapuller.h"
// /!\ Redefine NUMBER_OF_ROLES if you add more roles into RoleNames
#define NUMBER_OF_ROLES 3
/* need this to define how many elements will populate m_data
 * because in the view data grows from top to bottom until it reaches
 * MAX_LINES. It's very ugly, but didn't figure out how to deal with
 * QAbstractTableModel
 */
#define MAX_LINES 3

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
