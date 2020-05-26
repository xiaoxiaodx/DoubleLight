#include "warntcpserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "windows.h"
WarnTcpServer::WarnTcpServer(QObject *parent) : QObject(parent)
{

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

void WarnTcpServer::slot_newConnect(){
    //get cliet's sockfd
    if(cliSocket != nullptr){
        //disconnect(cliSocket,&QTcpSocket::readyRead,this,&WarnTcpServer::slot_readByte);
        // cliSocket->disconnectFromHost();
        cliSocket->abort();
        cliSocket->close();
        delete cliSocket;
        cliSocket = nullptr;
        //connect(cliSocket,&QTcpSocket::readyRead,this,&WarnTcpServer::slot_readByte);
    }


    cliSocket = tcpServer->nextPendingConnection();
  //  cliSocket->bind(cliSocket->peerPort(),QAbstractSocket::ReuseAddressHint);
    connect(cliSocket,&QTcpSocket::readyRead,this,&WarnTcpServer::slot_readByte);
    //get client's ip and port

//    QString cli_ip = cliSocket->peerAddress().toString();
//    quint16 cli_port = cliSocket->peerPort();
//    QString temp = QString("[%1:%2 connect success]").arg(cli_ip).arg(cli_port);
//    qDebug() << temp;
}

void WarnTcpServer::slot_readByte()
{
    //QByteArray msgdata=cliSocket->readAll();


    msgdata.append(cliSocket->readAll());
    qDebug()<<" slot_ReadMsg    msgdata1    "<<QString(msgdata);

    HttpMsgCallBack(msgdata.data());
}

#include <QJsonParseError>
#include <QJsonObject>
int WarnTcpServer::HttpMsgCallBack(char * pData) {

    QJsonParseError jsonError;


    qDebug()<<"报警推送";
    QJsonDocument doucment = QJsonDocument::fromJson(pData, &jsonError);  // 转化为 JSON 文档
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
                callbackMap.insert("mouth",object.value("data").toObject().value("alarmtime").toObject().value("month").toInt());
                callbackMap.insert("day",object.value("data").toObject().value("alarmtime").toObject().value("day").toInt());
                callbackMap.insert("hour",object.value("data").toObject().value("alarmtime").toObject().value("hour").toInt());
                callbackMap.insert("min",object.value("data").toObject().value("alarmtime").toObject().value("min").toInt());
                callbackMap.insert("sec",object.value("data").toObject().value("alarmtime").toObject().value("sec").toInt());
                callbackMap.insert("temperature",object.value("data").toObject().value("temperature").toString().toFloat());
                if(object.value("data").toObject().contains("imagedata"))
                    callbackMap.insert("imagedata",object.value("data").toObject().value("imagedata").toString());
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
