#include "datapuller.h"

DataPuller::DataPuller()
{
    timer = new QTimer(this);
    timer->setInterval(50000);
    connect(timer,&QTimer::timeout,this,&DataPuller::updateData);
    timer->start();
}

DataPuller::~DataPuller(){
    delete(timer);
}

void DataPuller::updateData(){
    emit dataUpdated();
}
