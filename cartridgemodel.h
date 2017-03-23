#ifndef CARTRIDGEMODEL_H
#define CARTRIDGEMODEL_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QDebug>
#include <QModelIndex>
#include "datapuller.h"

#define DEFAULT_WIDTH 3
#define DEFAULT_HEIGHT 10

class CartridgeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int widthModel READ widthModel WRITE setWidthModel NOTIFY widthModelChanged)
    Q_PROPERTY(int heightModel READ heightModel WRITE setHeightModel NOTIFY heightModelChanged)
    static const QString QUERY;
public:
    enum RoleNames{
        PERFORMER = Qt::UserRole,
        TITLE = Qt::UserRole+1,
        DURATION = Qt::UserRole+2,
        ID = Qt::UserRole+3,
        POSITION = Qt::UserRole+4
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
protected:
    QHash<int,QByteArray> roleNames() const override;

public slots:
    void fitToDimension();
    void changePanel(int idPanel);

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

    /*
     * Methods
     */
    void fillHolesInList(int maxPosition);
    void listFromSQL();
    void load();
    void clear();
};

#endif // CARTRIDGEMODEL_H
