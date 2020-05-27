#ifndef TCPWORKER_H
#define TCPWORKER_H


#define D_SYNCDATA_HEAD0 0x77
#define D_SYNCDATA_HEAD1 0x17
#define MAX_STREAM_MTU_SIZE 1450

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QHostAddress>
#include <QTimer>
#include <QImage>
#include "common1.h"
#include <QFile>
#include <QMutex>
#include <QAbstractSocket>
#define MAX_AUDIO_FRAME_SIZE 192000
#include "shigan.h"
//typedef enum
//{
//    CMD_HeartBeat			= 0x31,
//    CMD_transfers 			= 0x32,
//    CMD_SetCalPoint 		= 0x33,
//    CMD_PlaneCalibration	= 0x34,
//    CMD_DrawCalibRect		= 0x35,
//    CMD_SnapVideo			= 0x36,
//    CMD_AlarmSnapVideo		= 0x37,
//    CMD_SetTemperAlarm		= 0x38,
//    CMD_SetTemperOffset		= 0x39,
//    CMD_Reboot				= 0x3A,
//    CMD_UpGrade 			= 0x3B,
//    CMD_SetNetInfo			= 0x3C,
//    CMD_GetNetInfo			= 0x3D,
//    CMD_GetVersion			= 0x80,
//    CMD_Cmd_Ack	  			= 0x1f,
//}CMD;

//typedef enum
//{
//    ACK_OK = 0x00,
//    ACK_Err_Para  = 0x33,
//    ACK_Err_CRC   = 0x44,
//    ACK_Err_NoExit= 0xfd,
//    ACK_UpGrade_File	= 0x55,
//    ACK_UpGrade_Data	= 0x50,
//    ACK_NR,
//}MSG_ACK;



//// 媒体头
//typedef struct tagMediaContexHead
//{
//    unsigned char ImageFlag		:1;		// 图像存在标识
//    unsigned char ImageByte		:7;		// 每个图像单元占字节
//    unsigned char ImageType		:8;		// 图像类型
//    unsigned short ImageHeigh	:16;	// 图像长度
//    unsigned short ImageWidth	:16;	// 图像宽度

//    unsigned char TemperFlag	:1;		// 温度存在标识
//    unsigned char TemperByte	:7;		// 每个温度单元占字节
//    unsigned char TemperType	:8;		// 温度类型
//    unsigned short TemperHeigh	:16;	// 温度长度
//    unsigned short TemperWidth	:16;	// 温度宽度
//}MediaContexHead;

//typedef struct tagFloatDiv
//{
//    unsigned short f1:16;
//    unsigned short f2:16;
//}FloatDiv;

#define MSG_HEAR_LEN		23
#define MAX_MSG_BUF_LEN		(1024*1024)


#include "ffmpegconvert.h"

class TcpWorker : public QObject
{
    Q_OBJECT
public:
    explicit TcpWorker(int type,QObject *parent = nullptr);
    ~TcpWorker();


//    void pauseRecV();
//    void finish();
    void forceStopParse();
signals:

    //void signal_sendImg(QImage *pimg);
    void signal_sendH264(char* vH264Arr,int arrLen,long long pts,int resw,int resh);
    void signal_connected();
    void signal_sendImg(QImage *img,int len,quint64 time,int resw,int resh);

public slots:

    void slot_readData();
    void slot_tcpDisconnected();
    void slot_tcpConnected();


    void slot_timerConnectSer();
    void slot_disConnectSer();
    void slot_tcpSendAuthentication(QString did,QString name,QString pwd);
    void slot_tcpRecAuthentication(QString did,QString name,QString pwd);

    void slot_soceckErr(QAbstractSocket::SocketError socketError);
    void creatNewTcpConnect(QString ip,int port);

private:


    /*********************************/
    int saveVideoInfo(QByteArray &arr);
    int saveAudioInfo(QByteArray &arr);
    QueueVideoInputInfo_T infoV;
    QueueAudioInputInfo_T infoA;

    int vResW;
    int vResH;

    int byteArr2Int(QByteArray arr);
    void parseRecevieData();

    void initVariable();
    void resetAVFlag();

    void parseH264(QByteArray arr,int arrlen);
    void parseShiGanRgb(QByteArray arr,int arrlen);
    void parseShiGanRgb1(QByteArray arr,int arrlen,int resw,int resh);
    void parseShiGanRgb2(QByteArray arr,int arrlen,int resw,int resh);
    void parseShiGanRgb3(QByteArray arr,int arrlen,int resw,int resh);
    QTcpSocket *tcpSocket;

    QByteArray readDataBuff;
    bool isFindHead;
    bool isFindMediaType;
    bool isSaveVideoInfo;
    bool isSaveAudioInfo;
    bool isSendAudioData;
    int mediaDataType;
    int m_streamDateLen;

    int minLen;

    QString m_did;
    QString m_usrName;
    QString m_password;
    QString ip;
    quint16 port;

    QTimer *timerConnectSer;
    bool isReconnecting;



    QFile *audioSrc;

    int videoFrameMaxLen;
    int audioFrameMaxLen;

    QMutex mMutex;
    bool isForceFinish = false;


    bool isCheckedDataLong;
    bool isConnected;
    bool isHavaData;

    unsigned char * pNetMsg = nullptr;
    unsigned char *pNetMsgTmp = nullptr;

    int myType=0;


    FfmpegConvert *ffmpegConvert = nullptr;
};

#endif // TCPWORKER_H
