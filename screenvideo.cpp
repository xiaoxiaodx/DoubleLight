#include "screenvideo.h"
#include <QDebug>
#include <QQuickWindow>
#include <QBuffer>

ScreenVideo::ScreenVideo(QObject *parent) : QObject(parent)
{

}

bool ScreenVideo::funScreenShoot(QString path,QQuickWindow *quic,int capx,int capy,int capw,int caph)
{


    qDebug()<<"screnn "<<path;

    QImage img= quic->grabWindow();

    QImage img1 = img.copy( capx, capy, capw, caph);

    QString  time = QDateTime::currentDateTime().toString("yyMMddhhmmss");

    img.save(path+"/"+time+".png","PNG");

}


bool ScreenVideo::funStartScreenRecrod(QString str)
{

    if(writeAvi == nullptr){

        writeAvi = new AviRecord("doubleLight");
        writeAviThread = new QThread;
        connect(this,&ScreenVideo::signal_readyWriteAvi,writeAvi,&AviRecord::slot_readyRecord);
        connect(this,&ScreenVideo::signal_endWriteAvi,writeAvi,&AviRecord::slot_endRecord);
        connect(this,&ScreenVideo::signal_writeAvi,writeAvi,&AviRecord::slot_writeImage);
        writeAvi->moveToThread(writeAviThread);
        writeAviThread->start();

    }
    //这里加个阻塞等待

    emit signal_readyWriteAvi(str);
    return true;
}

void ScreenVideo::funScreenRecrod(QQuickWindow *quic,int capx,int capy,int capw,int caph)
{


    QImage img= quic->grabWindow();

    emit signal_writeAvi(img,capx,capy,capw,caph);

}

bool  ScreenVideo::funEndScreenRecrod()
{
    //这里加个阻塞等待
    if(writeAvi != nullptr && writeAvi->isRecording){
        return true;
    }
    emit signal_endWriteAvi();
    return true;
}

