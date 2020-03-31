﻿#include "XVideo.h"
#include <QPainter>
#include <QDebug>
#include <render/texturenode.h>
#include <QPainter>
QVariantList XVideo::listRectInfo;//tcp流画矩形使用的流
XVideo::XVideo()
{
    //setFlag(QQuickItem::ItemHasContents);

    initVariable();



    connect(&timerUpdate,&QTimer::timeout,this,&XVideo::slot_timeout);

    timerUpdate.start(20);
    QSize size;
    size.setWidth(640);
    size.setHeight(360);

    m_Imginfo.pImg = nullptr;

    // m_renderThread = new RenderThread(size,&listYuv,&yuvData,nullptr);
}

void XVideo::startNormalVideo()
{
    qDebug()<<"startNormalVideo ";
    createSearchIp();

}

void XVideo::startTemperatureVideo()
{
    qDebug()<<"startTemperatureVideo ";
    createYouseePull();
}
void XVideo::createYouseePull()
{
    if(mYouSeeParse == nullptr){
        youseeThread = new QThread;
        mYouSeeParse = new YouSeeParse;
        mYouSeeParse->moveToThread(youseeThread);
        mYouSeeParse->setList(listImgInfo);
        connect(this,&XVideo::signal_startinit,mYouSeeParse,&YouSeeParse::slot_init);
        connect(this,&XVideo::signal_stop,mYouSeeParse,&YouSeeParse::slot_stopPlay);
        connect(this,&XVideo::signal_getInitPar,mYouSeeParse,&YouSeeParse::slot_getInitPar);

        connect(youseeThread,&QThread::finished,youseeThread,&QThread::deleteLater);

        youseeThread->start();
    }
    emit signal_startinit();

}
void XVideo::finishYouPull()
{
    if(mYouSeeParse != nullptr){

        mYouSeeParse->forceFinish();
        youseeThread->exit();
        youseeThread->wait();
        youseeThread = nullptr;
    }

}

void XVideo::initVariable()
{

    listImgInfo.clear();


    minBuffLen = 15;

    worker = nullptr;
    m_readThread = nullptr;

    pffmpegCodec = nullptr;


    isImgUpdate  = false;
    isPlayAudio = false;
    isRecord =false;
    isScreenShot = false;
    isFirstData = false;


    isAudioFirstPlay = true;


    isStartRecord = false;

    mshotScreenFilePath = "";

    preAudioTime = 0;

}


void XVideo::createAviRecord()
{
    if(aviRecord == nullptr){
        recordThread = new QThread;
        aviRecord = new AviRecord("");
        connect(this,&XVideo::signal_recordAudio,aviRecord,&AviRecord::slot_writeAudio);
        connect(this,&XVideo::signal_recordVedio,aviRecord,&AviRecord::slot_writeVedio);
        connect(this,&XVideo::signal_startRecord,aviRecord,&AviRecord::slot_startRecord);
        connect(this,&XVideo::signal_endRecord,aviRecord,&AviRecord::slot_endRecord);
        connect(this,&XVideo::signal_setRecordingFilePath,aviRecord,&AviRecord::slot_setAviSavePath);
        connect(recordThread,&QThread::finished,aviRecord,&AviRecord::deleteLater);
        connect(recordThread,&QThread::finished,recordThread,&QThread::deleteLater);
        aviRecord->moveToThread(recordThread);
        recordThread->start();
    }
}

void XVideo::createFFmpegDecodec()
{
    if(pffmpegCodec == nullptr)
    {
        pffmpegCodec = new FfmpegCodec;
        pffmpegCodec->vNakedStreamDecodeInit(AV_CODEC_ID_H264);
        pffmpegCodec->aNakedStreamDecodeInit(AV_CODEC_ID_PCM_ALAW,AV_SAMPLE_FMT_S16,8000,1);
        pffmpegCodec->resetSample(AV_CH_LAYOUT_MONO,AV_CH_LAYOUT_MONO,8000,44100,AV_SAMPLE_FMT_S16,AV_SAMPLE_FMT_S16,160);


        if(m_readThread != nullptr)
            connect(m_readThread,&QThread::finished,pffmpegCodec,&FfmpegCodec::deleteLater);
    }

}

void XVideo::fun_getInitPar()
{
    qDebug()<<"fun_getInitPar";
    // emit signal_getInitPar();

}


void XVideo::createTcpThread()
{
    if(worker == nullptr){
        worker = new TcpWorker();
        m_readThread = new QThread;

        connect(worker,&TcpWorker::signal_sendH264,this,&XVideo::slot_recH264,Qt::DirectConnection);
        connect(this,&XVideo::signal_connentSer,worker,&TcpWorker::creatNewTcpConnect);
        connect(m_readThread,&QThread::finished,worker,&TcpWorker::deleteLater);
        connect(m_readThread,&QThread::finished,m_readThread,&QThread::deleteLater);
        worker->moveToThread(m_readThread);
        m_readThread->start();
        emit signal_connentSer(m_ip,555);
    }
    //createHttpApi();

}

void XVideo::createHttpApi(){

    if(httpDevice == nullptr){
        httpThread = new QThread;
        httpDevice = new CHttpApiDevice("INEW-004122-JWGWM", m_ip,8564, "admin", "admin");
        connect(httpDevice, &CHttpApiDevice::signal_ReadMsg, this, &XVideo::slog_HttpmsgCb);
        connect(this, &XVideo::signal_getInitPar,httpDevice,&CHttpApiDevice::slot_httpGetInitPar);
        connect(this, &XVideo::signal_httpParSet,httpDevice,&CHttpApiDevice::slot_httpParSet);
        httpDevice->moveToThread(httpThread);
        httpThread->start();


        emit signal_getInitPar();
    }
}


void XVideo::fun_temMax(QVariant mvalue){
    YouSeeParse::check_max_temp = mvalue.toFloat();
    qDebug()<<" check_max_temp  "<<YouSeeParse::check_max_temp;
}
void XVideo::fun_temMin(QVariant mvalue){
    YouSeeParse::check_min_temp = mvalue.toFloat();
    qDebug()<<" check_min_temp  "<<YouSeeParse::check_min_temp;
}
void XVideo::fun_temOffset(QVariant mvalue){
    YouSeeParse::temp_offset = mvalue.toFloat();
    qDebug()<<" temp_offset  "<<YouSeeParse::temp_offset;
}

void XVideo::slog_HttpmsgCb(QMap<QString,QVariant> map) {

    qDebug()<<" slog_HttpmsgCb ";
    emit signal_httpUiParSet(QVariant::fromValue(map));

}

void XVideo::createSearchIp()
{
    if(psearch == nullptr){
        psearch = new MySearch1;
        searchThread = new QThread;
        connect(psearch,&MySearch1::signal_sendIp,this,&XVideo::recSearchIp);
        connect(this,&XVideo::signal_startSearch,psearch,&MySearch1::startSearch);
        connect(this,&XVideo::signal_resetSearch,psearch,&MySearch1::resetSearch);
        connect(this,&XVideo::signal_finishSearch,psearch,&MySearch1::forceFinishSearch);
        connect(searchThread,&QThread::finished,searchThread,&MySearch1::deleteLater);
        connect(searchThread,&QThread::finished,psearch,&MySearch1::deleteLater);
        psearch->moveToThread(searchThread);
        searchThread->start();
    }
    m_ip = "10.67.1.55";
    emit signal_resetSearch();


    QTimer::singleShot(2000,[&]{
        if(m_ip == ""){
            qDebug()<<"*************";
            //emit signal_resetSearch();

        }else{
            createTcpThread();
        }

    });

}

void XVideo::recSearchIp(QString ip)
{
    qDebug()<<"my recSearchIp:"<<ip;

    m_ip = "10.67.1.55";



}

void XVideo::funScreenShot()
{
    //    isScreenShot = true;
    //    if(m_Img != nullptr && (!m_Img->isNull())){

    //        QString filename;

    //        QDir dir;
    //        if(mshotScreenFilePath == ""){

    //            mshotScreenFilePath = dir.absolutePath() +"/ScreenShot";
    //        }
    //        QString desFileDir = mshotScreenFilePath +"/" +mDid;
    //        if (!dir.exists(desFileDir))
    //        {
    //            bool res = dir.mkpath(desFileDir);
    //            qDebug() << "新建最终目录是否成功:" << res;
    //        }

    //        QString curTimeStr = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    //        QString tmpFileName = mDid+"_" + curTimeStr+".png";

    //        filename = desFileDir + "/"+tmpFileName;

    //        qDebug()<<" filename    "<<filename;

    //        if(m_Img->save(filename,0))
    //            qDebug()<<"图片保存成功";
    //        else
    //            qDebug()<<"图片保存失败";
    //    }
}


void XVideo::slot_timeout()
{
    //qDebug()<<"slot_timeout thread:"<<QThread::currentThreadId()<<" "<<listImgInfo.size();
    // qDebug()<<"slot_timeout thread:" <<listImgInfo.size();
    int size = listImgInfo.size();
    if(size >= 2){

        //如果不增加这句代码 ，则会出现视频不会第一时间显示，而是显示灰色图像
        if(!isFirstData){

            emit signal_loginStatus("Get the stream successfully");
            isFirstData = true;
        }


        if(listImgInfo.first().pImg != nullptr){
            if( m_Imginfo.pImg != nullptr)
                delete m_Imginfo.pImg;
            m_Imginfo = listImgInfo.takeFirst();
            update();
        }
    }


}

#include <QFontMetrics>

void XVideo::paint(QPainter *painter)
{

    QFont font("Microsoft Yahei", 20);
    QFontMetrics fm(font);
    //    int pixelsWide = fm.horizontalAdvance("What's the width of this text?");
    //    int pixelsHigh = fm.height();

    QPen pen(QBrush(QColor(0,255,0)),1);
    painter->setPen(pen);
    painter->setFont(font);

    if(m_Imginfo.pImg != nullptr){

        painter->drawImage(QRect(0,0,width(),height()), *m_Imginfo.pImg);

        if(m_Imginfo.isDrawLine){
            qreal kX = (qreal)this->width()/(qreal)384;
            qreal kY = (qreal)this->height()/(qreal)288;

           // QVariantList listVar;
            listRectInfo.clear();
            for(int i=0;i<m_Imginfo.listRect.size();i++){
                RectInfo oriRectinfo = m_Imginfo.listRect.at(i);
                QRect oriRect = oriRectinfo.rect;
                QRectF desRect(oriRect.x()*kX,oriRect.y()*kY,oriRect.width()*kX,oriRect.height()*kY);
                QString strText = QString::number(oriRectinfo.temp, 'f', 1);

                QVariantMap map;
                map.insert("rect",desRect);
                map.insert("temp",strText);
                listRectInfo.append(map);

                painter->drawRect(desRect);
                painter->drawText(desRect.x(),desRect.y()-3,strText);
            }
            //qDebug()<<"yousee 矩形:"<<listRectInfo.size();

            if(mYouSeeParse != nullptr){
                QMap<QString,QVariant> map;
                map.insert("parType","temp");
                map.insert("tempValue",m_Imginfo.temp);
                emit signal_tempPar(map);
                //emit signal_sendListRect(listVar);
                //qDebug()<<"yousee 流线程:"<<QThread::currentThreadId();
            }
        }else{

            //qDebug()<<"tcp 流线程:"<<QThread::currentThreadId();
            //qDebug()<<"tcp 矩形:"<<listRectInfo.size();
            for (int i=0;i<listRectInfo.size();i++) {
                QVariantMap vmap = listRectInfo.at(i).toMap();
                QRectF desRect = vmap.value("rect").toRectF();
                QString strText = vmap.value("temp").toString();
                painter->drawRect(desRect);
                painter->drawText(desRect.x(),desRect.y()-3,strText);
            }
        }



    }
}

//tcpworker 线程
void XVideo::slot_recH264(char* h264Arr,int arrlen,quint64 time)
{

    // qDebug()<<QString(__FUNCTION__) + "    "+QString::number(__LINE__) ;
    createFFmpegDecodec();

    if(pffmpegCodec != nullptr){

        QImage *Img = nullptr;
        if(pffmpegCodec != nullptr){
            Img = pffmpegCodec->decodeVFrame((unsigned char*)h264Arr,arrlen);

            // qDebug()<<QString(__FUNCTION__) + "    "+QString::number(__LINE__) ;

            if (Img != nullptr && (!Img->isNull()))
            {

                ImageInfo imgInfo;
                imgInfo.pImg = Img;
                imgInfo.time = time;

//                if(YouSeeParse::listImgtmpInfo->size() > 0){
//                    ImageInfo tmpIf = YouSeeParse::listImgtmpInfo->at(0);
//                    imgInfo.listRect = tmpIf.listRect;
//                    imgInfo.isDrawLine = true;
//                }

                //qDebug()<<QString(__FUNCTION__) + "    "+QString::number(__LINE__) ;
                if(listImgInfo.size() < minBuffLen){

                    listImgInfo.append(imgInfo);

                }else
                    delete Img;
            }
        }

    }
}

void XVideo::fun_getListRect(QVariant var){
    qDebug()<<"tcp 流线程 fun_getListRect:"<<QThread::currentThreadId()<<"    "<<var;
    if(worker != nullptr)
    {
        listRectInfo = var.toList();
    }
}


//tcpworker 线程
void XVideo::slot_recPcmALaw( char * buff,int len,quint64 time)
{
    preAudioTime = time;
    createFFmpegDecodec();

    // emit signal_recordAudio(buff,len,time);

    //声卡准备
    if(isAudioFirstPlay){
        isAudioFirstPlay = false;
        //emit signal_preparePlayAudio(44100,0,0,1,0);
        return;
    }else {
        if(pffmpegCodec != nullptr){
            QByteArray arr;
            pffmpegCodec->decodeAFrame((unsigned char*)buff,len,arr);


            //qDebug()<<"signal_playAudio";
            //emit signal_playAudio((unsigned char*)arr.data(),arr.length(),time);
        }
    }
}


void XVideo::fun_timeSwitch(bool isChecked){
    qDebug()<< "    fun_timeSwitch  "<<isChecked;
    QMap<QString , QVariant> map;
    map.insert("cmd","setosdparam");
    map.insert("enable",isChecked);
    emit signal_httpParSet(map);
}
void XVideo::fun_warnSwith(bool mchecked){

}
void XVideo::fun_screenShotSwith(bool mchecked){

}
void XVideo::fun_beerSwith(bool mchecked){

}
void XVideo::fun_recordSwith(bool mchecked){

}
void XVideo::fun_temSet(QVariant mvalue){

}
void XVideo::fun_screenShotPathSet(QVariant mvalue){

}
void XVideo::fun_recordPathSet(QVariant mvalue){

}
void XVideo::fun_temDrift(QVariant mvalue)
{
    YouSeeParse::temp_offset = mvalue.toFloat()/2;
//        qDebug()<<" fun_temDrift    ";
//        if(mYouSeeParse != nullptr){
//            float temDrift = mvalue.toFloat();
//            bool isSucc = mYouSeeParse->slot_setTemOffset(temDrift);
//            qDebug()<<" fun_temDrift    "<<isSucc;
//        }
}

void XVideo::funSetShotScrennFilePath(QString str)
{
    mshotScreenFilePath = str;
}

void XVideo::funSetRecordingFilePath(QString str)
{

}

XVideo::~XVideo()
{
    qDebug()<<mDid + " 析构   XVideo";

    //析构tcpworker

    if(worker != nullptr)
    {


        m_readThread->quit();

    }

    if(pffmpegCodec != nullptr)
        pffmpegCodec->deleteLater();


    qDebug()<<mDid + " 析构   XVideo 结束";
}

