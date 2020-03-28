#ifndef __HTTPAPIDEVICE_H__
#define __HTTPAPIDEVICE_H__


#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QString>
#include <QTimer>

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

class CHttpApiDevice : public QObject
{

    Q_OBJECT

public:
    CHttpApiDevice(QString devid, QString ip, unsigned short port, QString usr, QString pas);//设备
    ~CHttpApiDevice();

    int CjsonMakeHttpHead(QJsonObject *msgObject, JsonMsg_T *msg);
    int SendRequestMsg(QJsonObject msg);

    int InitHttpSocket();

    void LoginDevice();
    void LogoutDevice();
    int HttpMsgCallBack(char * pData);
    void HttpGetOsdParam();
    void HttpSetOsdParam(int osdTimeEnable);
    void HttpGetMotiondetectParam();
    void HttpSetMotiondetectParam(int);
    void HttpGetRecordParam();
    void HttpSetRecordParam(int);
public:

private:
        QString g_ip;//http服务器地址
        unsigned short g_port;//http服务器断开

        LoginInfo_T g_loginInfo;//用户信息
        QTcpSocket *g_tcpsocket;//http socket
        bool g_httpTcpConnectState;//连接状态
        QTimer *timeHttpSocket; //定时器重连
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
signals:
        void signal_ReadMsg(int, QString);//QJsonObject
};

#endif
