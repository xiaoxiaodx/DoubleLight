#include "shiganobject.h"

ShiGanObject::ShiGanObject(QObject *parent) : QObject(parent)
{


}

void ShiGanObject::sendHeart()
{
    shiGan.KeepAliveReq();
}
void ShiGanObject::forceFinishThread()
{
    QMutexLocker lockrt(&mMutex);
    isFinish = true;
}

#include <QThread>
void ShiGanObject::slot_loopRec(){

    if(!shiGan.startRec()){
        qDebug()<<"shigan start loop failue";
        return;
    }
    shiGan.loopInit();
    while(true){
        QMutexLocker lockrt(&mMutex);
        if(isFinish)
            break;

        ImageInfo info;
        info.pImg = nullptr;
        if(!shiGan.readOneFrame(info)){

            break;
        }
        if(info.pImg != nullptr){
            qDebug()<<"info:"<<info.listRect.size();
            emit signal_sendImageInfo(info.pImg, QVariant::fromValue(info.listRect),info.areaMaxtemp);
        }
        QThread::msleep(5);
    }


    shiGan.loopUnInit();

    if(!isFinish){
        qDebug()<<"重新连接";
        slot_loopRec();
    }
}
