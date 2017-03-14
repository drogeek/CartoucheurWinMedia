#include "cartridgemodel.h"

const QString CartridgeModel::QUERY="\
            SELECT Position,Performer,Title,Cartridge.Duration,ICartridge\
            FROM [Winmedia].[dbo].[Cartridge],[Winmedia].[dbo].[Media],[WinMedia].[dbo].[Panel]\
            WHERE Cartridge.Media = Media.IMedia\
            AND Cartridge.Panel = Panel.IPanel\
            AND Panel.IPanel = 1";

CartridgeModel::CartridgeModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames[PERFORMER]= "performer";
    m_roleNames[DURATION]= "duration";
    m_roleNames[TITLE]= "title";
    m_roleNames[ID]= "id";

    connect(&m_updater,&IUpdateNotifier::dataUpdated,this,&CartridgeModel::listFromSQL);
    listFromSQL();
}

void CartridgeModel::listFromSQL(){
    qDebug() << "Data updated";
    QSqlQuery query(QUERY);

    int position, maxPosition=0;
    while(query.next()){
        position = query.value(0).toInt();
        qDebug() << "Position = " << position;
        maxPosition = (maxPosition < position) ? position : maxPosition;
        fillHolesInList(maxPosition);

        QHash<RoleNames,QVariant> hash;
        hash.insert(PERFORMER,query.value(1));
        hash.insert(TITLE,query.value(2));
        hash.insert(DURATION,query.value(3));
        hash.insert(ID,query.value(4));
        m_data.replace(position, hash);
    }
        //TODO: send signal that data has changed
    emit dataChanged(index(0,0),index(m_data.count()-1,0));
}

/* iteratively fill holes in the list for a gridView with one
 * constant dimension of size MAX_LINES: ugly but I don't know
 * another way to deal with how the view is filled if there is
 * nothing
 */
void CartridgeModel::fillHolesInList(int maxPosition){
    int toIndex = (maxPosition/MAX_LINES+1)*MAX_LINES;
    int indexLast = m_data.count();
    for(int i=0; i<toIndex-indexLast;i++){
        m_data << QHash<RoleNames,QVariant>();
    }
}

int CartridgeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    return m_data.count();
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

void CartridgeModel::fitToDimension(int x, int y){
    int nbrBlockInView=x*y, nbrCurrentData=m_data.count();
    if(nbrBlockInView > nbrCurrentData){
        emit beginInsertRows(QModelIndex(),nbrCurrentData,nbrBlockInView);
        while(m_data.count()<nbrBlockInView)
            m_data << QHash<RoleNames,QVariant>();
        qDebug() << m_data.count();
        emit endInsertRows();
    }
}
