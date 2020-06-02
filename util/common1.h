#ifndef COMMON_H
#define COMMON_H

//一般错误
#ifndef KEY_FALSE
#define KEY_FALSE				-1
#endif

#ifndef KEY_TRUE
#define KEY_TRUE				0
#endif







typedef struct _Rectangle_T
{
 int pointX;
 int pointY;
 int width;
 int high;
}Rectangle_T;


typedef struct _IradPointInfo_T
{
 Rectangle_T point;
 float tempvalue;
}IradPointInfo_T;

typedef enum _TempDisplay_T{
 ENUM_TEMP_Centigrade = 0x00,
 ENUM_TEMP_Fahrenheit,
}TempDisplay_T;

typedef struct _IradPoint_T
{
 int pointNum;
 TempDisplay_T  tempdisplay;
 IradPointInfo_T iradPointInfo[5];
}IradPoint_T;

typedef struct _VideoReslution_T
{
    int width;
    int height;
}VideoReslution_T;

typedef struct _QueueVideoInputInfo_T{
    unsigned char fps;   //帧率
    unsigned char rcmode;//编码器编码选择0 CBR  1 VBR
    unsigned char frametype;//帧类型
    unsigned char staty0; //编码类型
    VideoReslution_T reslution;
    int bitrate;
    //unsigned long long  pts;//ms 级别
    unsigned int highPts;
    unsigned int lowPts;//ms 级别
}QueueVideoInputInfo_T;

typedef struct _QueueAudioInputInfo_T{
    int samplerate;//采样率率
    int prenum;//采样点
    int bitwidth;//位宽
    int soundmode;   //声道总数
    //unsigned long long  pts; //时间戳
    unsigned int highPts;
    unsigned int lowPts;//ms 级别
}QueueAudioInputInfo_T;


typedef enum _Enum_MediaType
{
    MediaType_H264 = 0x01,
    MediaType_H265,
    MediaType_JPEG = 0x11,
    MediaType_MPEG4,
    MediaType_MJPEG,
    MediaType_G711A = 0x21,
    MediaType_G711U,
    MediaType_G726,
    MediaType_AAC,
    MediaType_PCM,
    MediaType_RGB,
    MediaType_MSG,
    MediaType_IRADPOINT  = 0x30,
}Enum_MediaType;



typedef enum _Enum_MsgCmd
{
    Msg_RecvGetDevicId = 0x01,
    Msg_GetPlay = 0x20,
    Msg_GetTearDown = 0x21,
    Msg_SendStreamPlayRes = 0x22,
}Enum_MsgCmd;

typedef struct _MeidaHead_T{
    unsigned char sysncHead0;
    unsigned char sysncHead1;
    unsigned char streamType;//码流类型
    unsigned char mediaType;//视音频区分
}MeidaHead_T;

typedef struct _MeidaVideo_T{
    MeidaHead_T head;
    QueueVideoInputInfo_T info;
    int datalen;  //片长度
}MeidaVideo_T;








#endif
