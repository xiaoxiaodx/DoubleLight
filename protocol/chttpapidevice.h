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




//typedef struct{
//    int  index;  /*序号*/
//    char* pszCityName;
//    unsigned char u32STDSign;                         /*时区符号：0为"+",1为"-"*/
//    unsigned char hour;
// unsigned char min;
// unsigned char sec;
//} TZONE_T;

//static TZONE_T g_astruTZS[]={
//{ 1 ,  "Etc/GMT-12",            0,   12,  0,    0    },
//{ 2 ,  "Pacific/Apia",          0,   11,  0,    0    },
//{ 3 ,  "Pacific/Honolulu",      0,   10,  0,    0    },
//{ 4 ,  "America/Anchorage",     0,   9,   0,    0    },
//{ 5 ,  "America/Los_Angeles",   0,   8,   0,    0    },
//{ 6 ,  "America/Denver",        0,   7,   0,    0    },
//{ 7 ,  "America/Phoenix",       0,   7,   0,    0    },
//{ 8 ,  "America/Tegucigalpa", 0,  7,   0, 0    },
//{ 9 ,  "America/Winnipeg",      0,   6,   0,    0    },
//{ 10,  "America/Chicago",       0,   6,   0,    0    },
//{ 11,  "America/Costa_Rica",    0,   6,   0,    0    },
//{ 12,  "America/Mexico_City", 0,  6,   0, 0    },
//{ 13,  "America/Bogota",  0,  5,   0, 0    },
//{ 14,  "America/New_York",      0,   5,   0,    0    },
//{ 15,  "America/Indianapolis", 0,  5,   0, 0    },
//{ 16,  "America/Montreal",  0,  4,   0, 0    },
//{ 17,  "America/Caracas",       0,   4,   0,    0    },
//{ 18,  "America/Santiago",  0,  4,   0, 0    },
//{ 19,  "America/St_Johns",      0,   3,  30,    0    },
//{ 20,  "America/Sao_Paulo",  0,  3,   0, 0    },
//{ 21,  "America/Buenos_Aires",  0,   3,   0,    0    },
//{ 22,  "America/Thule",   0,  3,   0, 0    },
//{ 23,  "Atlantic/South_Georgia", 0,  2,   0,    0  },
//{ 24,  "Atlantic/Cape_Verde",   0,   1,   0,    0    },
//{ 25,  "Atlantic/Azores",       0,   1,   0,    0    },
//{ 26,  "Europe/Dublin",         0,   0,   0,    0    },
//{ 27,  "Africa/Casablanca",     0,   0,   0,    0    },
//{ 28,  "Europe/Amsterdam",      1,   1,   0,    0    },
//{ 29,  "Europe/Belgrade",       1,   1,   0,    0    },
//{ 30,  "Europe/Brussels",       1,   1,   0,    0    },
//{ 31,  "Europe/Warsaw",         1,   1,   0,    0    },
//{ 32,  "Africa/Lagos",          1,   1,   0,    0    },
//{ 33,  "Europe/Bucharest",      1,   2,   0,    0    },
//{ 34,  "Africa/Harare",   1,  2,   0, 0    },
//{ 35,  "Europe/Helsinki",  1,  2,   0, 0    },
//{ 36,  "Africa/Cairo",          1,   2,   0,    0    },
//{ 37,  "Europe/Athens",   1,  2,   0, 0    },
//{ 38,  "Asia/Jerusalem",        1,   2,   0,    0    },
//{ 39,  "Asia/Baghdad",          1,   3,   0,    0    },
//{ 40,  "Asia/Kuwait",           1,   3,   0,    0    },
//{ 41,  "Europe/Moscow",         1,   3,   0,    0    },
//{ 42,  "Africa/Nairobi",        1,   3,   0,    0    },
//{ 43,  "Asia/Tehran",           1,   3,   30,   0    },
//{ 44,  "Asia/Dubai",            1,   4,   0,    0    },
//{ 45,  "Asia/Baku",             1,   4,   0,    0    },
//{ 46,  "Asia/Kabul",            1,   4,   30,   0    },
//{ 47,  "Asia/Yekaterinburg",    1,   5,   0,    0    },
//{ 48,  "Asia/Karachi",          1,   5,   0,    0    },
//{ 49,  "Asia/Calcutta",         1,   5,   30,   0    },
//{ 50,  "Asia/Katmandu",         1,   5,   45,   0    },
//{ 51,  "Asia/Almaty",           1,   6,   0,    0    },
//{ 52,  "Asia/Dhaka",            1,   6,   0,    0    },
//{ 53,  "Asia/Colombo",          1,   6,   0,    0    },
//{ 54,  "Asia/Rangoon",          1,   6,   30,  0    },
//{ 55,  "Asia/Krasnoyarsk",  1,  7,   0, 0    },
//{ 56,  "Asia/Bangkok",          1,   7,   0,    0    },
//{ 57,  "Asia/Hong_Kong",        1,   8,   0,    0    },
//{ 58,  "Asia/Kuala_Lumpur",     1,   8,   0,    0    },
//{ 59,  "Australia/Perth",       1,   8,   0,    0    },
//{ 60,  "Asia/Taipei",           1,   8,   0,    0    },
//{ 61,  "Asia/Irkutsk",   1,  8,   0, 0    },
//{ 62,  "Asia/Tokyo",            1,   9,   0,    0    },
//{ 63,  "Asia/Seoul",            1,   9,   0,    0    },
//{ 64,  "Asia/Yakutsk",          1,   9,   0,    0    },
//{ 65,  "Australia/Adelaide",    1,   9,   30,   0    },
//{ 66,  "Australia/Darwin",  1,  9,   30, 0    },
//{ 67,  "Australia/Brisbane",    1,   10,  0,    0    },
//{ 68,  "Asia/Vladivostok",  1,  10,  0, 0    },
//{ 69,  "Pacific/Guam",          1,   10,  0,   0    },
//{ 70,  "Australia/Hobart",      1,   10,  0,    0    },
//{ 71,  "Australia/Sydney",  1,  10,  0, 0    },
//{ 72,  "Asia/Magadan",          1,   11,  0,    0    },
//{ 73,  "Pacific/Auckland",      1,   12,  0,    0    },
//{ 74,  "Pacific/Fiji",          1,   12,  0,    0    },
//{ 75,  "Pacific/Tongatapu",     1,   13,  0,    0    },
//{ 0,   NULL,           1,   8,   0,    0    },
//};


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
    bool loginFlag = false;      //login 标志位，登录成功才让操作

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
