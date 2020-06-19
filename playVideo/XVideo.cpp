#include "XVideo.h"
#include <QPainter>
#include <QDebug>
#include <QPainter>


XVideo::XVideo()
{
    qDebug()<<"XVideo "<<"  "<<QThread::currentThreadId() ;

    pRenderImginfo.pImg = nullptr;

    connect(&timerUpdate,&QTimer::timeout,this,&XVideo::slot_timeoutUpdate);
}


void XVideo::fun_setInitPar(QString ip,int parentW,int parentH,int x,int y,int w,int h)
{
    m_ip = ip;
    showRectX = x;
    showRectY = y;
    showRectW = w;
    showRectH = h;
    showParentW = parentW;
    showParentH = parentH;

    qDebug()<<" fun_setInitPar  "<<showRectX<<" "<<showRectY;
}
void XVideo::funStartSearch()
{
    listdeivceinfo.clear();
    createSearchIp();
}

void XVideo::slot_timeoutUpdate(){

    //qDebug()<<"tcp 流线程 fun_setListRect:"<<QThread::currentThreadId()<<"    "<<listBuffImg.size();
    if(mMutex.tryLock()){

        if(listBuffImg.size()>0){
            if(pRenderImginfo.pImg != nullptr){
                delete pRenderImginfo.pImg;
                pRenderImginfo.pImg = nullptr;
            }
            QImage *img = listBuffImg.takeFirst();
            pRenderImginfo.pImg = img;
        }
        mMutex.unlock();
    }

    if(pRenderImginfo.pImg == nullptr)
        return;

    //如果不增加这句代码 ，则会出现视频不会第一时间显示，而是显示灰色图像
    if(!isFirstData){
        emit signal_loginStatus("start rec stream ");
        isFirstData = true;
    }

    update();
}

void XVideo::startNormalVideo(float tp,QString deviceinfo)
{
    DebugLog::getInstance()->writeLog("startNormalVideo " + deviceinfo);

    QStringList listip = deviceinfo.split(".");

    if(listip.size() >= 4){

        //if(m_ip.compare(deviceinfo)!=0){

            destroyAllFunction();
       // }
        m_ip = deviceinfo;
        warnTemp = tp;
        createTcpThread();
        return;
    }


    for (int i=0;i<listdeivceinfo.size();i++) {

        QVariantMap map = listdeivceinfo.at(i);
        if(deviceinfo.compare(map.value("uuid").toString())==0){
            QString curip = map.value("ip").toString();

           // if(m_ip.compare(curip)!=0){
                destroyAllFunction();
           // }
            m_ip = curip;
            warnTemp = tp;
            createTcpThread();
        }
    }
}


void XVideo::destroyAllFunction()
{
    //关闭更新定时器
    if(timerUpdate.isActive())
        timerUpdate.stop();


    if(httpDevice != nullptr){

        emit signal_destroyHttp();
        httpThread->quit();
        if(httpThread->wait(2000)){
            qDebug()<<"http线程结束成功";
        }else
            qDebug()<<"http线程结束失败";
        httpThread = nullptr;
        httpDevice =nullptr;
    }

    //结束流线程
    if(worker != nullptr)
    {
        worker->forceStopParse();
        m_readThread->quit();
        if(m_readThread->wait(2000)){

            //清空缓存
            mMutex.lock();
            for(int i=0;i<listBuffImg.size();i++){
                QImage *img = listBuffImg.at(i);
                if(!img->isNull() || img!= nullptr)
                    delete img;
            }
            listBuffImg.clear();
            mMutex.unlock();

            if(pRenderImginfo.pImg != nullptr){
                delete pRenderImginfo.pImg;
                pRenderImginfo.pImg = nullptr;
            }
            worker = nullptr;
            m_readThread = nullptr;
            qDebug()<<"tcp线程结束成功";
        }else
            qDebug()<<"tcp线程结束失败";

    }


    if(pffmpegCodecH264 != nullptr){
        pffmpegCodecH264 = nullptr;
    }
    if(pffmpegCodecH265 != nullptr){
        pffmpegCodecH265 = nullptr;
    }

}

FfmpegCodec * XVideo::createFFmpegDecodec(QString type)
{

    FfmpegCodec  *pffmpegCodec = new FfmpegCodec;
    if(type.compare("h264")==0)
        pffmpegCodec->vNakedStreamDecodeInit(AV_CODEC_ID_H264);
    else if(type.compare("h265")==0)
        pffmpegCodec->vNakedStreamDecodeInit(AV_CODEC_ID_H265);

    pffmpegCodec->aNakedStreamDecodeInit(AV_CODEC_ID_PCM_ALAW,AV_SAMPLE_FMT_S16,8000,1);
    pffmpegCodec->resetSample(AV_CH_LAYOUT_MONO,AV_CH_LAYOUT_MONO,8000,44100,AV_SAMPLE_FMT_S16,AV_SAMPLE_FMT_S16,160);

    if(m_readThread != nullptr)
        connect(m_readThread,&QThread::finished,pffmpegCodec,&FfmpegCodec::deleteLater);
    return pffmpegCodec;
}

void XVideo::fun_sendCommonPar(QVariantMap map)
{
    emit signal_httpParSet(map);
}
void XVideo::fun_setIraInfo(QVariantMap map)
{
    emit signal_httpParSet(map);
}

void XVideo::fun_getInitPar()
{

}


void XVideo::createTcpThread()
{
    if(worker == nullptr){
        worker = new TcpWorker(0);
        m_readThread = new QThread;

        connect(worker,&TcpWorker::signal_sendH264,this,&XVideo::slot_recH264,Qt::DirectConnection);
        connect(worker,&TcpWorker::signal_sendH265,this,&XVideo::slot_recH265,Qt::DirectConnection);
        connect(this,&XVideo::signal_connentSer,worker,&TcpWorker::creatNewTcpConnect);
        connect(worker,&TcpWorker::signal_connected,this,&XVideo::slot_tcpConnected);

        connect(m_readThread,&QThread::finished,worker,&TcpWorker::deleteLater);
        connect(m_readThread,&QThread::finished,m_readThread,&QThread::deleteLater);
        worker->moveToThread(m_readThread);
        m_readThread->start();



        emit signal_connentSer(m_ip,555);
    }
}

void XVideo::slot_tcpConnected()
{
    if(!timerUpdate.isActive())
        timerUpdate.start();
    emit signal_connected(true,m_ip);
    createHttpApi();
}

void XVideo::createHttpApi(){


    if(httpDevice == nullptr){

        qDebug()<<" createHttpApi   "<<m_ip;
        httpThread = new QThread;
        httpDevice = new CHttpApiDevice("INEW-004122-JWGWM", m_ip,8564, "gzuser@u", "gzuser@p");
        connect(httpDevice, &CHttpApiDevice::signal_ReadMsg, this, &XVideo::slog_HttpmsgCb);
        connect(this, &XVideo::signal_getInitPar,httpDevice,&CHttpApiDevice::slot_httpGetInitPar);
        connect(this, &XVideo::signal_httpParSet,httpDevice,&CHttpApiDevice::slot_httpParSet);
        connect(this, &XVideo::signal_createHttp,httpDevice,&CHttpApiDevice::createConnect);
        connect(this, &XVideo::signal_destroyHttp,httpDevice,&CHttpApiDevice::slot_destoryConnect);
        connect(httpDevice, &CHttpApiDevice::signal_httpConnected,this,&XVideo::slot_httpConnected);

        connect(httpThread, &QThread::finished,httpDevice,&CHttpApiDevice::deleteLater);
        connect(httpThread, &QThread::finished,httpThread,&QThread::deleteLater);

        httpDevice->moveToThread(httpThread);
        httpThread->start();

        emit signal_createHttp();


    }else
        emit signal_createHttp();

}
//获取型号后在获取其他参数
void XVideo::slot_httpConnected()
{

    QMap<QString,QVariant> map;
    map.insert("cmd","getinftempmodel");
    emit signal_httpParSet(map);
}

void XVideo::slog_HttpmsgCb(QMap<QString,QVariant> map) {

    DebugLog::getInstance()->writeLog("http_RecCallback :"+ map.value("cmd").toString());
    emit signal_httpUiParSet(QVariant::fromValue(map));
}

void XVideo::createSearchIp()
{
    if(psearch == nullptr){
        psearch = new MySearch1;


        connect(psearch,&MySearch1::signal_sendDeviceinfo,this,&XVideo::recSearchDeviceinfo);

    }
    psearch->createSearch();
}

void XVideo::recSearchDeviceinfo(QVariantMap info)
{

    info.insert("cmd","deviceinfo");

    listdeivceinfo.append(info);

    emit signal_httpUiParSet(info);
}

void XVideo::updateUi()
{



}

void XVideo::fun_updateDate()
{
    QMap<QString,QVariant> map;
    map.insert("cmd","setcurrenttime");
    emit signal_httpParSet(map);

}

void XVideo::paint(QPainter *painter)
{

    //qDebug()<<"XVideo paint";
    if(pRenderImginfo.pImg == nullptr)
        return;

    QFont font("Microsoft Yahei", 20);
    QPen pen(QBrush(QColor(0,255,0)),1);
    painter->setPen(pen);
    painter->setFont(font);

//    qDebug()<<""<<tempImgResW;


    painter->drawImage(QRect(0,0,width(),height()), *pRenderImginfo.pImg);


    return;


    qreal kX = (qreal)this->width()/(qreal)tempImgResW;
    qreal kY = (qreal)this->height()/(qreal)tempImgResH;

    qreal kshowRectX = (qreal)this->width()/showParentW;
    qreal kshowRectY = (qreal)this->height()/showParentH;

    //画限制区域矩形
    painter->save();
    QRectF rectF(showRectX * kshowRectX,showRectY*kshowRectY,showRectW*kshowRectX,showRectH*kshowRectY);
    painter->setPen(QPen(QBrush(QColor(255,0,0)),3));
    painter->drawRect(rectF);
    painter->restore();

    qreal kshowX = (qreal)rectF.width()/(qreal)this->width();
    qreal kshowY = (qreal)rectF.height()/(qreal)this->height();

    //画温度矩形
    for (int i=0;i<pRenderImginfo.listRect.size();i++) {
        QVariantMap vmap = pRenderImginfo.listRect.at(i);
        QRectF oriRect = vmap.value("rect").toRectF();

        QRectF desRect(rectF.x()+oriRect.x()*kX*kshowX,rectF.y()+oriRect.y()*kshowY*kY,oriRect.width()*kX*kshowX,oriRect.height()*kY*kshowY);


        float temp = vmap.value("temp").toFloat();
        QString strText = QString::number(temp, 'f', 1);

        //高温标记矩形
        if(strText.toFloat()>warnTemp){
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
    // DebugLog::getInstance()->writeLog("painter kejianguang end***");
}

void XVideo::fun_setRectPar(int sx,int sy,int sw,int sh,int pw,int ph){

    showRectX = sx;
    showRectY = sy;
    showRectW = sw;
    showRectH = sh;

    showParentW = pw;
    showParentH = ph;

    qreal kx = (qreal)showParentW / (qreal)1920;
    qreal ky = (qreal)showParentH / (qreal)1080;

    qreal x1 = showRectX / kx;
    qreal y1 = showRectY / ky;
    qreal w = showRectW / kx;
    qreal h = showRectH / ky;

    int desX = x1/2;
    int desY = y1/2;
    int desW = w/2;
    int desH = h/2;


    QMap<QString,QVariant> map;
    map.insert("cmd","setmeasurablerange");
    map.insert("x",desX*2);
    map.insert("y",desY*2);
    map.insert("w",desW*2);
    map.insert("h",desH*2);


    emit signal_httpParSet(map);


    QString pos1 =  "pos1:"+ QString::number(x1,'f',1)+"    "+QString::number(y1,'f',3);
    QString pos2 = "pos2:"+QString::number(x1+w,'f',1)+"    "+QString::number(y1,'f',3);
    QString pos3 = "pos3:"+QString::number(x1,'f',1)+"  "+QString::number(y1+h,'f',3);
    QString pos4 = "pos4:"+QString::number(x1+w,'f',1)+"    "+QString::number(y1+h,'f',3);

    QString str1920 ="1920*1080 rect:"+QString::number(x1)+" "+QString::number(y1)+" "+QString::number(w)+"  "+QString::number(h);

    DebugLog::getInstance()->writeLog(str1920);
    DebugLog::getInstance()->writeLog(pos1);
    DebugLog::getInstance()->writeLog(pos2);
    DebugLog::getInstance()->writeLog(pos3);
    DebugLog::getInstance()->writeLog(pos4);
}



//tcpworker 线程
void XVideo::slot_recH264(char* h264Arr,int arrlen,quint64 time)
{

    //qDebug()<<"slot_recH264 ";
    if(pffmpegCodecH264 == nullptr)
       pffmpegCodecH264 = createFFmpegDecodec("h264");


    if(pffmpegCodecH264 != nullptr){

        QImage *Img = nullptr;

            Img = pffmpegCodecH264->decodeVFrame((unsigned char*)h264Arr,arrlen);
            if (Img != nullptr )
            {
                mMutex.lock();
               // qDebug()<<"图片为空1:"<<listBuffImg.size();
                if(listBuffImg.size() < maxBuffLen)
                    listBuffImg.append(Img);
                else
                    delete Img;
                mMutex.unlock();
            }else{

                qDebug()<<"图片为空";
            }
    }
}

void XVideo::slot_recH265(char* h264Arr,int arrlen,quint64 time)
{
    if(pffmpegCodecH265 == nullptr)
     pffmpegCodecH265 = createFFmpegDecodec("h265");
    if(pffmpegCodecH265 != nullptr){

        QImage *Img = nullptr;

            Img = pffmpegCodecH265->decodeVFrame((unsigned char*)h264Arr,arrlen);
            if (Img != nullptr )
            {
                mMutex.lock();
                if(listBuffImg.size() < maxBuffLen)
                    listBuffImg.append(Img);
                else
                    delete Img;
                mMutex.unlock();
            }

    }
}

//由红外控制ui更新
void XVideo::fun_setListRect(QVariant var){
    //    //qDebug()<<"tcp 流线程 fun_setListRect:"<<QThread::currentThreadId()<<"    "<<var.toList();
    //    if(mMutex.tryLock()){

    //        if(listBuffImg.size()>0){
    //            if(pRenderImginfo.pImg != nullptr){
    //                delete pRenderImginfo.pImg;
    //                pRenderImginfo.pImg = nullptr;
    //            }
    //            QImage *img = listBuffImg.takeFirst();
    //            pRenderImginfo.pImg = img;
    //        }
    //        mMutex.unlock();
    //    }

    //    if(pRenderImginfo.pImg == nullptr)
    //        return;

    //    //如果不增加这句代码 ，则会出现视频不会第一时间显示，而是显示灰色图像
    //    if(!isFirstData){
    //        emit signal_loginStatus("Get the stream successfully");
    //        isFirstData = true;
    //    }

    pRenderImginfo.listRect.clear();
    if(var.toList().size() > 0){
        QVariantList listv = var.toList();
        for(int i=0;i<listv.size();i++){
            QVariantMap map = listv.at(i).toMap();
            pRenderImginfo.listRect.append(map);
        }
    }


}

void XVideo::fun_initRedFrame(int w,int h){


    tempImgResW = w;
    tempImgResH = h;

    //    if(w == 384 && h==288){
    //        showRectX = 65;
    //        showRectY = 41;
    //        showRectW = 349;
    //        showRectH = 327;
    //        showParentW = 494;
    //        showParentH = 369;
    //    }else if(w == 160 && h==120){
    //        showRectX = 192;
    //        showRectY = 107;
    //        showRectW = 720;
    //        showRectH = 605;
    //        showParentW = 954;
    //        showParentH = 714;
    //    }
}


void XVideo::fun_timeSwitch(bool isChecked){
    qDebug()<< "    fun_timeSwitch  "<<isChecked;
    QMap<QString , QVariant> map;
    map.insert("cmd","setosdparam");
    map.insert("enable",isChecked);
    emit signal_httpParSet(map);
}

void XVideo::fun_temSet(QVariant mvalue){
    warnTemp = mvalue.toFloat();
}

void XVideo::fun_temMin(QVariant mvalue)
{

}
void XVideo::fun_temOffset(QVariant mvalue)
{

}

XVideo::~XVideo()
{
    qDebug()<<" 析构   XVideo";

    //析构tcpworker

    if(worker != nullptr)
    {
        worker->forceStopParse();
        m_readThread->quit();
    }

    if(pffmpegCodecH264 != nullptr)
        pffmpegCodecH264->deleteLater();
    if(pffmpegCodecH265 != nullptr)
        pffmpegCodecH265->deleteLater();

    if(httpDevice != nullptr){

        httpThread->quit();
    }

}

