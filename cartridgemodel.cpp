#include "cartridgemodel.h"

const QString CartridgeModel::QUERY=QString("\
            SELECT Cartridge.Position,Performer,Title,Cartridge.Start,Cartridge.Stop,Cartridge.Stretch,ICartridge,Backcolor\
            FROM [WinMedia].[dbo].[Cartridge],[WinMedia].[dbo].[Media],[WinMedia].[dbo].[Panel],[WinMedia].[dbo].[Belong],[WinMedia].[dbo].[Category]\
            WHERE Cartridge.Media = Media.IMedia\
            AND Belong.media = Media.IMedia\
            AND Belong.category = Category.ICategory\
            AND getdate() between beginning and ending\
            AND Cartridge.Panel = Panel.IPanel\
            AND Panel.IPanel = %1\
            AND Cartridge.Position < %2\
            ORDER BY Cartridge.Position");

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
    m_roleNames[COLOR]= "backcolor";

    //TODO: recover from file
    setWidthModel(DEFAULT_WIDTH);
    setHeightModel(DEFAULT_HEIGHT);

    connect(this,&CartridgeModel::panelChanged,this,&CartridgeModel::load);
//    connect(&m_updater,&IUpdateNotifier::dataUpdated,this,&CartridgeModel::load);
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
    m_notifier->send(m_formatedQuery,ClientNotifier::TYPE_DB,ClientNotifier::TARGET_CARTRIDGE);
}

void CartridgeModel::listFromJson(QString target,QJsonValue value){
    if(target == ClientNotifier::TARGET_CARTRIDGE){
        qDebug() << "Data updated";

        QJsonArray array = value.toArray();
        beginResetModel();
        for(auto cell : array){
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
            hash.insert(COLOR,obj["Backcolor"].toInt());
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
    QJsonObject obj;
    obj.insert(ClientNotifier::JSON_TYPE,QString(ClientNotifier::notifType.TABHASCHANGED));
    obj.insert(ClientNotifier::JSON_DATA,idPanel);
    m_notifier->send(obj,ClientNotifier::TYPE_NOTIFY,ClientNotifier::TARGET_CARTRIDGE);
    emit panelChanged();
}

void CartridgeModel::swap(int indexFrom, int indexTo, int idFrom, int idTo){
    qDebug() << "from:" << indexFrom;
    qDebug() << "to:" << indexTo;
    qDebug() << "idfrom:" << idFrom;
    qDebug() << "idto:" << idTo;
    if(idFrom == -1 && idTo == -1)
        return;

    if(idFrom == -1)
        m_notifier->send(MOVE.arg(indexFrom).arg(idTo),ClientNotifier::TYPE_DB,ClientNotifier::TARGET_CARTRIDGE);
    else if(idTo == -1)
        m_notifier->send(MOVE.arg(indexTo).arg(idFrom),ClientNotifier::TYPE_DB,ClientNotifier::TARGET_CARTRIDGE);
    else
        m_notifier->send(SWAP.arg(idFrom).arg(idTo),ClientNotifier::TYPE_DB,ClientNotifier::TARGET_CARTRIDGE);
//    if(isOk != -1){
    //TODO: check for error before swapping
    m_data.swap(indexFrom,indexTo);
    changePanel(m_idPanel);
    dataChanged(index(0),index(rowCount()-1));
//    }
//    else
//        qDebug() << m_sock.errorString();
}
