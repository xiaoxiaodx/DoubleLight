#include "tcpworker.h"
#include<QFile>

#include "debuglog.h"
TcpWorker::TcpWorker(int type,QObject *parent) : QObject(parent)
{

    myType = type;
    initVariable();
    readDataBuff.clear();
}

void TcpWorker::initVariable()
{
    videoFrameMaxLen = 1024 * 3500;
    audioFrameMaxLen = 200;

    isReconnecting = false;
    minLen = 32;
    tcpSocket = nullptr;

    isFindHead = false;
    isFindMediaType = false;
    isSaveAudioInfo = false;
    isSaveVideoInfo = false;
    isCheckedDataLong = false;
    isConnected = false;
    isHavaData = false;
    isSendAudioData = false;

    mediaDataType = -1;
    m_streamDateLen = -1;

    timerConnectSer = nullptr;
}


void TcpWorker::slot_soceckErr(QAbstractSocket::SocketError socketError)
{

    qDebug()<<" tcp socket socketError:"<<socketError;
    qDebug()<<" tcp socket errStr:"<<tcpSocket->errorString();
}
void TcpWorker::creatNewTcpConnect(QString ip, int port)
{

    DebugLog::getInstance()->writeLog("tcp开始连接  "+ip+"  "+QString::number(port));
    this->ip = ip;
    this->port = port;
    if(tcpSocket == nullptr){
        tcpSocket = new QTcpSocket;

        timerConnectSer = new QTimer;
        connect(tcpSocket,&QTcpSocket::connected,this,&TcpWorker::slot_tcpConnected);
        connect(tcpSocket,&QTcpSocket::readyRead,this,&TcpWorker::slot_readData);
        connect(tcpSocket,&QTcpSocket::disconnected,this,&TcpWorker::slot_tcpDisconnected);
        //connect(tcpSocket,&QTcpSocket::error,this,&TcpWorker::slot_soceckErr);
        connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slot_soceckErr(QAbstractSocket::SocketError)));
        connect(timerConnectSer,&QTimer::timeout,this,&TcpWorker::slot_timerConnectSer);

        //tcpSocket->setReadBufferSize(1024*1024);
        //tcpSocket->bind(QHostAddress(this->ip),this->port,QAbstractSocket::ReuseAddressHint);
        tcpSocket->connectToHost(this->ip,this->port);
        isForceFinish = false;
        timerConnectSer->start(3000);
    }
    /*else{

        tcpSocket->connectToHost(this->ip,this->port);
        timerConnectSer->start(3000);
    }*/
    DebugLog::getInstance()->writeLog("tcp连接创建完成"+m_did);
}

void TcpWorker::slot_disConnectSer()
{

    tcpSocket->abort();
    readDataBuff.clear();
}

//定时对tcp做连接 以达到自动重连的目的
void TcpWorker::slot_timerConnectSer()
{

    QMutexLocker locker(&mMutex);
    if(isForceFinish){
        timerConnectSer->stop();
        disconnect(timerConnectSer,&QTimer::timeout,this,&TcpWorker::slot_timerConnectSer);
        timerConnectSer->deleteLater();

    }else{
        if(!isConnected){

            if(tcpSocket != nullptr){
                DebugLog::getInstance()->writeLog("start reconnect "+QString::number(myType)+" " + this->ip+"  "+QString::number(this->port));
                //qDebug()<<"开始重连 "<<myType <<"   "<<this->ip<<"  "<<this->port;
                tcpSocket->abort();
                readDataBuff.clear();
                tcpSocket->connectToHost(this->ip,this->port);
            }

        }

        if(!isHavaData){

            isConnected = false;

        }

        isHavaData = false;
    }
}



void TcpWorker::slot_tcpConnected()
{

    DebugLog::getInstance()->writeLog( " tcp连接成功");
    isConnected = true;
    slot_tcpSendAuthentication(m_did,m_usrName,m_password);

    emit signal_connected();
}

void TcpWorker::slot_tcpDisconnected()
{

    isConnected = false;

}

void TcpWorker::slot_readData()
{

    //qDebug()<<"slot_readData    "<<
    isHavaData = true;
    // isConnected = true;

    readDataBuff.append(tcpSocket->readAll());

    isCheckedDataLong = false;
    if(readDataBuff.length() > videoFrameMaxLen*2){
        qDebug()<<"readDataBuff 数据过长"<<readDataBuff.length();
        isCheckedDataLong = true;
        //        QTextStream out(debugfile);
        //        out << " data is too long: " << readDataBuff.toHex() << "\n";
        resetAVFlag();
    }
    parseRecevieData();
}

int TcpWorker::saveVideoInfo(QByteArray &arr)
{
    int index = 0;
    int fps = 0x000f & arr.at(index++);
    int rcmode = 0x000f & arr.at(index++);
    int frameType = 0x000f & arr.at(index++);
    int staty0 = 0x000f & arr.at(index++);
    //VideoReslution_T

    QByteArray arrW = arr.mid(index,4);
    int width = byteArr2Int(arrW);
    vResW = width;
    index += 4;


    QByteArray arrH = arr.mid(index,4);

    int height = byteArr2Int(arrH);

    vResH = height;
    index += 4;

    //  bitrate
    QByteArray arrBitrate = arr.mid(index,4);
    int bitrate =  byteArr2Int(arrBitrate);
    index += 4;
    //pts


    // qDebug()<< "bitrate:"<<bitrate<<"    rcmode"<<rcmode<<" fps"<<fps <<" width:"<<width<<"    height "<<height;
    index += 8;
    //裸流数据长度

    QByteArray arrDatalen = arr.mid(index,4);

    int datalen = byteArr2Int(arrDatalen);
    index += 4;

    memcpy(&infoV,arr.data(),sizeof (QueueAudioInputInfo_T));
    arr.remove(0,index);

    return datalen;
}
int TcpWorker::saveAudioInfo(QByteArray &arr)
{

    int index = 0;
    QByteArray arrS = arr.mid(index,4);
    int samplerate = byteArr2Int(arrS);
    index += 4;

    QByteArray arrP = readDataBuff.mid(index,4);
    int prenum = byteArr2Int(arrP);
    index += 4;

    QByteArray arrB = readDataBuff.mid(index,4);
    int bitwidth = byteArr2Int(arrB);
    index += 4;

    QByteArray arrSoude = readDataBuff.mid(index,4);
    int soundmode = byteArr2Int(arrSoude);
    index += 4;

    QByteArray arrH = readDataBuff.mid(index,4);
    int highPts = byteArr2Int(arrH);
    index += 4;

    QByteArray arrL = readDataBuff.mid(index,4);
    int lowPts = byteArr2Int(arrL);
    index += 4;


    QByteArray arrDatalen = readDataBuff.mid(index,4);
    int datalen = byteArr2Int(arrDatalen);
    index += 4;

    memcpy(&infoA,arr.data(),sizeof (QueueAudioInputInfo_T));
    arr.remove(0,index);

    return datalen;
}

void TcpWorker::resetAVFlag()
{
    m_streamDateLen = -1;
    mediaDataType = -1;
    isFindHead = false;
    isSaveAudioInfo = false;
    isSaveVideoInfo = false;
    isFindMediaType = false;
}

void TcpWorker::parseRecevieData()
{
    int needlen = 2;

    /*
        循环解析整个readDataBuff，把所有有效的数据都解析出来
        每次找到一个信息都会把包含该信息的字节数据删除
        needlen代表解析数据还需要的字节长度
        1、找头，
        2、找媒体信息类型
        3、找媒体裸流数据（消息回应）
    */

    while(readDataBuff.length() >= needlen)
    {

        QMutexLocker lockr(&mMutex);
        if(isForceFinish)
            break;


        if(!isFindHead)
        {

            if(readDataBuff.at(0) == D_SYNCDATA_HEAD0 && readDataBuff.at(1)==D_SYNCDATA_HEAD1)
            {

                readDataBuff.remove(0,2);
                isFindHead = true;
                needlen = 2;
            }else {
                readDataBuff.remove(0,1);
                continue;
            }
        }


        //找到头后，找媒体类型
        if(!isFindMediaType)
        {
            if(readDataBuff.length()>=needlen){
                int tmp = readDataBuff.at(1);
                mediaDataType =  tmp & 0x000000ff;
                readDataBuff.remove(0,2);


                if(mediaDataType >= MediaType_H264 && mediaDataType <= MediaType_MSG){

                    isFindMediaType = true;

                }else {//不合理的媒体类型则从新开始找头
                    resetAVFlag();
                    continue;
                }
            }
            else
                continue;
        }

        if(mediaDataType == MediaType_H264)
        {
            needlen = 28;

            if(!isSaveVideoInfo)
            {
                if(readDataBuff.length() >= needlen)
                {
                    m_streamDateLen = saveVideoInfo(readDataBuff);
                    isSaveVideoInfo = true;
                    if(m_streamDateLen > videoFrameMaxLen || m_streamDateLen <0)
                    {
                        qDebug()<<"视频帧数据长度异常:"<<m_streamDateLen;
                        resetAVFlag();
                        continue;
                    }
                }else
                    continue;
            }

            needlen = m_streamDateLen;

            if(readDataBuff.length()>=needlen)
            {

                //emit signal_writeMediaVideoQueue(readDataBuff.data(),m_streamDateLen,infoV,MediaDataProcess::mMediaVeidoType);

                quint64 ptsH = 0x00000000ffffffff & infoV.highPts;
                quint64 ptsL = 0x00000000ffffffff & infoV.lowPts;
                quint64 pts = ptsH *256 *255*256 + ptsL;

                if(myType == 0)
                    emit signal_sendH264(readDataBuff.data(),m_streamDateLen,pts,vResW,vResH);
                else if(myType == 1){
                    parseShiGanRgb1(readDataBuff,m_streamDateLen,vResW,vResH);
                }else if(myType == 2){
                    parseShiGanRgb2(readDataBuff,m_streamDateLen,vResW,vResH);
                }

                readDataBuff.remove(0,m_streamDateLen);
                resetAVFlag();

                needlen = 2;


                continue;

            }else{

                continue;
            }
        }
        if(mediaDataType == MediaType_H265)
        {
            needlen = 28;

            if(!isSaveVideoInfo)
            {
                if(readDataBuff.length() >= needlen)
                {
                    m_streamDateLen = saveVideoInfo(readDataBuff);
                    isSaveVideoInfo = true;
                    if(m_streamDateLen > videoFrameMaxLen || m_streamDateLen <0)
                    {
                        qDebug()<<"视频帧数据长度异常:"<<m_streamDateLen;
                        resetAVFlag();
                        continue;
                    }
                }else
                    continue;
            }

            needlen = m_streamDateLen;

            if(readDataBuff.length()>=needlen)
            {

                //emit signal_writeMediaVideoQueue(readDataBuff.data(),m_streamDateLen,infoV,MediaDataProcess::mMediaVeidoType);

                quint64 ptsH = 0x00000000ffffffff & infoV.highPts;
                quint64 ptsL = 0x00000000ffffffff & infoV.lowPts;
                quint64 pts = ptsH *256 *255*256 + ptsL;

               // if(myType == 0)
                    emit signal_sendH265(readDataBuff.data(),m_streamDateLen,pts,vResW,vResH);
//                else if(myType == 1){
//                    parseShiGanRgb1(readDataBuff,m_streamDateLen,vResW,vResH);
//                }else if(myType == 2){
//                    parseShiGanRgb2(readDataBuff,m_streamDateLen,vResW,vResH);
//                }

                readDataBuff.remove(0,m_streamDateLen);
                resetAVFlag();

                needlen = 2;


                continue;

            }else{

                continue;
            }
        }
        else if(mediaDataType == MediaType_RGB)
        {
            needlen = 28;

            if(!isSaveVideoInfo)
            {
                if(readDataBuff.length() >= needlen)
                {
                    m_streamDateLen = saveVideoInfo(readDataBuff);
                    isSaveVideoInfo = true;
                    if(m_streamDateLen > videoFrameMaxLen || m_streamDateLen <0)
                    {
                        qDebug()<<"视频帧数据长度异常:"<<m_streamDateLen;
                        resetAVFlag();
                        continue;
                    }
                }else
                    continue;
            }

            needlen = m_streamDateLen;

            if(readDataBuff.length()>=needlen)
            {

                //emit signal_writeMediaVideoQueue(readDataBuff.data(),m_streamDateLen,infoV,MediaDataProcess::mMediaVeidoType);
                quint64 ptsH = 0x00000000ffffffff & infoV.highPts;
                quint64 ptsL = 0x00000000ffffffff & infoV.lowPts;
                quint64 pts = ptsH *256 *255*256 + ptsL;

                parseShiGanRgb(readDataBuff,m_streamDateLen);

                readDataBuff.remove(0,m_streamDateLen);
                resetAVFlag();

                needlen = 2;


                continue;

            }else{

                continue;
            }
        }
        else if(MediaType_G711A == mediaDataType)
        {

            needlen = 28;
            if(!isSaveAudioInfo){

                if(readDataBuff.length() >= needlen)
                {
                    m_streamDateLen = saveAudioInfo(readDataBuff);
                    isSaveAudioInfo = true;

                    if(m_streamDateLen > audioFrameMaxLen)
                    {
                        //qDebug()<<"音频数据长度异常:"<<m_streamDateLen;


                        //qDebug()<<"音频数据长度异常   "<<readDataBuff.toHex();

                        resetAVFlag();
                        continue;
                    }


                }else
                    continue;
            }

            needlen = m_streamDateLen;

            if(m_streamDateLen > 0 && readDataBuff.length()>=needlen)
            {
                //audioSrc->write(readDataBuff.data(),m_streamDateLen);
                quint64 ptsH = 0x00000000ffffffff & infoA.highPts;
                quint64 ptsL = 0x00000000ffffffff & infoA.lowPts;
                quint64 pts = ptsH *256 *255*256 + ptsL;

                readDataBuff.remove(0,m_streamDateLen);

                m_streamDateLen = 2;
                resetAVFlag();
                continue;
            }else
                continue;
        }
        else if(MediaType_MSG  == mediaDataType)
        {
            //            qDebug()<<this->m_did <<"    777";
            //            qDebug()<<"find MediaType_MSG";
            needlen = 136;
            if(readDataBuff.length() >= needlen){
                int index = 0;
                QByteArray arrlen = readDataBuff.mid(index,4);
                int datalen = byteArr2Int(arrlen);
                index += 4;

                QByteArray arrstatuscode = readDataBuff.mid(index,4);
                int statuscode = byteArr2Int(arrstatuscode);
                index += 4;

                QByteArray arrDid = readDataBuff.mid(index,128);

                readDataBuff.remove(0,136);
                needlen = 2;
                resetAVFlag();
                continue;
            }else {
                continue;
            }
        }else{

            qDebug()<<"mediaDataType    "<<mediaDataType;
        }


        resetAVFlag();
        needlen = 2;

    }


}
#define BUFFLEN 320*240*3*4+512
void TcpWorker::parseShiGanRgb(QByteArray arr,int arrlen)
{


    //qDebug()<<"parseShiGanRgb   "<<arr.length()<<"  "<<arrlen;


    if(arrlen > BUFFLEN)
    {
        qDebug()<<"parseShiGanRgb 长度过长:"<<arrlen;
        return;
    }
    MediaContexHead stMtHd;

    if(pNetMsg == nullptr)
        pNetMsg = (unsigned char *)malloc(arrlen);

    memcpy(pNetMsg,arr.data(),arrlen);

    int framelen = (pNetMsg[4]<< 24)|(pNetMsg[3]<<16)|(pNetMsg[2] << 8)|(pNetMsg[1]);



    int bufpos=0;
    bufpos -= framelen + 3;  // head[1 byte]  crc[2 byte]

    int fpos = 0;
    fpos = MSG_HEAR_LEN;

    memcpy(&stMtHd, &pNetMsg[fpos], sizeof(MediaContexHead));
    fpos += sizeof(MediaContexHead);

    //        qDebug()<<"Image Flag:"<<stMtHd.ImageFlag<<"    Type:"<<stMtHd.ImageType<<endl
    //               <<"W"<<stMtHd.ImageWidth<<" H:"<<stMtHd.ImageHeigh<<"   Byte:"<<stMtHd.ImageByte
    //              <<"Tempe Flag:"<<stMtHd.TemperFlag<<"  TemperType:"<<stMtHd.TemperType
    //             <<"TemperWidth:"<<stMtHd.TemperWidth<<"    TemperHeigh:"<<stMtHd.TemperHeigh<<" TemperByte:"<<stMtHd.TemperByte;
    int w,h,x,y;
    if( stMtHd.ImageFlag )
    {
        w = stMtHd.ImageWidth;
        h = stMtHd.ImageHeigh;
        if(pNetMsgTmp == nullptr)
            pNetMsgTmp = new unsigned char[(w+2)*h*3];
        int tpos = 0;
        for(y=0; y < h; y++)
        {
            for(x=0; x < w; x++)
            {
                // R G B
                pNetMsgTmp[tpos++] = pNetMsg[fpos+y*w*3+x*3+2];
                pNetMsgTmp[tpos++] = pNetMsg[fpos+y*w*3+x*3+1];
                pNetMsgTmp[tpos++] = pNetMsg[fpos+y*w*3+x*3];
            }
            pNetMsgTmp[tpos++] = 0;
            pNetMsgTmp[tpos++] = 0;
        }

        //emit signal_sendH264((char*)pNetMsgTmp,w*h*3,10,w,h);
        QImage *pImg = nullptr;
        try {
            pImg =  new QImage(pNetMsgTmp, w, h, QImage::Format_RGB888);
            // 其它代码
        } catch ( const std::bad_alloc& e ) {
            qDebug()<<" 图片分配内存失败";
            pImg = nullptr;
        }

        emit signal_sendImg(pImg,w*h*3,10,w,h);
    }

}
void TcpWorker::parseShiGanRgb1(QByteArray arr,int arrlen,int resw,int resh)
{

    //qDebug()<<"parseShiGanRgb1   "<<arr.length()<<"  "<<arrlen;
    if(pNetMsgTmp == nullptr)
        pNetMsgTmp = new unsigned char[resw * resh* 4];

    memcpy(pNetMsgTmp,arr.data(),arrlen);

    QImage *pImg = nullptr;
    try {
        pImg =  new QImage(pNetMsgTmp, resw,resh, QImage::Format_RGB32);
        // 其它代码
    } catch ( const std::bad_alloc& e ) {
        qDebug()<<" 图片分配内存失败";
        pImg = nullptr;
    }
    // qDebug()<<" parseShiGanRgb1 "<<resw<<"  "<<resh<<"  "<<arrlen;
    emit signal_sendImg(pImg,arrlen,10,resw,resh);
}


void TcpWorker::parseShiGanRgb2(QByteArray arr,int arrlen,int resw,int resh)
{
    //qDebug()<<"parseShiGanRgb2   "<<arr.length()<<"  "<<arrlen;


    if(pNetMsgTmp == nullptr)
        pNetMsgTmp = new unsigned char[resw * resh* 4];

    memcpy(pNetMsgTmp,arr.data(),arrlen);

    QImage *pImg = nullptr;
    try {

        pImg =  new QImage(pNetMsgTmp, resw,resh, QImage::Format_ARGB32);
        // 其它代码
    } catch ( const std::bad_alloc& e ) {
        qDebug()<<" 图片分配内存失败";
        pImg = nullptr;
    }
    //qDebug()<<" parseShiGanRgb1 "<<resw<<"  "<<resh<<"  "<<arrlen <<"   "<< pImg->width() <<"  "<<pImg->height();
    emit signal_sendImg(pImg,arrlen,10,resw,resh);
}

void TcpWorker::parseH264(QByteArray arr,int arrlen)
{

}


int TcpWorker::byteArr2Int(QByteArray arr)
{

    int index = 0;
    int i1 = 0x000000ff & arr.at(index++);
    int i2 = 0x000000ff & arr.at(index++);
    int i3 = 0x000000ff & arr.at(index++);
    int i4 = 0x000000ff & arr.at(index++);

    return (i1 + i2*256 + i3*65536 + i4*16777216);
}


void TcpWorker::slot_tcpRecAuthentication(QString did,QString usrName,QString password)
{


    m_did = did;
    m_usrName = usrName;
    m_password = password;

}

void TcpWorker::slot_tcpSendAuthentication(QString did,QString usrName,QString password)
{
    // qDebug()<<"did usrName password:"<<did<<"   "<<usrName<<"   "<<password;

    // if(did != ""){
    //disconnect(tcpSocket,&QTcpSocket::readyRead,this,&TcpWorker::slot_readData);
    QByteArray arr;

    unsigned int datelen = 128 + 64 +64;
    int datelen0 = (0x000000ff & datelen);
    int datelen1 = (0x000000ff & (datelen>>8));
    int datelen2 = (0x000000ff & (datelen>>16));
    int datelen3 = (0x000000ff & (datelen>>24));

    arr.append(D_SYNCDATA_HEAD0);
    arr.append(D_SYNCDATA_HEAD1);
    arr.append(Msg_GetPlay);
    arr.append(MediaType_MSG);
    arr.append(datelen0);
    arr.append(datelen1);
    arr.append(datelen2);
    arr.append(datelen3);
    arr.append(did);
    arr.append(128-did.size(),0);
    arr.append(usrName);
    arr.append(64-usrName.size(),0);
    arr.append(password);
    arr.append(64-password.size(),0);

    int writeLen = tcpSocket->write(arr.data(),arr.length());
    qDebug()<<" writeLen    "<<writeLen<<"  "<<arr.toHex();
    //}
}

void TcpWorker::forceStopParse()
{
    QMutexLocker lockr(&mMutex);
    isForceFinish = true;
}

TcpWorker::~TcpWorker()
{
    qDebug()<<m_did +  " 析构   tcpWorker:"<<myType;


    if(tcpSocket != nullptr)
    {

        //   tcpSocket->disconnectFromHost();
        disconnect(tcpSocket,&QTcpSocket::readyRead,this,&TcpWorker::slot_readData);
        disconnect(tcpSocket,&QTcpSocket::disconnected,this,&TcpWorker::slot_tcpDisconnected);
        disconnect(tcpSocket,&QTcpSocket::connected,this,&TcpWorker::slot_tcpConnected);
        tcpSocket->disconnectFromHost();
        if (tcpSocket->state() == QAbstractSocket::UnconnectedState ||
                  tcpSocket->waitForDisconnected(1000))
        {
            qDebug()  << "disconnect server";
        }
        tcpSocket->abort();
        tcpSocket->close();
        tcpSocket == nullptr;
    }


    qDebug()<<m_did +  " 析构   tcpWorker 结束";
}



