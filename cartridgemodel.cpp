#include "cartridgemodel.h"

const QString CartridgeModel::QUERY=QString("\
            SELECT Position,Performer,Title,Cartridge.Start,Cartridge.Stop,Cartridge.Stretch,ICartridge\
            FROM [WinMedia].[dbo].[Cartridge],[WinMedia].[dbo].[Media],[WinMedia].[dbo].[Panel]\
            WHERE Cartridge.Media = Media.IMedia\
            AND Cartridge.Panel = Panel.IPanel\
            AND Panel.IPanel = %1\
            AND Cartridge.Position < %2\
            ORDER BY Position");
const QString CartridgeModel::SWAP=QString("\
            UPDATE [WinMedia].[dbo].[Cartridge] \
            SET Position = ( \
                       SELECT SUM(Position) \
                       FROM  [WinMedia].[dbo].[Cartridge] \
                       WHERE ICartridge IN ('%1', '%2') \
                       ) - Position \
            WHERE ICartridge IN ('%1', '%2')");
const QString CartridgeModel::MOVE=QString("\
            UPDATE [WinMedia].[dbo].[Cartridge] \
            SET Position = %1 \
            WHERE ICartridge = %2");

CartridgeModel::CartridgeModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames[PERFORMER]= "performer";
    m_roleNames[START]= "start";
    m_roleNames[STOP]= "stop";
    m_roleNames[STRETCH]= "stretch";
    m_roleNames[TITLE]= "title";
    m_roleNames[ID]= "id";

    //TODO: recover from file
    setWidthModel(DEFAULT_WIDTH);
    setHeightModel(DEFAULT_HEIGHT);

    Utils& util= Utils::getInstance();
    m_sock.connectToHost(QHostAddress(util.getServerIp()),util.getServerPort());
    qDebug() << "from CartridgeModel:";
    qDebug() << m_sock.errorString();
    connect(&m_sock,&QTcpSocket::connected,[this](){
        connect(this,&CartridgeModel::panelChanged,this,&CartridgeModel::load);
        connect(&m_updater,&IUpdateNotifier::dataUpdated,this,&CartridgeModel::load);
        connect(&m_sock,&QTcpSocket::readyRead,this,&CartridgeModel::listFromJson);
    });
}

        //TODO: implement headerData
bool CartridgeModel::setData(const QModelIndex &index, const QVariant &value, int role){
    int position = index.row();
    QHash<RoleNames,QVariant> update(m_data.at(position));
    update[(CartridgeModel::RoleNames)role] = value;
    m_data.replace(position,update);
    return true;
}

void CartridgeModel::sendQuery(){
    qDebug() << "query sent";
    m_sock.write(m_formatedQuery.toUtf8());
}

void CartridgeModel::listFromJson(){
    qDebug() << "Data updated";

    auto input = m_sock.readAll();
    qDebug() << input;
    beginResetModel();
    for(auto cell : QJsonDocument::fromJson(input).array()){
        QJsonObject obj = cell.toObject();
        int position = obj["Position"].toInt();
        qDebug() << "Position = " << position;
        fillHolesInList(position);
        QHash<RoleNames,QVariant> hash;
        hash.insert(PERFORMER,obj["Performer"].toString());
        hash.insert(TITLE,obj["Title"].toString());
        hash.insert(START,obj["Start"].toInt());
        hash.insert(STOP,obj["Stop"].toInt());
        hash.insert(STRETCH,obj["Stretch"].toDouble());
        hash.insert(ID,obj["ICartridge"].toInt());
        m_data.replace(position, hash);
    }
    while(m_data.count()<rowCount()){
        QHash<RoleNames,QVariant> empty;
        m_data.append(empty);
    }
    endResetModel();
        //TODO: send signal that data has changed
//    emit dataChanged(index(0,0),index(m_data.count()-1,0));
}

/* iteratively fill holes in the list */
void CartridgeModel::fillHolesInList(int maxPosition){
    int indexLast = m_data.count();

    for(int i=0; i<maxPosition-indexLast+1;i++){
        m_data << QHash<RoleNames,QVariant>();
    }
}

int CartridgeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
//    return m_data.count();
    return m_width*m_height;
}

QVariant CartridgeModel::data(const QModelIndex &index, int role) const
{
    int position = index.row();
    if(position<0 || position >= m_data.count()){
        return QVariant();
    }

    QHash<RoleNames,QVariant> dataAtPosition = m_data.at(position);

    return dataAtPosition.value((CartridgeModel::RoleNames)role);
}

QHash<int,QByteArray> CartridgeModel::roleNames() const
{
    return m_roleNames;
}

void CartridgeModel::fitToDimension(){
    int nbrBlockInView=m_width*m_height, nbrCurrentData=m_data.count();
    if(nbrBlockInView > nbrCurrentData){
        emit beginInsertRows(QModelIndex(),nbrCurrentData,nbrBlockInView+1);
        while(m_data.count()<nbrBlockInView)
            m_data << QHash<RoleNames,QVariant>();
        qDebug() << "number of cases " << m_data.count();
        emit endInsertRows();
    }
}

void CartridgeModel::load(){
    qDebug() << "model reloaded";
    clear();
    m_formatedQuery = QUERY.arg(m_idPanel).arg(m_width*m_height);
    sendQuery();
}

void CartridgeModel::clear(){
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void CartridgeModel::changePanel(int idPanel){
    m_idPanel = idPanel;
    emit panelChanged();
}

void CartridgeModel::swap(int indexFrom, int indexTo, int idFrom, int idTo){
    qDebug() << "from:" << indexFrom;
    qDebug() << "to:" << indexTo;
    qDebug() << "idfrom:" << idFrom;
    qDebug() << "idto:" << idTo;
    if(idFrom == -1 && idTo == -1)
        return;

    int isOk=false;
    if(idFrom == -1)
        isOk = m_sock.write(MOVE.arg(indexFrom).arg(idTo).toUtf8());
    else if(idTo == -1)
        isOk = m_sock.write(MOVE.arg(indexTo).arg(idFrom).toUtf8());
    else
        isOk = m_sock.write(SWAP.arg(idFrom).arg(idTo).toUtf8());
    if(isOk != -1){
        m_data.swap(indexFrom,indexTo);
        dataChanged(index(0),index(rowCount()-1));
    }
    else
        qDebug() << m_sock.errorString();
}
