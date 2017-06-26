#ifndef CARTRIDGEMODEL_H
#define CARTRIDGEMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QModelIndex>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "clientnotifier.h"
#include "datapuller.h"

#define DEFAULT_WIDTH 4
#define DEFAULT_HEIGHT 8

class CartridgeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int widthModel READ widthModel WRITE setWidthModel NOTIFY widthModelChanged)
    Q_PROPERTY(int heightModel READ heightModel WRITE setHeightModel NOTIFY heightModelChanged)
    Q_PROPERTY(ClientNotifier* notifier READ notifier WRITE setNotifier)
    static const QString QUERY;
    static const QString SWAP;
    static const QString MOVE;
public:
    enum RoleNames{
        PERFORMER = Qt::UserRole,
        TITLE = Qt::UserRole+1,
        START = Qt::UserRole+2,
        STOP = Qt::UserRole+3,
        STRETCH = Qt::UserRole+4,
        ID = Qt::UserRole+5,
        COLOR = Qt::UserRole+6,
        GENRE = Qt::UserRole+7
    };

    explicit CartridgeModel(QObject *parent = 0);

    // Basic functionalities:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    // Getters/Setters
    int widthModel(){ return m_width; }
    int heightModel(){ return m_height; }
    void setWidthModel(int newWidth) {
        m_width = newWidth;
        emit widthModelChanged();
    }
    void setHeightModel(int newHeight) {
        m_height = newHeight;
        emit heightModelChanged();
    }
    ClientNotifier* notifier(){ return m_notifier; }
    void setNotifier(ClientNotifier* notifier){
        qDebug() << "notifier added to Cartridge";
        m_notifier = notifier;
        qDebug() << notifier;
        connect(m_notifier,&ClientNotifier::newQuery,this,&CartridgeModel::listFromJson);
    }

protected:
    QHash<int,QByteArray> roleNames() const override;

public slots:
    void fitToDimension();
    void changePanel(int idPanel);
    void swap(int indexFrom, int indexTo, int idFrom, int idTo);

signals:
    void panelChanged();
    void widthModelChanged();
    void heightModelChanged();

private:
    /*
     * Attributes
     */
    QList<QHash<RoleNames,QVariant>> m_data;
    QHash<int,QByteArray> m_roleNames;
    DataPuller m_updater;
    int m_idPanel;
    QString m_formatedQuery;
    int m_width,m_height;
    ClientNotifier* m_notifier;

    /*
     * Methods
     */
    void fillHolesInList(int maxPosition);
    void listFromJson(QString target,QJsonValue value);
    void sendQuery();
    void load();
    void clear();
};

#endif // CARTRIDGEMODEL_H
