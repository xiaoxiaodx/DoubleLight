#include "faceimport.h"
#include <QVariant>
#include <QJsonObject>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
#include "debuglog.h"
FaceImport::FaceImport(QObject *parent) : QObject(parent)
{

}

QString FaceImport::createMsgId(QString cmd){
    //QMutexLocker locker(&m_msgMutex);
    int msgId = 0;
    for (int i=0;i<listMsg.size();i++) {
        QMap<QString,QVariant> map = listMsg.at(i);
        QString tmpCmd = map.value("cmd").toString();
        QString msgid = map.value("msgid").toString();
        if(cmd.compare(tmpCmd)==0){
            int tmpid = msgid.toInt();
            if(msgId < tmpid){
                msgId = tmpid+1;
            }
        }
    }
    return QString::number(msgId);
}

void FaceImport::removeAlreadySend(QString cmd,QString msgid1){

    //QMutexLocker locker(&m_msgMutex);

    int msgLen = listMsg.size();
    for (int i=msgLen-1;i>=0;i--){
        QMap<QString,QVariant> map = listMsg.at(i);
        QString tmpCmd = map.value("cmd").toString();
        QString msgid = map.value("msgid").toString();
        if(cmd.compare(tmpCmd)==0 && msgid.compare(msgid1) ==0){
            listMsg.removeAt(i);
            DebugLog::getInstance()->writeLog("faceObject :remove msg "+cmd);
            break;
        }
    }
}

void FaceImport::slot_createConnect(QString ip,int port){

    listMsg.clear();
    if (g_tcpsocket == nullptr)
    {
        g_tcpsocket = new QTcpSocket;
        //timeHttpSocket = new QTimer;

        connect(g_tcpsocket, SIGNAL(readyRead()), this, SLOT(slot_ReadMsg()));
        connect(g_tcpsocket, &QTcpSocket::connected, this, &FaceImport::slot_Connected);

        sendTimer = new QTimer;

        connect(sendTimer,&QTimer::timeout,this,&FaceImport::slot_sendtimerout);

    }else {
        g_tcpsocket->disconnectFromHost();
        g_tcpsocket->abort();
    }

    g_ip = ip;
    g_port = port;
    g_tcpsocket->connectToHost(this->g_ip,this->g_port);

}

void FaceImport::slot_destoryConnect()
{

    qDebug()<<" slot_destoryConnect ";
    if(g_tcpsocket != NULL){



        if(sendTimer != nullptr){

            qDebug()<<" slot_destoryConnect 2";
            disconnect(g_tcpsocket, SIGNAL(readyRead()), this, SLOT(slot_ReadMsg()));
            disconnect(sendTimer,&QTimer::timeout,this,&FaceImport::slot_sendtimerout);



            sendTimer->stop();

            listMsg.clear();


            sendTimer->deleteLater();


            g_tcpsocket->disconnectFromHost();
            g_tcpsocket->abort();
            g_tcpsocket = nullptr;

            sendTimer = nullptr;


        }

    }
}

void FaceImport::slot_sendMsg(QVariantMap map)
{

    if(!isConnected){
        DebugLog::getInstance()->writeLog("face import fail,--- network is disconnnect ");
        return;
    }

    qDebug()<<"slot_sendMsg";
    listMsg.append(map);

    if(sendTimer->isActive())
        sendTimer->start(200);

}

void FaceImport::slot_sendtimerout()
{


    if(listMsg.size() > 0){

        QVariantMap map =  listMsg.at(0);

        if(map.contains("sendTime")){

            qint64 curt = QDateTime::currentMSecsSinceEpoch();
            qint64 sendt = map.value("sendTime").toLongLong();

            if(curt - sendt > 2000){


            }


        }else {

            qint64 sendt = QDateTime::currentMSecsSinceEpoch();
            map.insert("sendTime",sendt);
            SendRequestMsg(makeJsonData(map));
            listMsg.append(map);
        }


    }else{
        sendTimer->stop();
    }

}

void FaceImport::slot_Connected()
{

}

void FaceImport::SendRequestMsg(QJsonObject obj){
    QJsonDocument document;
    document.setObject(obj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    QString httpSendmsg = "POST /faceput  HTTP/1.1\r\nHost:"
                          +g_ip+":"+QString::number(g_port)+"\r\nConnection: keep-alive\r\nContent-Length:"
                        +QString::number(strJson.length())+"\r\nPragma: no-cache\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36\r\n"
                         +"Content-Type: application/json\r\nAccept: */*\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: zh-CN,zh;q=0.9\r\n\r\n";


    qDebug() <<"http send msg :"<<httpSendmsg;

    if(g_tcpsocket != nullptr && g_tcpsocket->isWritable()){
        qint64 sendlen = g_tcpsocket->write(httpSendmsg.toStdString().c_str(), httpSendmsg.length());
        qDebug()<<"-----sendlen----:"<<sendlen;
        g_tcpsocket->flush();//不缓存，直接发送
    }
}


QJsonObject FaceImport::makeJsonData(QVariantMap map){

    QString cmd = map.value("cmd").toString();

    QJsonObject objMain;
    objMain.insert("cmd",map.value("cmd").toString());
    objMain.insert("msgid",createMsgId(cmd));
    objMain.insert("methods",map.value("request").toString());
    QJsonObject objData;
    if(cmd.compare("addface") == 0){

        QString imgFileAbsolute = map.value("filePath").toString();

        QFile file(imgFileAbsolute);
        if(file.open(QIODevice::ReadOnly)){
            objData.insert("name",map.value("name").toString());
            objData.insert("seq",map.value("seq").toString());
            QByteArray arr = file.readAll();
            QString facedata = arr.toBase64();
            objData.insert("facedata",facedata);
        }else {

            qDebug()<<"face Image open fail";
        }
    }else if(cmd.compare("delface") == 0){

        objData.insert("name",map.value("name").toString());
        objData.insert("seq",map.value("seq").toString());

    }else if(cmd.compare("modifyface") == 0){

        objData.insert("name",map.value("name").toString());
        objData.insert("seq",map.value("seq").toString());

    }

    objMain.insert("data",QJsonValue(objData));
    return objMain;
}

void FaceImport::slot_ReadMsg() {
    //http 消息
    QByteArray msgdata=g_tcpsocket->readAll();


    parseStr.append(QString(msgdata.data()));
    qDebug()<<" slot_ReadMsg    ***1    "<<parseStr;

    QStringList listData = parseStr.split("HTTP/1.1 ");
    int httpheadLen = QString("HTTP/1.1 ").length();
    int charOffset = 0;
    for (int i=0;i<listData.size();i++) {//解决连包问题
        QString oneData = listData.at(i);

        QString stateCode = oneData.mid(0,3);


        qDebug()<<">>>>>>"<<i<<" stateCode "<<stateCode;


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


    }
    parseStr.remove(0,charOffset);
    qDebug()<<" slot_ReadMsg    ***1";
}

int FaceImport::HttpMsgCallBack(char * pData) {

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(pData, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象

            QJsonObject object = doucment.object();  // 转化为对象
            QString cmd = object.value("cmd").toString();
            QString msgid = object.value("msgid").toString();
            removeAlreadySend(cmd,msgid);
            qDebug()<<"接收的命令:"<<cmd;
            QMap<QString,QVariant> callbackMap;
            callbackMap.insert("cmd",cmd);
            if("login" == cmd) {

            }

        } else {
            qDebug()<<"not is document !";
        }
    }else {
        qDebug()<<"parse error ";
    }
}
