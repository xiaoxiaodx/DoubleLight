#include "warntcpserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
WarnTcpServer::WarnTcpServer(QObject *parent) : QObject(parent)
{

}

void WarnTcpServer::destroySer()
{
    if(cliSocket != nullptr){
        cliSocket->abort();
        cliSocket->close();
        delete cliSocket;
        cliSocket = nullptr;
    }

    if(tcpServer != nullptr){
        tcpServer->close();
        delete tcpServer;
        tcpServer = nullptr;
    }
}

void WarnTcpServer::createSer(int port)
{
    //creat a server object
    if(tcpServer == nullptr){
        tcpServer = new QTcpServer;
        qDebug()<<"createServer    ";
        //set server object to listen client
        //set server's ip  to be the same as host
        //set server's port to be 8888
        tcpServer->listen(QHostAddress::AnyIPv4,port);
        connect(tcpServer,&QTcpServer::newConnection,this,&WarnTcpServer::slot_newConnect);
    }
}

void WarnTcpServer::slot_newConnect(){
    //get cliet's sockfd
    if(cliSocket != nullptr){
        cliSocket->abort();
        cliSocket->close();
        delete cliSocket;
        cliSocket = nullptr;
    }
    cliSocket = tcpServer->nextPendingConnection();
    connect(cliSocket,&QTcpSocket::readyRead,this,&WarnTcpServer::slot_readByte);
    //get client's ip and port

    QString cli_ip = cliSocket->peerAddress().toString();
    quint16 cli_port = cliSocket->peerPort();
    QString temp = QString("[%1:%2 connect success]").arg(cli_ip).arg(cli_port);
    qDebug() << temp;
}
void WarnTcpServer::slot_readByte()
{
    QByteArray msgdata=cliSocket->readAll();

    qDebug()<<" slot_ReadMsg    msgdata1    "<<QString(msgdata);

    HttpMsgCallBack(msgdata.data());
}
/*
void WarnTcpServer::slot_readByte()
{
    //http 消息
    QByteArray msgdata=cliSocket->readAll();

    parseStr.append(QString(msgdata.data()));

    qDebug()<<" slot_ReadMsg    msgdata1    "<<QString(msgdata);

    qDebug()<<" slot_ReadMsg    ***1    "<<parseStr;

    QStringList listData = parseStr.split("HTTP/1.1 ");
    int httpheadLen = QString("HTTP/1.1 ").length();
    int charOffset = 0;
    for (int i=0;i<listData.size();i++) {//解决连包问题
        QString oneData = QString(msgdata);//listData.at(i);

        QString keyContentLength = "Content-Length: ";

       //不包含 长度字段 则下一组测试
        if(!oneData.contains(keyContentLength)){
            //如果还有下一帧数据，则丢弃这一帧无效数据
            if((i+1)<listData.size())
                charOffset =charOffset + oneData.length() + httpheadLen;
            continue;
        }

        int contentOffset = oneData.indexOf(keyContentLength);

        QString keyConnect = "\r\nConnection";
       //不包含 长度字段 则下一组测试
        if(!oneData.contains(keyConnect)){
            //如果还有下一帧数据，则丢弃这一帧无效数据
            if((i+1)<listData.size())
                charOffset =charOffset + oneData.length() + httpheadLen;
            continue;
        }
        int contentOffset1 = oneData.indexOf(keyConnect);


        qDebug()<<  "contentOffset: "<<contentOffset1<<"    "<<contentOffset<<" "<<keyContentLength.length();
        QString contentLenStr = oneData.mid(contentOffset + keyContentLength.length(),contentOffset1-contentOffset-keyContentLength.length());
        bool isOk = false;
        int contentLen = contentLenStr.toInt(&isOk);
        if(!isOk){
            charOffset =charOffset + oneData.length() + httpheadLen;
            continue;
        }

        QString keyJson = "\r\n\r\n";
        //不包含 JSON字段 则下一组测试
         if(!oneData.contains(keyJson)){
             //如果还有下一帧数据，则丢弃这一帧无效数据
             if((i+1)<listData.size())
                 charOffset =charOffset + oneData.length() + httpheadLen;
             continue;
         }
         int jsonOffset = oneData.indexOf(keyJson);
         if(oneData.length() >= (jsonOffset+keyJson.length() + contentLen)){
             QString bodyData = oneData.mid(jsonOffset+keyJson.length(),contentLen);
             charOffset =charOffset + oneData.length() + httpheadLen;
             qDebug()<<"    bodyData    "<<bodyData<<"  " <<bodyData.length();
             HttpMsgCallBack(bodyData.toLatin1().data());

         }

        //解析分发
        //HttpMsgCallBack(bodyData);
    }
    parseStr.remove(0,charOffset);
    qDebug()<<" slot_ReadMsg    ***1";

}
*/
#include <QJsonParseError>
#include <QJsonObject>
int WarnTcpServer::HttpMsgCallBack(char * pData) {

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(pData, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
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
            }

            emit signal_WarnMsg(callbackMap);

        } else {
            qDebug()<<"not is document !";
        }
    }else {
        qDebug()<<"parse error ";
    }
}
