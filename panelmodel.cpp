#include "panelmodel.h"

const QString PanelModel::QUERY = QString("\
           SELECT IPanel,Name\
           FROM [Winmedia].[dbo].[Panel]"
        );

PanelModel::PanelModel(QObject *parent)
    : QSqlQueryModel(parent)
{
     m_roleNames[ID]="id";
     m_roleNames[NAME]="name";
     QSqlQueryModel::setQuery(QUERY);
}

QVariant PanelModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index,role);
    if (role == ID){
        value = QSqlQueryModel::data(this->index(index.row(),0));
        return value.toString();
    }

    else if(role == NAME){
        value = QSqlQueryModel::data(this->index(index.row(),1));
        return value.toString();
    }

    // FIXME: Implement me!
    return QVariant();
}

QHash<int,QByteArray> PanelModel::roleNames() const{
    return m_roleNames;
}
