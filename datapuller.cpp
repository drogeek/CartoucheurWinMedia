#include "datapuller.h"

DataPuller::DataPuller()
{
    timer = new QTimer(this);
    timer->setInterval(5000);
    connect(timer,&QTimer::timeout,this,&updateData);
    timer->start();
}

DataPuller::~DataPuller(){
    delete(timer);
}

void DataPuller::updateData(){
    emit dataUpdated();
}
