#include "XVideo.h"
#include <QPainter>
#include <QDebug>
#include <render/texturenode.h>
#include <QPainter>

XVideo::XVideo()
{
    qDebug()<<"XVideo "<<"  "<<QThread::currentThreadId() ;

    pRenderImginfo = new ImageInfo;
    pRenderImginfo->pImg = nullptr;
}

void XVideo::startNormalVideo(float tp)
{
    DebugLog::getInstance()->writeLog("startNormalVideo ");
    warnTemp = tp;
    createTcpThread();
    //createSearchIp();

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
    createHttpApi();

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

void XVideo::slog_HttpmsgCb(QMap<QString,QVariant> map) {

    qDebug()<<" slog_HttpmsgCb ";
    emit signal_httpUiParSet(QVariant::fromValue(map));

}

void XVideo::createSearchIp()
{
//    if(psearch == nullptr){
//        psearch = new MySearch1;
//        searchThread = new QThread;
//        connect(psearch,&MySearch1::signal_sendIp,this,&XVideo::recSearchIp);
//        connect(this,&XVideo::signal_resetSearch,psearch,&MySearch1::resetSearch);
//        connect(this,&XVideo::signal_finishSearch,psearch,&MySearch1::forceFinishSearch);
//        connect(searchThread,&QThread::finished,searchThread,&MySearch1::deleteLater);
//        connect(searchThread,&QThread::finished,psearch,&MySearch1::deleteLater);
//        psearch->moveToThread(searchThread);
//        searchThread->start();
//    }

//    emit signal_resetSearch();

}

void XVideo::recSearchIp(QString ip)
{

    DebugLog::getInstance()->writeLog("my recSearchIp:"+ip);
    //qDebug()<<"my recSearchIp:"<<ip;

    //m_ip = "10.67.1.146";//ip;//"192.168.1.101";
    //createTcpThread();

}


void XVideo::updateUi()
{

    //buff没有数据则返回
    buffMutex.lock();
    if( pBuffImg ==  nullptr){
        buffMutex.unlock();
        return;
    }
    //buff指针有数据而渲染指针没有数据则删除老数据，在更新
    if(pRenderImginfo->pImg != nullptr){

        delete  pRenderImginfo->pImg;
        pRenderImginfo->pImg = nullptr;
    }
    pRenderImginfo->pImg = pBuffImg;
    pBuffImg = nullptr;
    buffMutex.unlock();
    //如果不增加这句代码 ，则会出现视频不会第一时间显示，而是显示灰色图像
    if(!isFirstData){
        emit signal_loginStatus("Get the stream successfully");
        isFirstData = true;
    }
    update();
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

    if(pRenderImginfo == nullptr || pRenderImginfo->pImg == nullptr)
        return;
    QFont font("Microsoft Yahei", 20);
    QPen pen(QBrush(QColor(0,255,0)),1);
    painter->setPen(pen);
    painter->setFont(font);


    // DebugLog::getInstance()->writeLog("painter kejianguang start***");
    qreal kX = (qreal)this->width()/(qreal)384;
    qreal kY = (qreal)this->height()/(qreal)288;

    qreal kshowRectX = (qreal)this->width()/showParentW;
    qreal kshowRectY = (qreal)this->height()/showParentH;

    painter->drawImage(QRect(0,0,width(),height()), *pRenderImginfo->pImg);

    //画限制区域矩形
    painter->save();
    QRectF rectF(showRectX * kshowRectX,showRectY*kshowRectY,showRectW*kshowRectX,showRectH*kshowRectY);
    painter->setPen(QPen(QBrush(QColor(255,0,0)),3));
    painter->drawRect(rectF);
    painter->restore();

    qreal kshowX = (qreal)rectF.width()/(qreal)this->width();
    qreal kshowY = (qreal)rectF.height()/(qreal)this->height();


    //画温度矩形
    for (int i=0;i<pRenderImginfo->listRect.size();i++) {
        QVariantMap vmap = pRenderImginfo->listRect.at(i);
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


            //qDebug()<<"h264:"<<listImgInfo.size();
            if (Img != nullptr && (!Img->isNull()))
            {

                buffMutex.lock();
                if(pBuffImg == nullptr){
                    pBuffImg = Img;

                }else{
                    delete Img;

                }
                buffMutex.unlock();
            }
        }

    }
}
//由红外控制ui更新
void XVideo::fun_setListRect(QVariant var){
    //qDebug()<<"tcp 流线程 fun_setListRect:"<<QThread::currentThreadId()<<"    "<<var.toList();

    if(pRenderImginfo != nullptr){

        if(var.toList().size() > 0){
            pRenderImginfo->listRect.clear();
            QVariantList listv = var.toList();
            for(int i=0;i<listv.size();i++){
                QVariantMap map = listv.at(i).toMap();
                pRenderImginfo->listRect.append(map);
            }
        }
        updateUi();
    }else{
        // qDebug()<<"渲染图片为空";

    }
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


}

