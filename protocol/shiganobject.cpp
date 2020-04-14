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
        qDebug()<<"启动循环失败";
        return;
    }
    shiGan.loopInit();
    while(true){
        QMutexLocker lockrt(&mMutex);
        if(isFinish)
            break;
        if(!shiGan.readOneFrame()){

            break;
        }
    }


    shiGan.loopUnInit();

    if(!isFinish){
        qDebug()<<"重新连接";
        slot_loopRec();
    }
}
