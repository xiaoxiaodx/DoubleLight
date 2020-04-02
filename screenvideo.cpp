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
#include <QDir>
QString ScreenVideo::funGetCurPaht()
{
    QDir dir;
    return dir.currentPath();

}

bool ScreenVideo::funStartScreenRecrod(QString str)
{

    if(writeAvi == nullptr){

        writeAvi = new AviRecord("doubleLight");
        writeAviThread = new QThread;
        connect(this,&ScreenVideo::signal_readyWriteAvi,writeAvi,&AviRecord::slot_readyRecord);
        connect(this,&ScreenVideo::signal_endWriteAvi,writeAvi,&AviRecord::slot_endRecord);
        connect(this,&ScreenVideo::signal_writeAvi,writeAvi,&AviRecord::slot_writeImage);
        connect(writeAvi,&AviRecord::signal_startSucc,this,&ScreenVideo::slot_startSucc);
        connect(writeAvi,&AviRecord::signal_endSucc,this,&ScreenVideo::slot_endSucc);
        writeAvi->moveToThread(writeAviThread);
        writeAviThread->start();
    }

    emit signal_readyWriteAvi(str);
    loop.exec();
    return isStartSucc;
}

void ScreenVideo::slot_startSucc(bool isSucc){
    qDebug()<<"slot_startSucc   "<<isSucc;
    isStartSucc = isSucc;
    loop.exit();
}

void ScreenVideo::funScreenRecrod(QQuickWindow *quic,int capx,int capy,int capw,int caph)
{


    QImage img= quic->grabWindow();

    emit signal_writeAvi(img,capx,capy,capw,caph);

}

bool  ScreenVideo::funEndScreenRecrod()
{
    if(writeAvi == nullptr){
        return false;
    }
    qDebug()<<"signal_endWriteAvi";
    emit signal_endWriteAvi();
    loop.exec();
    return isEndSucc;
}

void ScreenVideo::slot_endSucc(bool isSucc){
    qDebug()<<"funEndScreenRecrod   "<<isSucc;
    isEndSucc = isSucc;
    loop.exit();


}

