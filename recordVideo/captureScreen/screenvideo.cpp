#include "screenvideo.h"
#include <QDebug>
#include <QQuickWindow>
#include <QBuffer>
#include <QDateTime>
#include <QDir>
#include "debuglog.h"

ScreenVideo::ScreenVideo(QObject *parent) : QObject(parent)
{
    writepath.clear();
}

QString ScreenVideo::funGetCurPath()
{
    QDir dir;
    return dir.currentPath();
}
//如果还没有创建则新建记录线程，如果创建了则发送参数信息
void ScreenVideo::funCreateScreenThread(QString capturePath,QString recordPath,int period)
{
//    if(writeH264 == nullptr){

//        writeH264 = new WriteH264(capturePath,recordPath,period);
//        writeThread = new QThread;
//        connect(this,&ScreenVideo::signal_writeH264,writeH264,&WriteH264::slot_writeH264);
//        connect(this,&ScreenVideo::signal_screenShot,writeH264,&WriteH264::slot_screenShot);
//        connect(this,&ScreenVideo::signal_recordPar,writeH264,&WriteH264::slot_recordPar);
//        connect(this,&ScreenVideo::signal_screenCapturePar,writeH264,&WriteH264::slot_screenCapturePar);
//        connect(this,&ScreenVideo::signal_forceFinishRecord,writeH264,&WriteH264::slot_forceFinishRecord);
//        writeH264->moveToThread(writeThread);
//        writeThread->start();
//    }else{
//        emit signal_recordPar(recordPath,period);
//        emit signal_screenCapturePar(capturePath);
//    }

}

void ScreenVideo::funCreateAviRecordThread(QString recordPath,int period)
{
    if(aviRecord == nullptr){
        aviRecord = new AviRecord("");
        aviThread = new  QThread;

        connect(this,&ScreenVideo::signal_writeH264,aviRecord,&AviRecord::slot_writeImage);
        connect(this,&ScreenVideo::signal_forceFinishRecord,aviRecord,&AviRecord::slot_endRecord);

        aviRecord->moveToThread(aviThread);
        aviThread->start();
    }else{
        emit signal_recordPar(recordPath,period);
    }

}
bool ScreenVideo::funScreenShoot(QString path,QQuickWindow *quic,int capx,int capy,int capw,int caph,float warnTemp)
{
//    if(nullptr == writeH264)
//        DebugLog::getInstance()->writeLog("截屏线程还未初始化");
//    else{
//        QImage img= quic->grabWindow();
//        emit signal_screenShot(img,capx,capy,capw,caph,warnTemp);
//    }
}


void ScreenVideo::funScreenRecrod(QString path,QQuickWindow *quic,int capx,int capy,int capw,int caph)
{

//    if(nullptr == writeH264)
//        DebugLog::getInstance()->writeLog("录屏线程还未初始化");
//    else{
//        QImage img= quic->grabWindow();
//        emit signal_writeH264(img,capx,capy,capw,caph);
//    }


    if(aviRecord == nullptr)
        DebugLog::getInstance()->writeLog("录屏线程还未初始化");
    else{
        QImage img= quic->grabWindow();
        emit signal_writeH264(path,img,capx,capy,capw,caph);
    }
}


void ScreenVideo::funSetRecordPar(QString path,int period){
    emit signal_recordPar(path,period);
}

void ScreenVideo::funSetScreenCapturePar(QString path)
{
    emit signal_screenCapturePar(path);
}

void ScreenVideo::funForfinishRecord()
{
    emit signal_forceFinishRecord();
}


