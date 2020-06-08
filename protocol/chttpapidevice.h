#ifndef __HTTPAPIDEVICE_H__
#define __HTTPAPIDEVICE_H__


#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QEventLoop>
#include <QMutex>
using namespace std;

typedef struct _JsonMsg_T{
    char cmd[32];
    char method[32];
    char ssionID[32];
    char msgID[32];
}JsonMsg_T;
typedef struct _LoginInfo_T{

    char username[32];
    char password[32];

}LoginInfo_T;
#include "warntcpserver.h"
class CHttpApiDevice : public QObject
{

    Q_OBJECT

public:
    CHttpApiDevice(QString devid, QString ip, unsigned short port, QString usr, QString pas);//设备
    ~CHttpApiDevice();

    int CjsonMakeHttpHead(QJsonObject *msgObject, JsonMsg_T *msg);
    int SendRequestMsg(QJsonObject msg);
    int  HttpMsgCallBack(char * pData);

public:

private:
    QString g_ip;//http服务器地址
    unsigned short g_port;//http服务器断开

    LoginInfo_T g_loginInfo;//用户信息
    QTcpSocket *g_tcpsocket = nullptr;//http socket
    bool g_httpTcpConnectState;//连接状态

    char sessionId[32]; //回话id
    QTimer *timeoutCnt; //定时发送命令，
    QString timerEventCmd;//命令
    int dataEnableParam;  //数据
    bool loginFlag;      //login 标志位，登录成功才让操作

public slots:
    void slot_ReadMsg();
    void slot_Connected();
    void slot_disconnected();
    void slot_connectServer();
    void slot_msgEvent();
    void slot_httpParSet(QMap<QString,QVariant> map);
    void slot_httpGetInitPar();
    void slot_destoryConnect();
    bool createConnect();
    void slot_sendtimerout();
    void slot_heartimertout();
    bool send_httpParSet(QMap<QString,QVariant> map);

    void slot_WarnMsg(QMap<QString,QVariant> map);
signals:
    void signal_ReadMsg(QMap<QString,QVariant>);//QJsonObject
    void signal_httpErr();
    void signal_MsgReply(QString cmd);
    void signal_sendMag(QMap<QString,QVariant> map);
    void signal_httpConnected();
private:
    int connectCount = 0;

    void removeAlreadySend(QString cmd,QString msgid);
    QString createMsgId(QString cmd);
    void LoginDevice(QString msgid);
    void LogoutDevice(QString msgid);
    void HttpGetOsdParam();
    void HttpSetOsdParam(int osdTimeEnable,QString msgid);
    void HttpGetMotiondetectParam();
    void HttpSetMotiondetectParam(int);
    void HttpGetRecordParam();
    void HttpSetRecordParam(int);
    void HttpSetDate(QString msgid);
    void HttpGetDeviceType();
    void HttpSetIraInfo(QVariantMap value,QString msgid);
    void HttpSubscriptionWarn(QMap<QString,QVariant>,QString msgid);

    void HttpSetMeasureRect(QVariantMap value);
    void HttpSetalarmparam(QVariantMap value);
    void HttpSetVideoEncode(QVariantMap value);
    void HttpGetVideoEncode(QVariantMap value);
    void HttpSetimagparam(QVariantMap value);
    void HttpSetTempColor(QVariantMap value);


    void httpSendCommonCmd(QString cmd,QString );


    void createWarnService(QString ip,int port);
    void destroyWarnService();

    QString read_ip_address();
    const bool cmdSend = false;
    const bool cmdSendSucc = true;

    QMutex m_msgMutex;
    QTimer *SendTimer = nullptr;
    QTimer *reconnectTimer = nullptr;
    QList<QMap<QString,QVariant>> listMsg;
    int reconnectTimerCount = 0 ;
    int reconnectInter = 200;

    int sendertimerInter = 200;

    QString parseStr = "";
    QMap<QString,QVariant> warnPushMap;

    //QThread *m_warnPushThread= nullptr;
    QThread *warnTcpServerThread = nullptr;
    WarnTcpServer *warnTcpServer = nullptr;


    //视频参数
//    int chn = 0;
//    int framerate;
//    int bitrate ;
//    int quality ;
//    QString cvbrmode;
//    QString "h264",
//    "h264profile" : "baseline",
//    "resolution" : {
//        "width" : 1920,
//    "height" : 1080
//    }

    bool isForceFinish = false;

};

#endif
