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
     Utils& util = Utils::getInstance();
     qDebug() << util.getServerIp();
     qDebug() << util.getServerPort();
     m_sock.connectToHost(QHostAddress(util.getServerIp()),util.getServerPort());
     qDebug() << "from PanelModel:";
     qDebug() << m_sock.errorString();
     connect(&m_sock,&QTcpSocket::connected,this,&PanelModel::sendQuery);
     connect(&m_sock,&QTcpSocket::readyRead,this,&PanelModel::listFromJson);
}

void PanelModel::sendQuery(){
    qDebug() << "query sent for Panels";
    m_sock.write(QUERY.toUtf8());
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

void PanelModel::listFromJson(){
    qDebug() << "Panel query";
    auto input = m_sock.readAll();
    beginResetModel();
    for(auto cell : QJsonDocument::fromJson(input).array()){
        QJsonObject obj = cell.toObject();
        QHash<RoleNames,QVariant> hash;
        hash.insert(ID,obj["IPanel"].toInt());
        hash.insert(NAME,obj["Name"].toString());
        m_data.append(hash);
    }
    endResetModel();
}
