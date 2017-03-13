#ifndef DATAPULLER_H
#define DATAPULLER_H
#include "iupdatenotifier.h"
#include <QTimer>

class DataPuller : public IUpdateNotifier
{
public:
    DataPuller();
    ~DataPuller();
    void updateData();
private:
    QTimer* timer;
};


#endif // DATAPULLER_H
