#include "ircnet.h"
#include <QDebug>
#include "xvideotemp.h"
#include "youseeparse.h"
IRCNet::IRCNet(QObject *parent) : QObject(parent)
{

}

void WINAPI  messagecallback(IRNETHANDLE hHandle, WPARAM wParam, LPARAM  lParam, void *context)
{
    switch (wParam)
    {
    case LAUMSG_LINKMSG:
        if (!lParam)//connection successful
        {
            qDebug()<<"IRC connection successful";
        }
        else//connect failed
        {
             qDebug()<<"IRC connection failed";
        }
        break;
    default:
        break;
    }

}

short temp_data[384 * 288] = { 0 };
ImageInfo curImgInfo;
void RawCallBackFunc(char * data, int width, int height, void * context){

    //qDebug()<<" width :"<<width<<"  height:"<<height;

    int ifMax = 0;
    qreal fTempValue = 0;
    memcpy(&temp_data[0], data, 2 * width*height);

   // qDebug()<<" width :"<<width<<"  height:"<<height<<" temp_data[0]:"<<temp_data[0]<<" "<<temp_data[1];
    if(temp_data[0]>7300){
        ifMax = 1;
    }else{
        ifMax = 0;
    }

    for(int j=0;j<height;j++){

        for(int i=0;i<width;i++){

            if(ifMax==1){
                fTempValue = 15.0;
                temp_data[j*width+i] = (temp_data[j*width+i]+25000)/100 - 273.15;
            }else{
                fTempValue = 30;
                //temp_data[j*width+i] = (temp_data[j*width+i]+7000)/fTempValue - 273.2;

                temp_data[j*width+i] = (temp_data[j*width+i]+25000)/100 - 273.15;
            }
        }
    }

    static char *p = (char *)malloc(height*width);
    YouSeeParse::HotnessResetData(temp_data,height,width,p,1,0);

    if(YouSeeParse::pFrame == NULL)
        YouSeeParse::pFrame=cvCreateImageHeader(cvSize(width,height),IPL_DEPTH_8U,1);
    cvSetData(YouSeeParse::pFrame,p,width);

    CvMemStorage*stor = NULL;
    CvSeq* cont = NULL;
    CvRect mdrects;

    stor = cvCreateMemStorage(0);
    cont=YouSeeParse::cvSegmentFGMask(YouSeeParse::pFrame,true,10.0,stor,cvPoint(0,0));

    float maxAvgT = -1;
    curImgInfo.listRect.clear();
    for(int i=0;cont!=0;cont=cont->h_next,i++)
    {
        mdrects=((CvContour*)cont)->rect;
        float avgT = YouSeeParse::getTempAavl(temp_data, height, width, 1, 0, &mdrects) ;
        if(mdrects.width < TEMP_MIN_INTERVAL && mdrects.height < TEMP_MIN_INTERVAL)
        {
            continue;
        }

        QMap<QString,QVariant> map;
        QRect rect(mdrects.x,mdrects.y,mdrects.width,mdrects.height);
        map.insert("rect",rect);
        map.insert("temp",avgT);
        //qDebug()<<  "avgT   "<<avgT;
        curImgInfo.listRect.append(map);
        if(maxAvgT == -1){
            maxAvgT = avgT;
        }else{
            if(avgT > maxAvgT){
                maxAvgT = avgT;
            }
        }
    }
    //qDebug()<<" curImgInfo.listRect "<<curImgInfo.listRect.size();
    curImgInfo.areaMaxtemp = maxAvgT;
    curImgInfo.isDrawLine = true;
}


#include "ffmpegconvert.h"

FfmpegConvert *yuv2rgb=nullptr;
void ShowCallBackFunc(BYTE * m_y, BYTE * m_u, BYTE * m_v, int stridey, int strideuv, int width, int height, void * context)
{
    //qDebug()<<"stridey:"<<stridey<<"    strideuv"<<strideuv<<"  "<<width<<" height"<<height;
    BYTE *pBuf = new BYTE[stridey*height + strideuv*height];
    memcpy(pBuf, m_y, sizeof(BYTE)*stridey*height);
    memcpy(pBuf + sizeof(BYTE)*stridey*height, m_u, sizeof(BYTE)*strideuv*height * 1 / 2);
    memcpy(pBuf + sizeof(BYTE)*stridey*height + sizeof(BYTE)*strideuv*height * 1 / 2, m_v, sizeof(BYTE)*strideuv*height * 1 / 2);
    if(yuv2rgb == nullptr){
        yuv2rgb = new FfmpegConvert;
        yuv2rgb->initYuv420ToRgb32(width,height);
    }

    ImageInfo info;
    info.pImg = yuv2rgb->yuv420ToRgb32((char*)pBuf,width,height);
    info.isDrawLine = curImgInfo.isDrawLine;
    for(int i=0;i<curImgInfo.listRect.size();i++)
        info.listRect.append(curImgInfo.listRect.at(i));
    info.areaMaxtemp = curImgInfo.areaMaxtemp;

    XVideoTemp::mutex.lock();
    if(XVideoTemp::listBufferImginfo.size()<XVideoTemp::maxBuffLen){
        XVideoTemp::listBufferImginfo.append(info);
    }else {
        if(info.pImg != nullptr)
            delete info.pImg;
    }
    XVideoTemp::mutex.unlock();
}

BOOL GetByte(BYTE * ou, char h, char l)
{
    if (!ou)
    {
        qDebug()<<"ou is NULL";
        return false;
    }
    static std::map<char, BYTE> t;
    //qDebug()<<"h:"<<h<<" l:"<<l<<" size:"<<t.size();
    if (!t.size())
    {
        t['0'] = 0;
        t['1'] = 1;
        t['2'] = 2;
        t['3'] = 3;
        t['4'] = 4;
        t['5'] = 5;
        t['6'] = 6;
        t['7'] = 7;
        t['8'] = 8;
        t['9'] = 9;
        t['A'] = 10;
        t['B'] = 11;
        t['C'] = 12;
        t['D'] = 13;
        t['E'] = 14;
        t['F'] = 15;
        t['a'] = 10;
        t['b'] = 11;
        t['c'] = 12;
        t['d'] = 13;
        t['e'] = 14;
        t['f'] = 15;
    }
    std::map<char, BYTE>::iterator i, j;

    if ((i = t.find(h)) == t.end() || (j = t.find(l)) == t.end())
    {
        qDebug()<<"i = t.find(h)) == t.end()";
        return false;
    }

    *ou = (i->second << 4) | (j->second);
    return true;
}

void IRCNet::FuncSetColor(int type)
{
    char value[64];

    if(type == 2) //彩色
    {
          sprintf(value,"AA05002D0104E1EBAA");
    }
    else
    {
         sprintf(value,"AA05002D0100DDEBAA");
    }

    int length = strlen(value);
    //qDebug()<<"type:"<<type<<" length:"<<length<<" value:"<<value;
    if (length == 0 || length % 2)
    {
        return;
    }
    char *tm = new char[length / 2];
    if (!tm)
    {
        return;
    }
    bool fillrst = true;
    for (int i = 0, j = length / 2; i < j; ++i)
    {
        //str.AppendFormat(_T("%02X "),st[2*i],st[2*i+1]);
        if (!GetByte((BYTE*)(tm + i), value[2 * i], value[2 * i + 1]))
        {
            fillrst = false;
            break;
        }
    }
    //qDebug()<<"tm:"<<tm<<" fillrst:"<<fillrst;
    if (fillrst && IRNET_ClientSerialSendNew(m_serialconnect, tm, length / 2))
    {
        qDebug()<<"IRNET_ClientSerialSendNew OK";
        delete[] tm;
        return;
    }
    else
    {
        qDebug()<<"IRNET_ClientSerialSendNew failed";
    }
    delete[] tm;
    return;
}

void FuncSerialDataCallBack(IRNETHANDLE hSerial, char * pRecvDataBuff, int BuffSize, void * context)
{
    return;
}

void IRCNet::ircInit()
{

    qDebug()<<" ircInit ";
    if (!IRNET_ClientStartup(0, NULL, messagecallback, NULL))
    {
        qDebug()<<" IRNET_ClientStartup failed!!";
        return;
    }
    //测温回调
    clientInfo.m_hChMsgWnd = NULL;
    clientInfo.m_nChmsgid = 0;
    clientInfo.m_sername = "video server";
    clientInfo.m_username = "888888";
    clientInfo.m_password = "888888";
    clientInfo.m_playstart = TRUE;//解码
    clientInfo.m_tranType = 3;//3—tcp方式通信
    clientInfo.m_useoverlay = FALSE;
    clientInfo.nColorKey = RGB(255, 0, 0);
    clientInfo.m_ch = 0;//0通道
    clientInfo.m_buffnum = 20;//缓存帧数目
    clientInfo.m_hVideohWnd = NULL;// m_ctrlPIC.GetSafeHwnd();//视频渲染窗口
    //clientInfo.m_hVideohWnd = image1.GetSafeHwnd();//视频渲染窗口
    clientInfo.context = NULL;
    clientInfo.m_messagecallback = NULL;
    int errcode = 0;
     qDebug()<<"start  IRNET_ClientStart ";
    if ((IRNETHANDLE)(-1) == (m_handleClientStart =
                              IRNET_ClientStart("192.168.1.29",
                                                &clientInfo,
                                                3000,
                                                0//码流号 和通道号共同作用，唯一确定设备视频流的编码格式
                                                ))){
        errcode = 3;
        if(!IRNET_ClientCleanup())
            errcode = 4;
    }

    //注册YUV数据读取
    if (!IRNET_ClientShowcallback(m_handleClientStart, &ShowCallBackFunc, NULL))
    {
        //测温回调注册失败
        Sleep(500);
        if (!IRNET_ClientShowcallback(m_handleClientStart, &ShowCallBackFunc, NULL))
        {
            errcode = -4;
            qDebug()<<"IRNET_ClientShowcallback failed!~";
            if (!IRNET_ClientStop(m_handleClientStart))
            {
                //停止预览失败
                errcode = -5;
                return;
            }
            if (!IRNET_ClientCleanup())
            {
                errcode = -6;
                return;
            }
            return;
        }
    }

    clientInfo.m_nChmsgid = 0;
    clientInfo.m_sername = "video server";
    clientInfo.m_username = "888888";
    clientInfo.m_password = "888888";
    clientInfo.m_playstart = TRUE;//解码
    clientInfo.m_tranType = 3;//3—tcp方式通信
    clientInfo.m_useoverlay = FALSE;
    clientInfo.nColorKey = RGB(255, 0, 0);
    clientInfo.m_ch = 0;//0通道
    clientInfo.m_buffnum = 20;//缓存帧数目
    clientInfo.m_hVideohWnd = nullptr;
    //m_ctrlPIC.GetSafeHwnd();//视频渲染窗口
    clientInfo.context = NULL;
    clientInfo.m_messagecallback = NULL;

    if ((IRNETHANDLE)(-1) == (m_handleClientStart2 =
                        IRNET_ClientStart("192.168.1.29",
                            &clientInfo,
                            3000,
                            2//码流号 和通道号共同作用，唯一确定设备视频流的编码格式
                            )))
    {
        //出错
        errcode = -3;
        qDebug()<<"Open temp channel failed!~";
        if ((IRNETHANDLE)(-1) != m_handleClientStart2 && !IRNET_ClientStop(m_handleClientStart))
        {
            //停止预览失败
            errcode = -5;
            return;
        }
        if (!IRNET_ClientCleanup())
        {
            errcode = -6;
            return;
        }
        return;
    }

    if (!IRNET_ClientRegRawCallback(m_handleClientStart2, &RawCallBackFunc, NULL))
    {
        //测温回调注册失败
        errcode = -4;
        qDebug()<<"Read temp failed!~";
        if ((IRNETHANDLE)(-1) != m_handleClientStart2 && !IRNET_ClientStop(m_handleClientStart))
        {
            //停止预览失败
            errcode = -5;
            return;
        }

        if ((IRNETHANDLE)(-1) != m_handleClientStart2 && !IRNET_ClientStop(m_handleClientStart2))
        {
            //停止预览失败
            errcode = -5;
            return;
        }

        if (!IRNET_ClientCleanup())
        {
            errcode = -6;
            return;
        }
    }

    if (m_serialconnect != IRNETHANDLE(-1))
    {
        if (IRNET_ClientSerialStop(m_serialconnect))
        {
            m_serialconnect = IRNETHANDLE(-1);
        }

    }

    VSSERIAL_INFO serialInfo;
    serialInfo.baudrate = 115200;
    serialInfo.checkbit = 0;
    serialInfo.databit = 8;
    serialInfo.flowcontrol = 0;

    std::string seraddr = "";
    std::string pwd = "";
    std::string username = "";
    int type = 0;
    WORD port = 0;
    pwd = "888888";
    username = "888888";
    type = 1;
    port = 3000;
    m_serialconnect = IRNET_ClientSerialStart(NULL, "192.168.1.29",
        "888888",
        "888888",
        type,
        &serialInfo,
        &FuncSerialDataCallBack,
        this,
        port);
    if (m_serialconnect == IRNETHANDLE(-1))
    {
        qDebug()<<"Open Com trans Fail~";
        return;
    }

    qDebug()<<"errcode : "<<errcode;
}

