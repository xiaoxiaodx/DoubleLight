#include "j07device.h"
#include "xvideotemp.h"

J07Device::J07Device(QString ip,int type,QObject *parent) : QObject(parent)
{
    m_ip = ip;
    m_type = type;
}

void J07Device::startRec()
{
    createTcpThread();
}

void J07Device::createTcpThread()
{
    if(worker == nullptr){
        worker = new TcpWorker(m_type);
        m_readThread = new QThread;

        // connect(worker,&TcpWorker::signal_sendH264,this,&J07Device::slot_recH264,Qt::DirectConnection);
        connect(worker,&TcpWorker::signal_sendImg,this,&J07Device::slot_recImg,Qt::DirectConnection);
        connect(this,&J07Device::signal_connentSer,worker,&TcpWorker::creatNewTcpConnect);
        connect(m_readThread,&QThread::finished,worker,&TcpWorker::deleteLater);
        connect(m_readThread,&QThread::finished,m_readThread,&QThread::deleteLater);
        worker->moveToThread(m_readThread);
        m_readThread->start();

        emit signal_connentSer(m_ip,556);
    }
}


void J07Device::slot_recImg(QImage *img,int len,quint64 time,int resw,int resh){

   // qDebug()<<"slot_recImg";
    if(img != nullptr){
        ImageInfo info;
        info.pImg =img;
        info.listRect.clear();
        XVideoTemp::mutex.lock();
        if(XVideoTemp::listBufferImginfo.size() < XVideoTemp::maxBuffLen)
            XVideoTemp::listBufferImginfo.append(info);
        else{
            if(info.pImg != nullptr)
                delete info.pImg;
        }
        XVideoTemp::mutex.unlock();
    }
}


//tcpworker 线程  rgba数据
void J07Device::slot_recH264(char* h264Arr,int arrlen,quint64 time,int resw,int resh)
{

   // qDebug()<<QString(__FUNCTION__) <<"***/***    "<<arrlen<< " "<<resw<<"  "<<resh;

    if(rgbBuff == nullptr)
        rgbBuff = new char[resw*resh*4];

    memcpy(rgbBuff,h264Arr,resw*resh*4);
    QImage *pImage = nullptr;
    try {

        pImage = new QImage((unsigned char*)rgbBuff,resw, resh, QImage::Format_RGB32);

        // 其它代码
    } catch ( const std::bad_alloc& e ) {

        qDebug()<<"图片分配内存失败     ";
        pImage = nullptr;
    }
    ImageInfo info;
    info.listRect.clear();
    XVideoTemp::mutex.lock();
    if(XVideoTemp::listBufferImginfo.size() < XVideoTemp::maxBuffLen)
        XVideoTemp::listBufferImginfo.append(info);
    else{
        if(info.pImg != nullptr)
            delete info.pImg;
    }

    XVideoTemp::mutex.unlock();
}


J07Device::~J07Device()
{
    qDebug()<<" 析构   XVideotemp";

    //析构tcpworker

    if(worker != nullptr)
    {
        worker->forceStopParse();
        m_readThread->quit();
    }




}
