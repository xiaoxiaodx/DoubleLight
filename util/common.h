#ifndef COMMON_H
#define COMMON_H
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
    MediaType_MSG,
}Enum_MediaType;



typedef enum _Enum_MsgCmd
{
    Msg_RecvGetDevicId = 0x01,
    Msg_GetPlay = 0x20,
    Msg_GetTearDown = 0x21,
    Msg_SendStreamPlayRes = 0x22,
}Enum_MsgCmd;











#endif
