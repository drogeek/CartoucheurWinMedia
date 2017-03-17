#ifndef PANELMODEL_H
#define PANELMODEL_H

#include <QSqlQueryModel>

class PanelModel : public QSqlQueryModel
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
//    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int,QByteArray> roleNames() const override;

private:
    QHash<int,QByteArray> m_roleNames;

};

#endif // PANELMODEL_H
