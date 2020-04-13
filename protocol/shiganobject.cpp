#include "shiganobject.h"

ShiGanObject::ShiGanObject(QObject *parent) : QObject(parent)
{

}

void ShiGanObject::forceFinishThread()
{
    QMutexLocker lockrt(&mMutex);
    isFinish = true;
}

bool ShiGanObject::startRec()
{
    return shiGan.startRec();
}

void ShiGanObject::slot_loopRec(){

    shiGan.loopInit();
    while(true){
        QMutexLocker lockrt(&mMutex);
        if(isFinish)
            break;
        shiGan.readOneFrame();
    }
    shiGan.loopUnInit();
}
