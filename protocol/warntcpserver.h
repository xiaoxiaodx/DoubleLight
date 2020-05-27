#ifndef WARNTCPSERVER_H
#define WARNTCPSERVER_H

#include <QObject>
#include <QVariant>

#define D_SYNCDATA_HEAD0 0x77
#define D_SYNCDATA_HEAD1 0x17
#define MAX_STREAM_MTU_SIZE 1450

typedef enum _E_ALARMTYPE
{
 ALARMTYPE_NULL = 0x00,
 ALARMTYPE_MOTION = 1 << 0,
 ALARMTYPE_BLIND = 1 << 1,//遮挡告警
 ALARMTYPE_IO1 = 1 << 2,
 ALARMTYPE_IO2 = 1 << 3,
 ALARMTYPE_SDCARD = 1 << 4,
 ALARMTYPE_SDCARD1 = 1 << 5,
 ALARMTYPE_LOWBAT = 0x41,
 ALARMTYPE_PIR = 0x42,
 ALARMTYPE_LOWFU = 0x43,
 ALARMTYPE_HEAT = 0x50, //高温告警 用于红外热感
 ALARMTYPE_NORMAL = 0x51, //正常温度上抛
 ALARMTYPE_MASK = 0x52, //口罩识别
}E_ALARMTYPE;


typedef struct _TimeYMD_T
{
 int year;
 int month;
 int day;
 int hour;
 int min;
 int sec;
}TimeYMD_T;

typedef struct _Time_T{
 int timeZone;//当前时区
 int weekday;
 TimeYMD_T curTime;//当前时间
}Time_T;

class QTcpServer;
class QTcpSocket;
class WarnTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit WarnTcpServer(QObject *parent = nullptr);

    ~WarnTcpServer();
    void createSer(QString ip,int port);
    void destroySer();
signals:
    void signal_WarnMsg(QMap<QString,QVariant>);
public slots:

    void slot_newConnect();
    void slot_readByte();
private:
    int HttpMsgCallBack(QByteArray arr);
    void HttpMsgCallBack1(QByteArray arr);
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *cliSocket = nullptr;
    QString parseStr = "";

    QByteArray msgdata;

    bool isFindHead = false;
    bool isFindMediaType;
};

#endif // WARNTCPSERVER_H
