#ifndef SHIGAN_H
#define SHIGAN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <iostream>
using namespace std;

//#define SERVER_IP  "10.67.1.125"
#define SERVER_IP  "192.168.173.100"
#define SERVER_PORT 8080

#define MSG_HEAR_LEN		23
#define MAX_MSG_BUF_LEN		(1024*1024)

#include <QDebug>
typedef enum
{
    CMD_HeartBeat			= 0x31,
    CMD_transfers 			= 0x32,
    CMD_SetCalPoint 		= 0x33,
    CMD_PlaneCalibration	= 0x34,
    CMD_DrawCalibRect		= 0x35,
    CMD_SnapVideo			= 0x36,
    CMD_AlarmSnapVideo		= 0x37,
    CMD_SetTemperAlarm		= 0x38,
    CMD_SetTemperOffset		= 0x39,
    CMD_Reboot				= 0x3A,
    CMD_UpGrade 			= 0x3B,
    CMD_SetNetInfo			= 0x3C,
    CMD_GetNetInfo			= 0x3D,
    CMD_GetVersion			= 0x80,
    CMD_Cmd_Ack	  			= 0x1f,
}CMD;

typedef enum
{
    ACK_OK = 0x00,
    ACK_Err_Para  = 0x33,
    ACK_Err_CRC   = 0x44,
    ACK_Err_NoExit= 0xfd,
    ACK_UpGrade_File	= 0x55,
    ACK_UpGrade_Data	= 0x50,
    ACK_NR,
}MSG_ACK;

// 媒体头
typedef struct tagMediaContexHead
{
    unsigned char ImageFlag		:1;		// 图像存在标识
    unsigned char ImageByte		:7;		// 每个图像单元占字节
    unsigned char ImageType		:8;		// 图像类型
    unsigned short ImageHeigh	:16;	// 图像长度
    unsigned short ImageWidth	:16;	// 图像宽度

    unsigned char TemperFlag	:1;		// 温度存在标识
    unsigned char TemperByte	:7;		// 每个温度单元占字节
    unsigned char TemperType	:8;		// 温度类型
    unsigned short TemperHeigh	:16;	// 温度长度
    unsigned short TemperWidth	:16;	// 温度宽度
}MediaContexHead;

typedef struct tagFloatDiv
{
    unsigned short f1:16;
    unsigned short f2:16;
}FloatDiv;
#include <WinSock2.h>
#include <QMutex>
#include "youseeparse.h"


class ShiGan
{

public:
    explicit ShiGan();

    bool startRec();
    void loopInit();
    void loopUnInit();
    bool readOneFrame(ImageInfo &info);
    int KeepAliveReq();
private:
    unsigned short do_crc_16(unsigned char *message, unsigned int len);
    bool PackHeader(unsigned char * buf, unsigned int len, unsigned char cmd1, unsigned char cmd2);
    bool SendTransferStream(SOCKET sockfd, bool bIsStart);
    bool CheckHead(unsigned char *buf, int len);
    int NetMsgRecv(SOCKET sockfd, unsigned char *pbufs, int buflen, int &bufpos);
    void PrintBuf(unsigned char *buf, int len);


    SOCKET sockfd;
    int iret;
    int irlen;
    int buflen;
    int bufpos;
    int framelen;
    float *pftpufs;
    unsigned char *pNetMsg;
    MediaContexHead stMtHd;
};

#endif // SHIGAN_H
