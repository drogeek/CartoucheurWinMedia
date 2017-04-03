#include "cartridgemodel.h"

const QString CartridgeModel::QUERY=QString("\
            SELECT Position,Performer,Title,Cartridge.Start,Cartridge.Stop,Cartridge.Stretch,ICartridge\
            FROM [Winmedia].[dbo].[Cartridge],[Winmedia].[dbo].[Media],[WinMedia].[dbo].[Panel]\
            WHERE Cartridge.Media = Media.IMedia\
            AND Cartridge.Panel = Panel.IPanel\
            AND Panel.IPanel = %1\
            AND Cartridge.Position < %2\
            ORDER BY Position");

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
    connect(this,&CartridgeModel::panelChanged,this,&CartridgeModel::load);
    connect(&m_updater,&IUpdateNotifier::dataUpdated,this,&CartridgeModel::load);
}

        //TODO: implement headerData
bool CartridgeModel::setData(const QModelIndex &index, const QVariant &value, int role){
    int position = index.row();
    QHash<RoleNames,QVariant> update(m_data.at(position));
    update[(CartridgeModel::RoleNames)role] = value;
    m_data.replace(position,update);
    return true;
}

void CartridgeModel::listFromSQL(){
    qDebug() << "Data updated";
    QSqlQuery query(m_formatedQuery);

    int position;
    beginResetModel();
    while(query.next()){
        position = query.value(0).toInt();
        qDebug() << "Position = " << position;
        fillHolesInList(position);

        QHash<RoleNames,QVariant> hash;
        hash.insert(PERFORMER,query.value(1));
        hash.insert(TITLE,query.value(2));
        hash.insert(START,query.value(3));
        hash.insert(STOP,query.value(4));
        hash.insert(STRETCH,query.value(5));
        hash.insert(ID,query.value(6));
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
    listFromSQL();
//    fitToDimension();
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

void CartridgeModel::swap(int from, int to){
    qDebug() << "from:" << m_data.at(from);
    qDebug() << "to:" << m_data.at(to);
    m_data.swap(from,to);
    qDebug() << "new from:" << m_data.at(from);
    qDebug() << "new to:" << m_data.at(to);
    dataChanged(index(0),index(rowCount()-1));
}
