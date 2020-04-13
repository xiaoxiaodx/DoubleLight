#include "ircnet.h"
#include <QDebug>
#include "xvideotemp.h"
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

short temp_data[640 * 512] = { 0 };

void rawCallBackFunc(char * data, int width, int height, void * context){



    qDebug()<<" width :"<<width<<"  height:"<<height;

    int ifMax = 0;
    qreal fTempValue = 0;
    memcpy(&temp_data[0], data, 2 * width*height);

    if(temp_data[0]>7300){
        ifMax = 1;
    }else{
        ifMax = 0;
    }


    for(int j=0;j<height;j++){

        for(int i=0;i<width;i++){

            if(ifMax==1){
                fTempValue = 15.0;
                temp_data[j*width+i] = temp_data[j*width+i]-3300;
            }else{
                fTempValue = 30;
                temp_data[j*width+i] = temp_data[j*width+i]+700;
            }
        }
    }


    ImageInfo info;
    try {
        info.pImg =  new QImage((uchar*)temp_data, width, height, QImage::Format_RGB16);
        // 其它代码
    } catch ( const std::bad_alloc& e ) {
        DebugLog::getInstance()->writeLog("Yousee 图片分配内存失败");
        info.pImg = nullptr;
    }

    if(XVideoTemp::listBufferImginfo.size()<XVideoTemp::maxBuffLen){
        XVideoTemp::listBufferImginfo.append(info);
    }else {
        if(info.pImg != nullptr)
            delete info.pImg;
    }

}

void IRCNet::ircInit()
{

    qDebug()<<" ircInit ";
    if (!IRNET_ClientStartup(0, NULL, messagecallback, NULL))
    {
        qDebug()<<" IRNET_ClientStartup ";
        return;
    }
    //测温回调
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

    int errcode = 0;

    if ((IRNETHANDLE)(-1) == (m_handleClientStart2 =
                              IRNET_ClientStart("192.168.1.29",
                                                &clientInfo,
                                                3000,
                                                2//码流号 和通道号共同作用，唯一确定设备视频流的编码格式
                                                ))){

        errcode = 3;
        if(!IRNET_ClientCleanup())
            errcode = 4;

    }


    if (!IRNET_ClientRegRawCallback(m_handleClientStart2,
                                    &rawCallBackFunc, NULL))
    {
        //测温回调注册失败
        errcode = -4;
        qDebug()<<("Read temp failed!~");
        //        if ((IRNETHANDLE)(-1) != m_handleClientStart2&& !IRNET_ClientStop(m_handleClientStart))
        //        {
        //            //停止预览失败
        //            errcode = -5;

        //        }
        if (!IRNET_ClientCleanup()){
            errcode = -6;

        }
    }

    qDebug()<<"errcode : "<<errcode;
}

