#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>

#include <fcntl.h>

#include "chttpapidevice.h"

#include <debuglog.h>


CHttpApiDevice::CHttpApiDevice(QString devid, QString ip, unsigned short port, QString usr, QString pas)
{
    this->g_ip = ip;
    this->g_port = port;
    memset(g_loginInfo.username, 0, sizeof(g_loginInfo.username));
    memset(g_loginInfo.password, 0, sizeof(g_loginInfo.password));

    sprintf(g_loginInfo.username, "%s", usr.toStdString().c_str());
    sprintf(g_loginInfo.password, "%s", pas.toStdString().c_str());

    g_tcpsocket = NULL;
    timeHttpSocket = NULL;


}

CHttpApiDevice::~CHttpApiDevice()
{
    QMap<QString , QVariant> map;
    map.insert("cmd","loginout");
    send_httpParSet(map);
}
void CHttpApiDevice::slot_heartimertout(){

    reconnectTimerCount++;
    if(reconnectInter * reconnectTimerCount >= 3*60*1000){
        reconnectTimerCount = 0;
        QMap<QString,QVariant> map;
        map.insert("cmd","keepalive");
        slot_httpParSet(map);
    }
}

void CHttpApiDevice::slot_sendtimerout()
{
    //QMutexLocker locker(&m_msgMutex);
    qDebug()<<"slot_sendtimerout    "<<listMsg.size();
    if(listMsg.size() <= 0){
        SendTimer->stop();
    }else{
        QMap<QString,QVariant> map = listMsg.takeFirst();
        bool isOK = false;
        int sendcount = map.value("sendCount",0).toInt(&isOK);
        if(isOK)
            map.insert("sendCount",sendcount+1);
        else
            map.insert("sendCount",1);
        //如果一条消息发送了多次则默认网络异常，则重新开始创建连接
        if(sendcount > 10){
            map.insert("sendCount",0);
            createConnect();
        }

        if(sendcount % 4 == 0){
            emit signal_sendMag(map);

            qDebug()<< "signal_sendMag  "<<map;

        }
        listMsg.append(map);

    }
}

void CHttpApiDevice::removeAlreadySend(QString cmd,QString msgid1){

    //QMutexLocker locker(&m_msgMutex);

    for (int i=0;i<listMsg.size();i++) {
        QMap<QString,QVariant> map = listMsg.at(i);
        QString tmpCmd = map.value("cmd").toString();
        QString msgid = map.value("msgid").toString();
        if(cmd.compare(tmpCmd)==0 && msgid.compare(msgid1) ==0){
            listMsg.removeAt(i);
            DebugLog::getInstance()->writeLog("remove msg:"+cmd);
            break;
        }
    }

}

QString CHttpApiDevice::createMsgId(QString cmd){
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
bool CHttpApiDevice::createConnect() {

    if (g_tcpsocket == NULL)
    {
        g_tcpsocket = new QTcpSocket;
        //timeHttpSocket = new QTimer;

        connect(g_tcpsocket, SIGNAL(readyRead()), this, SLOT(slot_ReadMsg()));
        connect(g_tcpsocket, &QTcpSocket::connected, this, &CHttpApiDevice::slot_Connected);

        SendTimer = new QTimer;
        reconnectTimer = new QTimer;
        connect(SendTimer,&QTimer::timeout,this,&CHttpApiDevice::slot_sendtimerout);
        connect(this,&CHttpApiDevice::signal_sendMag,this,&CHttpApiDevice::send_httpParSet);

        connect(reconnectTimer,&QTimer::timeout,this,&CHttpApiDevice::slot_heartimertout);
        reconnectTimer->start(reconnectInter);
        g_tcpsocket->setReadBufferSize(1024*1024);

    }else {
        g_tcpsocket->disconnectFromHost();
        g_tcpsocket->abort();
    }



    g_tcpsocket->connectToHost(this->g_ip,this->g_port);

    if(g_tcpsocket->waitForConnected(1000)){//setcurrenttime
        QMap<QString,QVariant> map;
        map.insert("cmd","setcurrenttime");
        send_httpParSet(map);
        return true;
    }else{
        return false;
    }
}

int CHttpApiDevice::CjsonMakeHttpHead(QJsonObject *msgObject, JsonMsg_T *msg) {
    if( msg == NULL) {
        qDebug()<<"make cjson head error !";
        return -1;
    }
    msgObject->insert("cmd", QString::fromUtf8( msg->cmd));
    msgObject->insert("method", QString::fromUtf8(msg->method));
    msgObject->insert("msgid", QString::fromUtf8(msg->msgID));
    if(0 < strlen(msg->ssionID) ) {
        msgObject->insert("sessionid", QString::fromUtf8(msg->ssionID));
    }
    return 0;
}
int CHttpApiDevice::SendRequestMsg(QJsonObject msg){
    QJsonDocument document;
    document.setObject(msg);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    QString httpSendmsg = "POST /HttpApi HTTP/1.1\r\nHost:"+g_ip+"\r\nAccept:*/*\r\nContent-Length:"+ \
            QString::number(strJson.length()) +"\r\n\r\n" +strJson;

    qDebug() <<"http send msg :"<<httpSendmsg;

    if(g_tcpsocket != nullptr && g_tcpsocket->isWritable()){
        g_tcpsocket->write(httpSendmsg.toStdString().c_str(), httpSendmsg.length());
        g_tcpsocket->flush();//不缓存，直接发送
    }
    return 0;
}
//http 消息拆包，分发
int CHttpApiDevice::HttpMsgCallBack(char * pData) {
    if (NULL == pData) {
        qDebug()<<" http callback msg is null ";
        return -1;
    }
    char bodyData[1024]={0};
    memset(&bodyData,0,sizeof(bodyData));
    int enable = 0;


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
                loginFlag = true;

                qDebug()<<"*****";
                if (object.contains("data")) {  // 包含指定的 key
                    QJsonValue value = object.value("data");
                    qDebug()<<"*****    1";
                    if ( value.isObject() ) {  // Page 的 value 是对象
                        QJsonObject obj = value.toObject();
                        qDebug()<<"*****    2";
                        if (obj.contains("sessionid")) {
                            QJsonValue value = obj.value("sessionid");
                            if (value.isString()) {
                                QString ssionId = value.toString();

                                sprintf(this->sessionId, "%s", ssionId.toStdString().c_str());
                                DebugLog::getInstance()->writeLog("get sessionID succ");
                                qDebug() << "登录成功   sessionid : " << sessionId;
                                return 0;
                            }
                        }
                    }else {
                        qDebug()<<"value not object ";
                    }
                } else {
                    DebugLog::getInstance()->writeLog("get sessionID fail");
                    qDebug()<<"not find data ";

                }

            } else if ("loginout" == cmd) {

                memset(this->sessionId, 0, sizeof(this->sessionId));
                qDebug() << "登出成功   sessionid : " << sessionId;

                return 0;
            } else if ( "getosdparam" == cmd ) {
                if( object.contains("data") ) {
                    callbackMap.insert("enable",object.value("data").toObject().value("time").toObject().value("enabled").toInt());
                }
            }else if ( "getrecordparam" == cmd ) {
                if( object.contains("data") ) {
                    callbackMap.insert("alldayenabled",object.value("data").toObject().value("time").toObject().value("alldayenabled").toInt());

                }
            }else if ( "setcurrenttime" == cmd ) {

            }else if ("getinftempmodel" == cmd) {
                qDebug()<<"rec getinftempmodel";
                callbackMap.insert("timeenable",object.value("data").toObject().value("timeenable").toInt());
                callbackMap.insert("tempmodel",object.value("data").toObject().value("tempmodel").toString());
            }else if ("getiradinfo" == cmd) {
                qDebug()<<"rec getiradinfo";

                callbackMap.insert("alarmtempEnable",object.value("data").toObject().value("alarmparam").toObject().value("enable").toInt());
                callbackMap.insert("alarmTemp",object.value("data").toObject().value("alarmparam").toObject().value("alarmtemp").toDouble());
                callbackMap.insert("tempdriftcaplevelMin",object.value("data").toObject().value("ctrlparamlevel").toObject().value("tempdriftcaplevel").toObject().value("min").toInt());

                callbackMap.insert("tempdriftcaplevelMax",object.value("data").toObject().value("ctrlparamlevel").toObject().value("tempdriftcaplevel").toObject().value("max").toInt());
                callbackMap.insert("tempcontrolcaplevelMin",object.value("data").toObject().value("ctrlparamlevel").toObject().value("tempdriftcaplevel").toObject().value("min").toInt());
                callbackMap.insert("tempcontrolcaplevelMax",object.value("data").toObject().value("ctrlparamlevel").toObject().value("tempdriftcaplevel").toObject().value("max").toInt());

                callbackMap.insert("tempdrift",object.value("data").toObject().value("ctrlparam").toObject().value("tempdrift").toInt());
                callbackMap.insert("tempcontrol",object.value("data").toObject().value("ctrlparam").toObject().value("tempcontrol").toInt());
                callbackMap.insert("osdenable",object.value("data").toObject().value("osdenable").toInt());
            }
            //            emit signal_MsgReply(cmd);
            //            qDebug()<<"signal_ReadMsg   ";

            DebugLog::getInstance()->writeLog("callbackMap:"+callbackMap.value("cmd").toString());
            emit signal_ReadMsg(callbackMap);

        } else {
            qDebug()<<"not is document !";
        }
    }else {
        qDebug()<<"parse error ";
    }
}

void CHttpApiDevice::slot_httpGetInitPar()
{
    DebugLog::getInstance()->writeLog(" slot_httpGetInitPar **************");
    QMap<QString,QVariant> map;
    map.insert("cmd","getosdparam");
    slot_httpParSet(map);
    map.insert("cmd","setcurrenttime");
    slot_httpParSet(map);
}

void CHttpApiDevice::slot_ReadMsg() {
    //http 消息
    QByteArray msgdata=g_tcpsocket->readAll();

    QString dataStr = QString( msgdata.data() );
    QStringList listData = dataStr.split("HTTP/1.1");

    qDebug()<<" slot_ReadMsg    ***1    "<<dataStr;
    //验证 HTTP消息，并进行消息分发
    for (int i=0;i<listData.size();i++) {//解决连包问题
        char bodyData[4096]={0};
        memset(&bodyData,0,sizeof(bodyData));

        QString oneData = "HTTP/1.1" + listData.at(i);
        if(oneData.size() < 15){
            continue;
        }


        char *pOneData = oneData.toLatin1().data();
        //消息是否正确
        if(!(strstr(pOneData,"HTTP/1.1 200 OK"))) {
            //ret = -4;
        }
        char* ptr = NULL;
        int bodylen = 0;
        ptr = strstr(pOneData,"Content-Length:");
        if(ptr) {
            bodylen = atoi(ptr+16);
        }

        if(!ptr)
            return;
        //find body
        ptr = strstr(ptr, "\r\n\r\n");
        if(ptr == NULL) {
            //ret = -1;
        }

        ptr = ptr + 4;
        memcpy(bodyData, ptr ,bodylen);

        qDebug()<<"i == "<<i<<", "<<bodyData;
        if(0 != strlen(bodyData) ) {
            //解析分发
            HttpMsgCallBack(bodyData);
            continue;
        }
    }
    qDebug()<<" slot_ReadMsg    ***1";
}
void CHttpApiDevice::slot_Connected() {
    QMap<QString,QVariant> map;
    map.insert("cmd","login");
    slot_httpParSet(map);
    qDebug()<<"connetc success !";
}

void CHttpApiDevice::slot_disconnected() {
    qDebug()<<"disconnect ";

}
void CHttpApiDevice::slot_connectServer() {
    qDebug()<<"time out  ";

}


void CHttpApiDevice::LoginDevice(QString msgid){
    JsonMsg_T info ={"login","request","","012345"};
    sprintf(info.msgID, "%s",msgid.toLatin1().data());
    QJsonObject dataObj;
    QJsonObject msgObject;
    qDebug()<<"===="<<QString(QLatin1String(g_loginInfo.username));
    CjsonMakeHttpHead(&msgObject, &info);
    dataObj.insert("username", QString(QLatin1String(g_loginInfo.username)));
    dataObj.insert("password", QString(QLatin1String(g_loginInfo.password)));
    msgObject.insert("data", QJsonValue(dataObj));
    SendRequestMsg(msgObject);
}
void CHttpApiDevice::LogoutDevice(QString msgid){
    JsonMsg_T info ={"loginout","request","","012345"};
    sprintf(info.msgID, "%s",msgid.toLatin1().data());
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);
    QJsonObject msgObject;
    CjsonMakeHttpHead(&msgObject, &info);
    SendRequestMsg(msgObject);
}
//void CHttpApiDevice::HttpKeepalive()
//{
//    JsonMsg_T info ={"keepalive","request","","012345"};
//    if(!strlen(this->sessionId)) {
//        qDebug() <<"ssionId error "<<sessionId;
//        return ;
//    }
//    sprintf(info.ssionID, "%s", this->sessionId);
//    QJsonObject msgObject;
//    CjsonMakeHttpHead(&msgObject, &info);
//    SendRequestMsg(msgObject);
//}

//void CHttpApiDevice::HttpGetOsdParam(){


//    JsonMsg_T info ={"getosdparam","request","","012345"};
//    if(!strlen(this->sessionId)) {
//        qDebug() <<"ssionId error "<<sessionId;
//        return ;
//    }
//    sprintf(info.ssionID, "%s", this->sessionId);
//    QJsonObject msgObject;
//    CjsonMakeHttpHead(&msgObject, &info);
//    SendRequestMsg(msgObject);
//}

void CHttpApiDevice::slot_httpParSet(QMap<QString,QVariant> map)
{
    //    curCmdState = map;

    //    QMap<QString ,QVariant> mapSend;
    //    mapSend.insert("cmd","login");
    //    send_httpParSet(mapSend);

    qDebug()<<" slot_httpParSet "<<map;
    //QMutexLocker locker(&m_msgMutex);

    QString msgid = createMsgId(map.value("cmd").toString());
    map.insert("msgid",msgid);
    listMsg.append(map);

    if(SendTimer != nullptr && !SendTimer->isActive())
        SendTimer->start(sendertimerInter);
}

bool CHttpApiDevice::send_httpParSet(QMap<QString,QVariant> map)
{

    qDebug()<<"send_httpParSet  "<<map;
    DebugLog::getInstance()->writeLog(" send_httpParSet :"+ map.value("cmd").toString());

    QString cmd = map.value("cmd").toString();
    QString msgid = map.value("msgid").toString();
    if(cmd.compare("setosdparam")==0){
        bool enable = map.value("enable").toBool();
        HttpSetOsdParam(enable,msgid);
    }else if(cmd.compare("login") ==0){
        LoginDevice(msgid);
    }else if(cmd.compare("loginout") ==0){
        LogoutDevice(msgid);
    }else if(cmd.compare("setcurrenttime") ==0){
        HttpSetDate(msgid);
    }else
        httpSendCommonCmd(cmd,msgid);
    /*else if(cmd.compare("getosdparam")==0){
        httpSendCommonCmd("getosdparam");
    }else if(cmd.compare("getinftempmodel") ==0){
        httpSendCommonCmd("getinftempmodel");
    }else if(cmd.compare("getiradinfo") ==0){
        httpSendCommonCmd("getiradinfo");
    }else if(cmd.compare("setiradinfo")==0){
        HttpSetIraInfo(map);
    }else if(cmd.compare("keepalive")==0){
        httpSendCommonCmd("keepalive");
    }*/
}

void CHttpApiDevice::httpSendCommonCmd(QString cmd,QString msgid)
{

    JsonMsg_T info;
    sprintf(info.cmd, "%s", cmd.toLatin1().data());
    sprintf(info.msgID, "%s", msgid.toLatin1().data());
    sprintf(info.method, "%s", "request");
    sprintf(info.ssionID, "%s", "");
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    CjsonMakeHttpHead(&msgObject, &info);
    SendRequestMsg(msgObject);
}

//void CHttpApiDevice::HttpGetIraInfo()
//{
//    JsonMsg_T info ={"getiradinfo","request","","012345"};
//    if(!strlen(this->sessionId)) {
//        qDebug() <<"ssionId error "<<sessionId;
//        return ;
//    }
//    sprintf(info.ssionID, "%s", this->sessionId);

//    QJsonObject msgObject;
//    CjsonMakeHttpHead(&msgObject, &info);
//    SendRequestMsg(msgObject);
//}

void CHttpApiDevice::HttpSetIraInfo(QVariantMap value)
{

    JsonMsg_T info ={"setiradinfo","request","","012345"};
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject dataObj, alarmparamObj,ctrlparamObj;

    CjsonMakeHttpHead(&msgObject, &info);

    dataObj.insert("osdenable",value.value("osdenable").toInt());

    alarmparamObj.insert("alarmtempEnable", value.value("alarmtempEnable").toInt());
    alarmparamObj.insert("alarmtemp", value.value("alarmtemp").toString().toDouble());

    ctrlparamObj.insert("tempdrift", value.value("tempdrift").toString().toInt());
    ctrlparamObj.insert("tempcontrol", value.value("tempcontrol").toString().toInt());

    dataObj.insert("alarmparam", QJsonValue(alarmparamObj));
    dataObj.insert("ctrlparam", QJsonValue(ctrlparamObj));
    msgObject.insert("data", QJsonValue(dataObj));

    SendRequestMsg(msgObject);
}

//void CHttpApiDevice::HttpGetDeviceType(){

//    JsonMsg_T info ={"getinftempmodel","request","","012345"};
//    if(!strlen(this->sessionId)) {
//        qDebug() <<"ssionId error "<<sessionId;
//        return ;
//    }
//    sprintf(info.ssionID, "%s", this->sessionId);

//    QJsonObject msgObject;
//    //QJsonObject dataObj;
//   // QDateTime dateT = QDateTime::currentDateTime();
//    CjsonMakeHttpHead(&msgObject, &info);
////    dataObj.insert("utc", dateT.toUTC().toString("yyyy-MM-ddThh:mm:ssZ"));//设置utc时间，格式："2000-10-10T03:39:44Z"
////    msgObject.insert("data", QJsonValue(dataObj));
//    SendRequestMsg(msgObject);
//}
void CHttpApiDevice::HttpSetDate(QString msgid)
{
    JsonMsg_T info ={"setcurrenttime","request","","012345"};
    sprintf(info.msgID, "%s",msgid.toLatin1().data());
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject dataObj;
    QDateTime dateT = QDateTime::currentDateTime();


    CjsonMakeHttpHead(&msgObject, &info);
    dataObj.insert("utc", dateT.toUTC().toString("yyyy-MM-ddThh:mm:ssZ"));//设置utc时间，格式："2000-10-10T03:39:44Z"
    msgObject.insert("data", QJsonValue(dataObj));
    SendRequestMsg(msgObject);
}

void CHttpApiDevice::HttpSetOsdParam(int osdTimeEnable,QString msgid){


    JsonMsg_T info ={"setosdparam","request","","012345"};
    sprintf(info.msgID, "%s",msgid.toLatin1().data());
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject dataObj, timeObj;

    CjsonMakeHttpHead(&msgObject, &info);

    timeObj.insert("enabled", osdTimeEnable);
    dataObj.insert("time", QJsonValue(timeObj));
    msgObject.insert("data", QJsonValue(dataObj));
    SendRequestMsg(msgObject);
}

void CHttpApiDevice::HttpGetMotiondetectParam(){


    JsonMsg_T info ={"getmotiondetectparam","request","","012345"};
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);
    QJsonObject msgObject;
    CjsonMakeHttpHead(&msgObject, &info);
    SendRequestMsg(msgObject);

    //设备登出
    // this->LogoutDevice();
}
void CHttpApiDevice::HttpSetMotiondetectParam(int enable){


    JsonMsg_T info ={"setmotiondetectparam","request","","012345"};
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject dataObj;

    CjsonMakeHttpHead(&msgObject, &info);

    dataObj.insert("enabled", enable);
    msgObject.insert("data", QJsonValue(dataObj));
    SendRequestMsg(msgObject);

}

void CHttpApiDevice::HttpGetRecordParam(){

}

void CHttpApiDevice::HttpSetRecordParam(int enable){

}

void CHttpApiDevice::slot_msgEvent() {

}

