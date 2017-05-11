#ifndef STATEKEEPER_H
#define STATEKEEPER_H

#include <QObject>
#include <QMap>

class StateKeeper: public QObject
{
    Q_OBJECT
public slots:

    bool contains(const uint &key);
    void insert(const uint &key, const uint &value);
    int remove(const uint &key);
    uint get(const uint &key);
    void clear();

private:
    QMap<uint,uint> m_states;
};

#endif // STATEKEEPER_H
