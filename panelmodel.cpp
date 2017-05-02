#include "panelmodel.h"

const QString PanelModel::QUERY = QString("\
           SELECT IPanel,Name\
           FROM [Winmedia].[dbo].[Panel]"
        );

PanelModel::PanelModel(QObject *parent)
    : QAbstractListModel(parent)
{
     m_roleNames[ID]="id";
     m_roleNames[NAME]="name";
}

void PanelModel::sendQuery(){
    m_notifier->send(QUERY,ClientNotifier::TYPE_DB,ClientNotifier::TARGET_PANEL);
}

QVariant PanelModel::data(const QModelIndex &index, int role) const
{
    int position = index.row();
    if(position<0 || position >=m_data.count()){
        return QVariant();
    }

    QHash<RoleNames,QVariant> dataAtPosition = m_data.at(position);
    return dataAtPosition.value((PanelModel::RoleNames)role);
}

QHash<int,QByteArray> PanelModel::roleNames() const{
    return m_roleNames;
}

int PanelModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return m_data.count();
}

void PanelModel::listFromJson(QString target,QJsonValue value){
    if(target == ClientNotifier::TARGET_PANEL){
        qDebug() << "Panel query";
        QJsonArray array = value.toArray();
        beginResetModel();
        m_data.clear();
        for(auto cell : array){
            QJsonObject obj = cell.toObject();
            QHash<RoleNames,QVariant> hash;
            hash.insert(ID,obj["IPanel"].toInt());
            hash.insert(NAME,obj["Name"].toString());
            m_data.append(hash);
        }
        endResetModel();
    }
}
