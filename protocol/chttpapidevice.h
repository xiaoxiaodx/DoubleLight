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

    bool createConnect();


    int  HttpMsgCallBack(char * pData);

public:

private:
    QString g_ip;//http服务器地址
    unsigned short g_port;//http服务器断开

    LoginInfo_T g_loginInfo;//用户信息
    QTcpSocket *g_tcpsocket = nullptr;//http socket
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
    void slot_httpParSet(QMap<QString,QVariant> map);
    void slot_httpGetInitPar();
    void slot_resendCmd();
    //void slot_exitLoop(QString );
signals:
    void signal_ReadMsg(QMap<QString,QVariant>);//QJsonObject
    void signal_httpErr();
    void signal_MsgReply(QString cmd);
private:
    int connectCount = 0;

    bool send_httpParSet(QMap<QString,QVariant> map);
    void LoginDevice();
    void LogoutDevice();
    void HttpGetOsdParam();
    void HttpSetOsdParam(int osdTimeEnable);
    void HttpGetMotiondetectParam();
    void HttpSetMotiondetectParam(int);
    void HttpGetRecordParam();
    void HttpSetRecordParam(int);
    void HttpSetDate();
    QMap<QString,QVariant> curCmdState;

    QList<QMap<QString,QVariant>> listResendCmd;
    const bool cmdSend = false;
    const bool cmdSendSucc = true;
    QMutex mMutex;

    QTimer *timerReSendCmd = nullptr;


};

#endif
