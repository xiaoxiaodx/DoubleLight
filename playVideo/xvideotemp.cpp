#include "xvideotemp.h"

#include <QPainter>
#include <QDebug>
#include <render/texturenode.h>
#include <QPainter>


QList<ImageInfo> XVideoTemp::listBufferImginfo;//缓存的图片指针
QMutex XVideoTemp::mutex;
XVideoTemp::XVideoTemp()
{
    // setFlag(QQuickItem::ItemHasContents);
    // setRenderTarget(QQuickPaintedItem::FramebufferObject);

    mRenderImginfo.pImg = nullptr;
}



void XVideoTemp::startTemperatureVideo(float tp)
{
    DebugLog::getInstance()->writeLog("startTemperatureVideo ");

    //createYouseePull();
    //createShiGan();
    createIRCNet();

    warnTemp = tp;

    connect(&timerUpdate,&QTimer::timeout,this,&XVideoTemp::slot_timeout);

    timerUpdate.start(5);
}

void XVideoTemp::createYouseePull()
{
    if(mYouSeeParse == nullptr){
        youseeThread = new QThread;
        mYouSeeParse = new YouSeeParse;
        mYouSeeParse->moveToThread(youseeThread);
        connect(this,&XVideoTemp::signal_startinit,mYouSeeParse,&YouSeeParse::slot_init);
        connect(this,&XVideoTemp::signal_stop,mYouSeeParse,&YouSeeParse::slot_stopPlay);
        connect(this,&XVideoTemp::signal_getInitPar,mYouSeeParse,&YouSeeParse::slot_getInitPar);
        connect(youseeThread,&QThread::finished,youseeThread,&QThread::deleteLater);
        connect(this,&XVideoTemp::signal_parSet,mYouSeeParse,&YouSeeParse::slot_parSet);
        youseeThread->start();
    }
    emit signal_startinit();
}



void XVideoTemp::createIRCNet()
{
    mircNet.ircInit();
}

void XVideoTemp::finishYouPull()
{
    if(mYouSeeParse != nullptr){

        mYouSeeParse->forceFinish();
        youseeThread->exit();
        youseeThread->wait();
        youseeThread = nullptr;
    }

}

void XVideoTemp::createShiGan(){

    if(pShiGanObject == nullptr){
        shiganThread = new QThread;
        pShiGanObject = new ShiGanObject;
        pShiGanObject->moveToThread(shiganThread);
        connect(this,&XVideoTemp::signal_readOneFrame,pShiGanObject,&ShiGanObject::slot_loopRec);
        shiganThread->start();
    }

    if(pShiGanObject->startRec()){
        qDebug()<<"开始接收shigan数据";
        emit signal_readOneFrame();
    }else{
        qDebug()<<"启动shigan失败";
    }
}


void XVideoTemp::fun_getInitPar()
{
    qDebug()<<"fun_getInitPar";
    emit signal_getInitPar();
}

void XVideoTemp::fun_temMax(QVariant mvalue){
    YouSeeParse::check_max_temp = mvalue.toFloat();
    qDebug()<<" check_max_temp  "<<YouSeeParse::check_max_temp;
}
void XVideoTemp::fun_temMin(QVariant mvalue){
    YouSeeParse::check_min_temp = mvalue.toFloat();
    qDebug()<<" check_min_temp  "<<YouSeeParse::check_min_temp;
}
void XVideoTemp::fun_temOffset(QVariant mvalue){
    YouSeeParse::temp_offset = mvalue.toFloat()/2;
    qDebug()<<" temp_offset  "<<YouSeeParse::temp_offset;
}

void XVideoTemp::slot_timeout()
{




    if(mutex.tryLock()){

        if( listBufferImginfo.size() >0){

            if(mRenderImginfo.pImg != nullptr){
                delete mRenderImginfo.pImg;
                mRenderImginfo.pImg = nullptr;
            }
             mRenderImginfo = listBufferImginfo.takeFirst();
        }
        mutex.unlock();
    }else {
        qDebug()<<"MEI YOU HUO QU SUO";
        return;
    }

    if(mRenderImginfo.pImg == nullptr)
        return;
    //如果不增加这句代码 ，则会出现视频不会第一时间显示，而是显示灰色图像
    if(!isFirstData){
        emit signal_loginStatus("Get the stream successfully");
        emit signal_initRedFrame(mRenderImginfo.pImg->width(),mRenderImginfo.pImg->height());

        isFirstData = true;
    }
    update();
}


#include <QFontMetrics>

void XVideoTemp::paint(QPainter *painter)
{


    if(mRenderImginfo.pImg == nullptr)
        return;
    QFont font("Microsoft Yahei", 20);
    QPen pen(QBrush(QColor(0,255,0)),1);
    painter->setPen(pen);
    painter->setFont(font);
    //DebugLog::getInstance()->writeLog("painter hongwai start***");
    qreal kX = (qreal)this->width()/(qreal)mRenderImginfo.pImg->width();
    qreal kY = (qreal)this->height()/(qreal)mRenderImginfo.pImg->height();


    painter->drawImage(QRect(0,0,this->width(),this->height()), *(mRenderImginfo.pImg));


    //将矩形链表发送给可见光
    if(mRenderImginfo.listRect.size()>0){
        //qDebug()<<"发送矩形:"<<QVariant::fromValue(mRenderImginfo->listRect);
        QVariantList list ;
        for(int i=0;i<mRenderImginfo.listRect.size();i++){
            QVariant map = mRenderImginfo.listRect.at(i);
            list.append(map);
        }
        emit signal_sendListRect(QVariant::fromValue(list));
    }else{
        emit signal_sendListRect(QVariantList());
    }

    for(int i=0;i<mRenderImginfo.listRect.size();i++){
        QMap<QString,QVariant> oriRectinfo = mRenderImginfo.listRect.at(i);
        QRect oriRect = oriRectinfo.value("rect").toRect();
        QRectF desRect(oriRect.x()*kX,oriRect.y()*kY,oriRect.width()*kX,oriRect.height()*kY);
        float temp = oriRectinfo.value("temp").toFloat();
        QString strText = QString::number(temp, 'f', 1);

        QVariantMap map;
        map.insert("rect",oriRect);
        map.insert("temp",strText);

        if(temp>warnTemp){
            painter->save();
            painter->setPen(QPen(QBrush(QColor(255,0,0)),2));
            painter->drawRect(desRect);
            painter->drawText(desRect.x(),desRect.y()-3,strText);
            painter->restore();

        }else{
            painter->drawRect(desRect);
            painter->drawText(desRect.x(),desRect.y()-3,strText);
        }
    }

    if(mYouSeeParse != nullptr){
        QMap<QString,QVariant> map;
        map.insert("parType","temp");
        map.insert("tempValue",mRenderImginfo.areaMaxtemp);
        emit signal_areaMaxtemp(map);
    }
    // DebugLog::getInstance()->writeLog("painter hongwai end***");
}

void XVideoTemp::fun_updateDate(){

    if(mYouSeeParse != nullptr){
        QMap<QString,QVariant> map;
        map.insert("cmd","setDate");
        emit signal_parSet(map);
    }
}

void XVideoTemp::fun_timeSwitch(bool isChecked){

}


void XVideoTemp::fun_temSet(QVariant mvalue){
    warnTemp = mvalue.toFloat();
}

void XVideoTemp::fun_temDrift(QVariant mvalue)
{
    YouSeeParse::temp_offset = mvalue.toFloat()/2;
    qDebug()<<"dsadsa"<<YouSeeParse::temp_offset;
}


XVideoTemp::~XVideoTemp()
{
    qDebug()<< " 析构   XVideoTemp";
    finishYouPull();
    if(mRenderImginfo.pImg != nullptr){
        delete mRenderImginfo.pImg;
    }

}

