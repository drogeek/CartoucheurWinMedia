#ifndef IUPDATENOTIFIER_H
#define IUPDATENOTIFIER_H
#include <QObject>

class IUpdateNotifier : public QObject{
    Q_OBJECT
public:
    virtual void updateData() = 0;
    virtual ~IUpdateNotifier() {}
signals:
    void dataUpdated();
};

#endif // IUPDATENOTIFIER_H
