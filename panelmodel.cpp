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

//int PanelModel::rowCount(const QModelIndex &parent) const
//{
//    // For list models only the root node (an invalid parent) should return the list's size. For all
//    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
//    if (parent.isValid())
//        return 0;

//    // FIXME: Implement me!
//}

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
