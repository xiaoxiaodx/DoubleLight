#include "shigan.h"


#include "xvideotemp.h"
#include <QFile>
bool firstSave= true;
ShiGan::ShiGan()
{

}

bool ShiGan::startRec()
{

    struct hostent *he ;
    struct sockaddr_in server;
    WSADATA wd;
    if(WSAStartup(MAKEWORD(2,2),&wd)  != 0)                    //WSAStartup
    {
        qDebug()<<("WSAStartup error！");
        return false;
    }

    he = gethostbyname(SERVER_IP);

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM,0);

    memset(&server,0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr =*((struct in_addr*)he->h_addr);
    //char* localIP=inet_ntoa(*(struct in_addr*)*phost->h_addr_list);
    //server.sin_addr.S_un.S_addr = inet_addr(localIP);//设置服务器IP


    if( 0 != connect(sockfd, (struct sockaddr*)&server, sizeof(server)) )
    {
        qDebug()<<"connect failed"<<SERVER_IP;
        return false;
    }

    qDebug()<<("connect success\n");

    // send start stream
    if( !SendTransferStream(sockfd, true) )
    {
        printf("Start Transfer Stream failed\n");
        close(sockfd);
        return false;
    }
    return true;
}

void ShiGan::loopInit(){
    buflen = 320*240*3*4+512;
    pNetMsg = (unsigned char *)malloc(buflen);
    pftpufs = (float *)malloc(320*240*sizeof(float));
    bufpos = 0;
}
void ShiGan::loopUnInit()
{

    SendTransferStream(sockfd, false);
    close(sockfd);

    if( pNetMsg )
    {
        free(pNetMsg);
        pNetMsg = NULL;
    }

    if( pftpufs )
    {
        free(pftpufs);
        pftpufs = NULL;
    }
}

bool ShiGan::readOneFrame(ImageInfo &info){
    iret = NetMsgRecv(sockfd, pNetMsg, buflen, bufpos);
    if( iret < 0 )
    {
        qDebug()<<"NetMsgRecv fail:"<<iret;
        return false;
    }

    if(iret == 1)
    {



        framelen = (pNetMsg[4]<< 24)|(pNetMsg[3]<<16)|(pNetMsg[2] << 8)|(pNetMsg[1]);
        //qDebug()<<"Recv over .. bufpos:%d,framelen:%d\n"<<bufpos<<" "<<framelen;

        bufpos -= framelen + 3;  // head[1 byte]  crc[2 byte]

        int fpos;
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
            unsigned char *pNetMsgTmp = new unsigned char[(w+2)*h*3];
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

            try {
                info.pImg =  new QImage(pNetMsgTmp, w, h, QImage::Format_RGB888);
                // 其它代码
            } catch ( const std::bad_alloc& e ) {
                qDebug()<<" 图片分配内存失败";
                info.pImg = nullptr;
            }
           // delete pNetMsgTmp;
            fpos+=w*h*3;
        }

        if( stMtHd.TemperFlag )
        {
            int i,w,h;
            unsigned short *pucfts;
            FloatDiv* pfDivs;
            float* pftemp;
            w = stMtHd.TemperWidth;
            h = stMtHd.TemperHeigh;
            pucfts = (unsigned short*)&pNetMsg[fpos];
            pfDivs = (FloatDiv *)pftpufs;

            i = 0;
            for(y=0; y < h; y++)
            {
                for(x=0; x < w; x++)
                {
                    // R G B
                    pfDivs[i].f2 = *(pucfts++);
                    pfDivs[i].f1 = 0;

                    i++;
                }
            }

            x = w/2;
            y = h/2;
            i = y * w + w;
            info.areaMaxtemp = pftpufs[i];

            //qDebug()<<"pftpufs[0]:%f"<<pftpufs[i];
            static char *p = (char *)malloc(w*h);

            YouSeeParse::HotnessResetData(pftpufs,w,h,p,1,0);

            if(YouSeeParse::pFrame == NULL)
                YouSeeParse::pFrame=cvCreateImageHeader(cvSize(w,h),IPL_DEPTH_8U,1);
            cvSetData(YouSeeParse::pFrame,p,w);

            CvMemStorage*stor = NULL;
            CvSeq* cont = NULL;
            CvRect mdrects;

            stor = cvCreateMemStorage(0);
            cont=YouSeeParse::cvSegmentFGMask(YouSeeParse::pFrame,true,10.0,stor,cvPoint(0,0));

            float maxAvgT = -1;
            for(int i=0;cont!=0;cont=cont->h_next,i++)
            {
                mdrects=((CvContour*)cont)->rect;
                float avgT = YouSeeParse::getTempAavl(pftpufs, h, w, 1, 0, &mdrects) ;
                if(mdrects.width < TEMP_MIN_INTERVAL && mdrects.height < TEMP_MIN_INTERVAL)
                {
                    continue;
                }

                QMap<QString,QVariant> map;
                QRect rect(mdrects.x,mdrects.y,mdrects.width,mdrects.height);
                map.insert("rect",rect);
                map.insert("temp",avgT);
                //qDebug()<<  "avgT   "<<avgT;
                info.listRect.append(map);
                if(maxAvgT == -1){
                    maxAvgT = avgT;
                }else{
                    if(avgT > maxAvgT){
                        maxAvgT = avgT;
                    }
                }
            }
            //qDebug()<<" curImgInfo.listRect "<<curImgInfo.listRect.size();
            info.areaMaxtemp = maxAvgT;
            info.isDrawLine = true;

            //            info.areaMaxtemp = pftpufs[i];
           // qDebug()<<"pftpufs[0]:%f\n"<<pftpufs[i];
        }


        XVideoTemp::mutex.lock();
        if(XVideoTemp::listBufferImginfo.size() < XVideoTemp::maxBuffLen)
            XVideoTemp::listBufferImginfo.append(info);
        else{
            if(info.pImg != nullptr)
                delete info.pImg;
        }
        XVideoTemp::mutex.unlock();

    }

    return true;

}
void ShiGan::PrintBuf(unsigned char *buf, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}


unsigned short ShiGan::do_crc_16(unsigned char *message, unsigned int len)
{
    unsigned int i, j;
    unsigned short crc_reg = 0;
    unsigned short current;
    for (i = 0; i < len; i++)
    {
        current = message[i] << 8;
        for (j = 0; j < 8; j++)
        {
            if ((short) (crc_reg ^ current) < 0)
                crc_reg = (crc_reg << 1) ^ 0x1021;
            else
                crc_reg <<= 1;
            current <<= 1;
        }
    }
    return crc_reg;		//分字节，为大端存储
}


bool ShiGan::PackHeader(unsigned char * buf, unsigned int len, unsigned char cmd1, unsigned char cmd2)
{
    buf[0] = 0xa5;

    buf[1] = (len & 0xff);
    buf[2] = (len & 0xff00)>>8;
    buf[3] = (len & 0xff0000)>>16;
    buf[4] = (len & 0xff000000)>>24;

    buf[5] = cmd1;
    buf[6] = cmd2;

    return true;
}

int ShiGan::KeepAliveReq()
{

    int len;
    unsigned short crc;
    unsigned char msgbuf[128]={0};

    len = MSG_HEAR_LEN - 1 + 1;
    PackHeader(msgbuf, len, CMD_HeartBeat, CMD_Cmd_Ack);
    msgbuf[MSG_HEAR_LEN] = 0x0;

    // CRC
    crc = do_crc_16(msgbuf+1, len);
    msgbuf[len+1] = (crc & 0xff00)>>8;
    msgbuf[len+2] =  crc & 0xff;

    // Send to networ

    len = send(sockfd, (char*)msgbuf, len + 1 + 2,0);   // [head |1][data | len][crc | 2]
    QByteArray arr;
    qDebug()<<"len  "<<len+1+2;
    arr.append((char*)msgbuf, len );
    qDebug()<<" KeepAliveReq * 2"<<arr.toHex();
    if(-1 == len)
    {
        return 1;
    }
    return 0;
}



bool ShiGan::SendTransferStream(SOCKET sockfd, bool bIsStart)
{
    int len;
    unsigned short crc;
    unsigned char msgbuf[128]={0};

    len = MSG_HEAR_LEN -1 + 1;
    PackHeader(msgbuf, len, CMD_transfers, CMD_Cmd_Ack);
    if( bIsStart ) {
        msgbuf[MSG_HEAR_LEN] = 0x1;
    } else {
        msgbuf[MSG_HEAR_LEN] = 0x0;
    }

    // CRC
    crc = do_crc_16(msgbuf+1, len);
    msgbuf[len+1] = (crc & 0xff00)>>8;
    msgbuf[len+2] =  crc & 0xff;

    // Send to networ
    len = send(sockfd,(char*)msgbuf,len+1+2, 0);   // [head |1][data | len][crc | 2]
    qDebug()<<" len "<<len;
    if( -1 == len )
    {
        return false;
    }
    return true;
}

bool ShiGan::CheckHead(unsigned char *buf, int len)
{
    int framelen;
    unsigned short crc;

    if (buf[0] != 0xa5)
    {
        printf("err head buf[0]:%02x\n", buf[0]);
        return false;
    }
    if (len < MSG_HEAR_LEN + 2)
    {
        //printf("%s\t len:%d is err\n", __FUNCTION__, len);
        return false;
    }
    //framelen = (buf[1] << 24) | (buf[2] << 16) | (buf[3] << 8) | (buf[4]);
    framelen = (buf[4] << 24) | (buf[3] << 16) | (buf[2] << 8) | (buf[1]);
    //printf("buf %02x %02x %02x %02x\n",buf[1],buf[2],buf[3],buf[4]);
    //printf("framelen:%d,len:%d\n",framelen,len);
    //if (framelen < (len - 3))
    if ((len - 3) < framelen)
    {
        //printf("frame len err framelen:%d, len:%d\n", framelen, len);
        return false;
    }
    // crc
    crc = buf[len - 2] << 8 | buf[len - 1];
    //printf("crc:%02x\n",crc);

    if (do_crc_16(buf + 1, len - 3) != crc)
    {
        printf("crc err:\n");
        return false;
    }
    return true;
}

int ShiGan::NetMsgRecv(SOCKET sockfd, unsigned char *pbufs, int buflen, int &bufpos)
{

    int rlen;

    rlen = (buflen-bufpos);
    if( rlen  < MSG_HEAR_LEN )
    {
        qDebug()<<"bufs is full"<<rlen;
        return -1;
    }

    rlen = recv(sockfd, (char*)&pbufs[bufpos], (buflen-bufpos), 0);
    if( -1 == rlen )
        return -1;

    bufpos += rlen;

    //printf("rlen:%d,bufpos:%d\n",rlen,bufpos);

    if( bufpos > MSG_HEAR_LEN + 2)
    {
        if( CheckHead(pbufs, bufpos) )
        {
            return 1;
        }
        else
        {
            //PrintBuf(pbufs, 30);
        }
    }

    return 0;
}





