#include "XVideo.h"
#include <QPainter>
#include <QDebug>
#include <render/texturenode.h>
#include <QPainter>

XVideo::XVideo()
{
    qDebug()<<"XVideo "<<"  "<<QThread::currentThreadId() ;
    pRenderImginfo.pImg = nullptr;
    //createSearchIp();
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

void XVideo::startNormalVideo(float tp)
{
    DebugLog::getInstance()->writeLog("startNormalVideo ");
    warnTemp = tp;
    createTcpThread();
    //createSearchIp();
    connect(&timerUpdate,&QTimer::timeout,this,&XVideo::slot_update);
    timerUpdate.start(50);
}

void XVideo::slot_update()
{
    update();
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

void XVideo::fun_sendCommonPar(QVariantMap map)
{

    qDebug()<<"fun_sendCommonPar";
    emit signal_httpParSet(map);
}
void XVideo::fun_setIraInfo(QVariantMap map)
{
    emit signal_httpParSet(map);
}

void XVideo::fun_getInitPar()
{
    qDebug()<<"fun_getInitPar";
    QMap<QString,QVariant> map;
    //    map.insert("cmd","getinftempmodel");
    //    emit signal_httpParSet(map);

    //    map.insert("cmd","getosdparam");
    //    emit signal_httpParSet(map);

    //    map.insert("cmd","setcurrenttime");
    //    emit signal_httpParSet(map);

    //    map.insert("cmd","getiradinfo");
    //    emit signal_httpParSet(map);
}


void XVideo::createTcpThread()
{
    if(worker == nullptr){
        worker = new TcpWorker(0);
        m_readThread = new QThread;

        connect(worker,&TcpWorker::signal_sendH264,this,&XVideo::slot_recH264,Qt::DirectConnection);
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
    createHttpApi();
}

void XVideo::createHttpApi(){

    if(httpDevice == nullptr){
        httpThread = new QThread;
        httpDevice = new CHttpApiDevice("INEW-004122-JWGWM", m_ip,8564, "admin", "admin");
        connect(httpDevice, &CHttpApiDevice::signal_ReadMsg, this, &XVideo::slog_HttpmsgCb);
        connect(this, &XVideo::signal_getInitPar,httpDevice,&CHttpApiDevice::slot_httpGetInitPar);
        connect(this, &XVideo::signal_httpParSet,httpDevice,&CHttpApiDevice::slot_httpParSet);
        connect(this, &XVideo::signal_createHttp,httpDevice,&CHttpApiDevice::createConnect);
        connect(httpDevice, &CHttpApiDevice::signal_httpConnected,this,&XVideo::slot_httpConnected);

        connect(httpThread, &QThread::finished,httpDevice,&CHttpApiDevice::deleteLater);
        connect(httpThread, &QThread::finished,httpThread,&QThread::deleteLater);

        httpDevice->moveToThread(httpThread);
        httpThread->start();

        emit signal_createHttp();
        // emit signal_getInitPar();
        //fun_getInitPar();
    }
}
//获取型号后在获取其他参数
void XVideo::slot_httpConnected()
{

    QMap<QString,QVariant> map;
    map.insert("cmd","getinftempmodel");
    emit signal_httpParSet(map);
}

void XVideo::slog_HttpmsgCb(QMap<QString,QVariant> map) {

    qDebug()<<" slog_HttpmsgCb "<<map;
    emit signal_httpUiParSet(QVariant::fromValue(map));
}

void XVideo::createSearchIp()
{
    if(psearch == nullptr){
        psearch = new MySearch1;
        psearch->createSearch();
        //        searchThread = new QThread;
        connect(psearch,&MySearch1::signal_sendIp,this,&XVideo::recSearchIp);
        //        connect(this,&XVideo::signal_resetSearch,psearch,&MySearch1::resetSearch);
        //        connect(this,&XVideo::signal_finishSearch,psearch,&MySearch1::forceFinishSearch);
        //        connect(searchThread,&QThread::finished,searchThread,&MySearch1::deleteLater);
        //        connect(searchThread,&QThread::finished,psearch,&MySearch1::deleteLater);
        //        psearch->moveToThread(searchThread);
        //        searchThread->start();
    }
    //emit signal_resetSearch();

}

void XVideo::recSearchIp(QString ip)
{

    DebugLog::getInstance()->writeLog("my recSearchIp:"+ip);
    //qDebug()<<"my recSearchIp:"<<ip;
    m_ip = ip;//ip;//"192.168.1.101";
    emit signal_setIp(m_ip);
    createTcpThread();
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


    if(tempImgResW == 0)
        return;
    // DebugLog::getInstance()->writeLog("painter kejianguang start***");
    qreal kX = (qreal)this->width()/(qreal)tempImgResW;
    qreal kY = (qreal)this->height()/(qreal)tempImgResH;

    qreal kshowRectX = (qreal)this->width()/showParentW;
    qreal kshowRectY = (qreal)this->height()/showParentH;

    painter->drawImage(QRect(0,0,width(),height()), *pRenderImginfo.pImg);


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

    // qDebug()<<QString(__FUNCTION__) + " "+QString::number(__LINE__)<<"  "<<QThread::currentThreadId() ;
    createFFmpegDecodec();

    if(pffmpegCodec != nullptr){

        QImage *Img = nullptr;
        if(pffmpegCodec != nullptr){
            Img = pffmpegCodec->decodeVFrame((unsigned char*)h264Arr,arrlen);
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
}
//由红外控制ui更新
void XVideo::fun_setListRect(QVariant var){
    //qDebug()<<"tcp 流线程 fun_setListRect:"<<QThread::currentThreadId()<<"    "<<var.toList();
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
        emit signal_loginStatus("Get the stream successfully");
        isFirstData = true;
    }

    pRenderImginfo.listRect.clear();
    if(var.toList().size() > 0){
        QVariantList listv = var.toList();
        for(int i=0;i<listv.size();i++){
            QVariantMap map = listv.at(i).toMap();
            pRenderImginfo.listRect.append(map);
        }
    }
    //update();
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


XVideo::~XVideo()
{
    qDebug()<<" 析构   XVideo";

    //析构tcpworker

    if(worker != nullptr)
    {
        worker->forceStopParse();
        m_readThread->quit();
    }

    if(pffmpegCodec != nullptr)
        pffmpegCodec->deleteLater();

    if(httpDevice != nullptr){

        httpThread->quit();
    }

}

