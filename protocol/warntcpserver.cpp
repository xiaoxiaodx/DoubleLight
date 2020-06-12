#include "warntcpserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "windows.h"
WarnTcpServer::WarnTcpServer(QObject *parent) : QObject(parent)
{

}

WarnTcpServer::~WarnTcpServer(){

    destroySer();
}

void WarnTcpServer::destroySer()
{

    if(tcpServer != nullptr){
        disconnect(tcpServer,&QTcpServer::newConnection,this,&WarnTcpServer::slot_newConnect);
        tcpServer->close();
        tcpServer = nullptr;
    }

    if(cliSocket != nullptr){
        disconnect(cliSocket,&QTcpSocket::readyRead,this,&WarnTcpServer::slot_readByte);
        cliSocket->disconnectFromHost();
        cliSocket->abort();
        cliSocket->close();
        delete cliSocket;
        cliSocket = nullptr;
    }

}

#include <QThread>
void WarnTcpServer::createSer(QString ip,int port)
{
    //creat a server object

    qDebug()<<" createSer   "<<QThread::currentThreadId();
    if(tcpServer == nullptr){
        tcpServer = new QTcpServer;
        qDebug()<<"createServer    ";

        SOCKET sockfd = 0;
        struct sockaddr_in servAddr;
        sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            qDebug()<<("can't open socket");
            return ;
        }

        int flag = 1;
        if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(flag)) != 0) {
            qDebug()<<("can't set SO_REUSEADDR");
            return ;
        }

        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = INADDR_ANY;
        servAddr.sin_port = htons(port);

        if (::bind(sockfd, (struct sockaddr*)&servAddr, sizeof(sockaddr_in)) != 0) {
            qDebug()<<("can't bind socket");
            return ;
        }

        if (::listen(sockfd, SOMAXCONN) != 0) {
            qDebug()<<("can't listen on port");
            return ;
        }

        tcpServer->setSocketDescriptor(sockfd);
        qDebug()<<("开启监听");
        //tcpServer->listen(QHostAddress(ip),port);
        connect(tcpServer,&QTcpServer::newConnection,this,&WarnTcpServer::slot_newConnect);
    }
}

void WarnTcpServer::setCurWarnIp(QString ip)
{
    curWarnIp = ip;
}

void WarnTcpServer::slot_newConnect(){
    //get cliet's sockfd
    if(cliSocket != nullptr){
        disconnect(cliSocket,&QTcpSocket::readyRead,this,&WarnTcpServer::slot_readByte);
        cliSocket->disconnectFromHost();
        cliSocket->abort();
        cliSocket->close();
        delete cliSocket;
        cliSocket = nullptr;
        //connect(cliSocket,&QTcpSocket::readyRead,this,&WarnTcpServer::slot_readByte);
    }



    cliSocket = tcpServer->nextPendingConnection();
    QString cli_ip = cliSocket->peerAddress().toString();
    quint16 cli_port = cliSocket->peerPort();
    QString temp = QString("[%1:%2 connect success]").arg(cli_ip).arg(cli_port);

    if(curWarnIp.compare(cli_ip)!=0){
        cliSocket->disconnectFromHost();
        cliSocket->abort();
        cliSocket->close();
        delete cliSocket;
        cliSocket = nullptr;
        return;
    }

    connect(cliSocket,&QTcpSocket::readyRead,this,&WarnTcpServer::slot_readByte);
    //get client's ip and port
}

void WarnTcpServer::slot_readByte()
{
    QByteArray msgdata1=cliSocket->readAll();

    // msgdata.append(cliSocket->readAll());
    qDebug()<<" slot_ReadMsg    msgdata1    "<<msgdata1.length();
    //qDebug()<<" slot_ReadMsg    msgdata1    "<<QString(msgdata1);

    HttpMsgCallBack(msgdata1);
}




typedef struct _SuitProtocolMsgHead_T{
    unsigned char sysncHead0;
    unsigned char sysncHead1;
    unsigned char cmd;
    unsigned char staty0;
    unsigned int msgLen;
}SuitProtocolMsgHead_T;

typedef struct _PushFaceAlarmMsg_T
{
    E_ALARMTYPE  alarmType;
    Time_T alarmTime;
    float temperature;
    int datalen;
}PushFaceAlarmMsg_T;

#include <QJsonParseError>
#include <QJsonObject>
#include <QDateTime>
void WarnTcpServer::HttpMsgCallBack1(QByteArray arr)
{
    msgdata.append(arr);
    int needlen = 8;

    while(msgdata.length() >= needlen)
    {
        if(!isFindHead)
        {
            if(msgdata.at(0) == D_SYNCDATA_HEAD0 && msgdata.at(1)==D_SYNCDATA_HEAD1)
            {

                qDebug()<<"find head ";
                isFindHead = true;
                int i1 = msgdata.at(4) & 0x000000ff;
                int i2 = msgdata.at(5) & 0x000000ff;
                int i3 = msgdata.at(6) & 0x000000ff;
                int i4 = msgdata.at(7) & 0x000000ff;

                needlen = i1+ i2*256 + i3*256*256 + i4*256*256*256;
                msgdata.remove(0,8);
            }else {
                msgdata.remove(0,1);
                continue;
            }
        }


        QMap <QString,QVariant> callbackMap;


        PushFaceAlarmMsg_T *faceAlarm = (PushFaceAlarmMsg_T*)msgdata.data();

       // qDebug()<<"faceAlarm->datalen"<<faceAlarm->datalen;

        callbackMap.insert("alarmtype",faceAlarm->alarmType);
        callbackMap.insert("year",faceAlarm->alarmTime.curTime.year);
        callbackMap.insert("month",faceAlarm->alarmTime.curTime.month);
        callbackMap.insert("day",faceAlarm->alarmTime.curTime.day);
        callbackMap.insert("hour",faceAlarm->alarmTime.curTime.hour);
        callbackMap.insert("min",faceAlarm->alarmTime.curTime.min);
        callbackMap.insert("sec",faceAlarm->alarmTime.curTime.sec);
        callbackMap.insert("temperature",faceAlarm->temperature);


        QDate tmpDate(callbackMap.value("year").toInt(),callbackMap.value("month").toInt(),callbackMap.value("day").toInt());
        QTime tmptime(callbackMap.value("hour").toInt(),callbackMap.value("min").toInt(),callbackMap.value("sec").toInt());

        QDateTime datetime(tmpDate,tmptime);

        qDebug()<<" datetime:"<<datetime.toString("yyyy-MM-dd hh:mm:ss");

        QByteArray imgarr;
        imgarr = msgdata.mid(44,faceAlarm->datalen);

        callbackMap.insert("imagedata",imgarr);

        emit signal_WarnMsg(callbackMap);

        msgdata.remove(0,needlen);

        isFindHead = false;
        needlen = 8;
    }
}

int WarnTcpServer::HttpMsgCallBack(QByteArray arr) {

    QJsonParseError jsonError;

    msgdata.append(arr);

    QString tmpJsonStr = QString(msgdata);

    QByteArray jsonArr;
    QString startFlagStr = "--boundarydonotcrossstart";
    QString endFlagStr = "--boundarydonotcrossend";


    if(tmpJsonStr.contains("--boundarydonotcrossstart") && tmpJsonStr.contains("--boundarydonotcrossend")){

        int frameStart = tmpJsonStr.indexOf(startFlagStr);
        int jsonStart = frameStart + startFlagStr.length();
        int jsonEnd = tmpJsonStr.indexOf(endFlagStr);

        int frameEndlen = jsonEnd + endFlagStr.length();

        jsonArr = msgdata.mid(jsonStart,jsonEnd-jsonStart);
        msgdata.remove(0,frameEndlen);
    }else
        return 1;

    QJsonDocument doucment = QJsonDocument::fromJson(jsonArr.data(), &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)){  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象

            QJsonObject object = doucment.object();  // 转化为对象
            QString cmd = object.value("cmd").toString();
            QString msgid = object.value("msgid").toString();

            QMap <QString,QVariant> callbackMap;
            if("pushalarm" == cmd){
                callbackMap.insert("cmd",cmd);
                callbackMap.insert("msgid",msgid);
                callbackMap.insert("alarmtype",object.value("data").toObject().value("alarmtype").toInt());
                callbackMap.insert("year",object.value("data").toObject().value("alarmtime").toObject().value("year").toInt());
                callbackMap.insert("month",object.value("data").toObject().value("alarmtime").toObject().value("month").toInt());
                callbackMap.insert("day",object.value("data").toObject().value("alarmtime").toObject().value("day").toInt());
                callbackMap.insert("hour",object.value("data").toObject().value("alarmtime").toObject().value("hour").toInt());
                callbackMap.insert("min",object.value("data").toObject().value("alarmtime").toObject().value("min").toInt());
                callbackMap.insert("sec",object.value("data").toObject().value("alarmtime").toObject().value("sec").toInt());
                callbackMap.insert("temperature",object.value("data").toObject().value("temperature").toString().toFloat());
                if(object.value("data").toObject().contains("imagedata"))
                    callbackMap.insert("imagedata",object.value("data").toObject().value("imagedata").toString());
                if(object.value("data").toObject().contains("snapimagedata")){

                    qDebug()<<"snapimagedata    *****";
                    callbackMap.insert("snapimagedata",object.value("data").toObject().value("snapimagedata").toString());
                }
            }

            emit signal_WarnMsg(callbackMap);
        } else {
            qDebug()<<"not is document !";

        }
        msgdata.clear();
    } else {
        qDebug()<<"parse error "<<jsonError.error;
    }
}
