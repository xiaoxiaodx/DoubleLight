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

#define MAX_AUDIO_FRAME_SIZE 192000

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

    void signal_sendImg(QImage *pimg);
    void signal_sendH264(char* vH264Arr,int arrLen,long long pts,int resw,int resh);
<<<<<<< HEAD

=======
    void signal_connected();
>>>>>>> 8a8a93cdb25da892e1836b7d644d539a48fda3c2
public slots:

    void slot_readData();
    void slot_tcpDisconnected();
    void slot_tcpConnected();


    void slot_timerConnectSer();
    void slot_disConnectSer();
    void slot_tcpSendAuthentication(QString did,QString name,QString pwd);
    void slot_tcpRecAuthentication(QString did,QString name,QString pwd);


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

    int myType=0;


};

#endif // TCPWORKER_H
