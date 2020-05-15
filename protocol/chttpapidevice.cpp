#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <fcntl.h>
#include "chttpapidevice.h"
#include <debuglog.h>

#define warnPort 50001

CHttpApiDevice::CHttpApiDevice(QString devid, QString ip, unsigned short port, QString usr, QString pas)
{
    this->g_ip = ip;
    this->g_port = port;
    memset(g_loginInfo.username, 0, sizeof(g_loginInfo.username));
    memset(g_loginInfo.password, 0, sizeof(g_loginInfo.password));

    sprintf(g_loginInfo.username, "%s", usr.toStdString().c_str());
    sprintf(g_loginInfo.password, "%s", pas.toStdString().c_str());

    g_tcpsocket = NULL;

    warnPushMap.insert("ip",read_ip_address());
    warnPushMap.insert("switchSubscription",false);
    warnPushMap.insert("port",warnPort);
    warnPushMap.insert("isSubscription",false);
}

void CHttpApiDevice::slot_destoryConnect()
{
    qDebug()<<" slot_destoryConnect ";
    if(g_tcpsocket != NULL){
         qDebug()<<" slot_destoryConnect 1";
        if(SendTimer != nullptr){

             qDebug()<<" slot_destoryConnect 2";
            disconnect(g_tcpsocket, SIGNAL(readyRead()), this, SLOT(slot_ReadMsg()));

            disconnect(SendTimer,&QTimer::timeout,this,&CHttpApiDevice::slot_sendtimerout);
            disconnect(reconnectTimer,&QTimer::timeout,this,&CHttpApiDevice::slot_heartimertout);

            SendTimer->stop();
            reconnectTimer->stop();

            listMsg.clear();

            SendTimer->deleteLater();
            reconnectTimer->deleteLater();

            g_tcpsocket->disconnectFromHost();
            g_tcpsocket->abort();
            g_tcpsocket = nullptr;

            SendTimer = nullptr;
            reconnectTimer = nullptr;
             qDebug()<<" slot_destoryConnect 5";
        }

    }
}

CHttpApiDevice::~CHttpApiDevice()
{


    qDebug()<<"析构:  CHttpApiDevice  ";
    QMap<QString , QVariant> map;
    map.insert("cmd","loginout");
    send_httpParSet(map);

    slot_destoryConnect();
    if(warnTcpServer != nullptr){
        warnTcpServer->destroySer();
    }

    qDebug()<<"析构:  CHttpApiDevice  结束";
}

void CHttpApiDevice::slot_heartimertout(){

    reconnectTimerCount++;
    if(reconnectInter * reconnectTimerCount >= 3*60*1000){
        reconnectTimerCount = 0;
        QMap<QString,QVariant> map;
        map.insert("cmd","keepalive");
        slot_httpParSet(map);

        if(warnPushMap.value("switchSubscription").toBool())
            HttpSubscriptionWarn(warnPushMap, 0);
    }

    //QMap<QString,QVariant> map;
    //map.insert("cmd","getiradrect");
    //slot_httpParSet(map);
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
        if(sendcount > 20){
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


bool CHttpApiDevice::createConnect(){

    if (g_tcpsocket == nullptr)
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
                callbackMap.insert("tempcontrolcaplevelMin",object.value("data").toObject().value("ctrlparamlevel").toObject().value("tempcontrolcaplevel").toObject().value("min").toInt());
                callbackMap.insert("tempcontrolcaplevelMax",object.value("data").toObject().value("ctrlparamlevel").toObject().value("tempcontrolcaplevel").toObject().value("max").toInt());

                callbackMap.insert("tempdrift",object.value("data").toObject().value("ctrlparam").toObject().value("tempdrift").toInt());
                callbackMap.insert("tempcontrol",object.value("data").toObject().value("ctrlparam").toObject().value("tempcontrol").toInt());
                callbackMap.insert("osdenable",object.value("data").toObject().value("osdenable").toInt());
            }else if("pushalarm" == cmd){
                callbackMap.insert("alarmtype",object.value("data").toObject().value("alarmtype").toInt());
                callbackMap.insert("year",object.value("data").toObject().value("alarmtime").toObject().value("year").toInt());
                callbackMap.insert("mouth",object.value("data").toObject().value("alarmtime").toObject().value("month").toInt());
                callbackMap.insert("day",object.value("data").toObject().value("alarmtime").toObject().value("day").toInt());
                callbackMap.insert("hour",object.value("data").toObject().value("alarmtime").toObject().value("hour").toInt());
                callbackMap.insert("min",object.value("data").toObject().value("alarmtime").toObject().value("min").toInt());
                callbackMap.insert("sec",object.value("data").toObject().value("alarmtime").toObject().value("sec").toInt());
                callbackMap.insert("temperature",object.value("data").toObject().value("temperature").toDouble());
            }else if("update" == cmd){

                QString tmp="http://"+g_ip+":80/cgi-bin/hi3510/upgrade.cgi";
                callbackMap.insert("did","INEW-004122-JWGWM");
                callbackMap.insert("url",tmp);
            }else if ("getiradrect"==cmd) {
                callbackMap.insert("x0",object.value("data").toObject().value("x0").toInt());
                callbackMap.insert("y0",object.value("data").toObject().value("y0").toInt());
                callbackMap.insert("w0",object.value("data").toObject().value("w0").toInt());
                callbackMap.insert("h0",object.value("data").toObject().value("h0").toInt());

                callbackMap.insert("x1",object.value("data").toObject().value("x1").toInt());
                callbackMap.insert("y1",object.value("data").toObject().value("y1").toInt());
                callbackMap.insert("w1",object.value("data").toObject().value("w1").toInt());
                callbackMap.insert("h1",object.value("data").toObject().value("h1").toInt());

                callbackMap.insert("x2",object.value("data").toObject().value("x2").toInt());
                callbackMap.insert("y2",object.value("data").toObject().value("y2").toInt());
                callbackMap.insert("w2",object.value("data").toObject().value("w2").toInt());
                callbackMap.insert("h2",object.value("data").toObject().value("h2").toInt());

                qDebug()<<"rec rect:"<<object.value("data").toObject().value("x").toInt();
            }else if("getalarmparam" == cmd){

                callbackMap.insert("alarmaudiooutenabled",object.value("data").toObject().value("alarmaudiooutenabled").toInt());
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


    parseStr.append(QString(msgdata.data()));
    qDebug()<<" slot_ReadMsg    ***1    "<<parseStr;

    QStringList listData = parseStr.split("HTTP/1.1 ");
    int httpheadLen = QString("HTTP/1.1 ").length();
    int charOffset = 0;
    for (int i=0;i<listData.size();i++) {//解决连包问题
        QString oneData = listData.at(i);

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
void CHttpApiDevice::slot_Connected() {

    emit signal_httpConnected();
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

void CHttpApiDevice::slot_httpParSet(QMap<QString,QVariant> map)
{

    QString msgid = createMsgId(map.value("cmd").toString());
    map.insert("msgid",msgid);
    listMsg.append(map);

    if(SendTimer != nullptr && !SendTimer->isActive())
        SendTimer->start(sendertimerInter);
}

bool CHttpApiDevice::send_httpParSet(QMap<QString,QVariant> map)
{

    if(g_tcpsocket == nullptr)
        return false ;
    qDebug()<<"send_httpParSet  "<<map;
    DebugLog::getInstance()->writeLog("http_sendMsg :"+ map.value("cmd").toString());

    QString cmd = map.value("cmd").toString();
    QString msgid = map.value("msgid").toString();
    if(cmd.compare("setosdparam")==0){
        bool enable = map.value("enable").toBool();
        if(enable)
            HttpSetOsdParam(1,msgid);
        else
            HttpSetOsdParam(0,msgid);
    }else if(cmd.compare("login") ==0){
        LoginDevice(msgid);
    }else if(cmd.compare("loginout") ==0){
        LogoutDevice(msgid);
    }else if(cmd.compare("setcurrenttime") ==0){
        HttpSetDate(msgid);
    }else if(cmd.compare("setiradinfo")==0){
        HttpSetIraInfo(map, msgid);
    }else if(cmd.compare("alarmsubscription")==0){
        QString ip = read_ip_address();
        warnPushMap.insert("ip",ip);
        warnPushMap.insert("switchSubscription",true);
        warnPushMap.insert("port",warnPort);
        warnPushMap.insert("isSubscription",true);
        createWarnService(ip,warnPort);
        HttpSubscriptionWarn(warnPushMap, msgid);
    }else if(cmd.compare("unalarmsubscription")==0){
        warnPushMap.insert("ip",read_ip_address());
        warnPushMap.insert("switchSubscription",false);
        warnPushMap.insert("port",warnPort);
        warnPushMap.insert("isSubscription",false);
        destroyWarnService();
        HttpSubscriptionWarn(warnPushMap, msgid);
    }else if(cmd.compare("setmeasurablerange")==0){
        HttpSetMeasureRect(map);
    }else if(cmd.compare("setalarmparam")==0){
        HttpSetalarmparam(map);
    }else
        httpSendCommonCmd(cmd,msgid);

}

void CHttpApiDevice::HttpSetalarmparam(QVariantMap value){
    JsonMsg_T info;
    sprintf(info.cmd, "%s", "setalarmparam");
    sprintf(info.msgID, "%s", value.value("msgid").toString().toLatin1().data());
    sprintf(info.method, "%s", "request");
    sprintf(info.ssionID, "%s", "");
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject dataObj;

    CjsonMakeHttpHead(&msgObject, &info);

    dataObj.insert("alarmaudiooutenabled",value.value("alarmaudiooutenabled").toInt());


    msgObject.insert("data", QJsonValue(dataObj));

    SendRequestMsg(msgObject);
}
void CHttpApiDevice::HttpSetMeasureRect(QVariantMap value)
{
    JsonMsg_T info;
    sprintf(info.cmd, "%s", "setmeasurablerange");
    sprintf(info.msgID, "%s", value.value("msgid").toString().toLatin1().data());
    sprintf(info.method, "%s", "request");
    sprintf(info.ssionID, "%s", "");
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject dataObj, alarmparamObj,ctrlparamObj;

    CjsonMakeHttpHead(&msgObject, &info);

    dataObj.insert("x",value.value("x").toInt());
    dataObj.insert("y",value.value("y").toInt());
    dataObj.insert("w",value.value("w").toInt());
    dataObj.insert("h",value.value("h").toInt());

    //    alarmparamObj.insert("alarmtempEnable", value.value("alarmtempEnable").toInt());
    //    alarmparamObj.insert("alarmtemp", value.value("alarmtemp").toString().toDouble());

    //    ctrlparamObj.insert("tempdrift", value.value("tempdrift").toString().toInt());
    //    ctrlparamObj.insert("tempcontrol", value.value("tempcontrol").toString().toInt());

    //    dataObj.insert("alarmparam", QJsonValue(alarmparamObj));
    //    dataObj.insert("ctrlparam", QJsonValue(ctrlparamObj));
    msgObject.insert("data", QJsonValue(dataObj));

    SendRequestMsg(msgObject);
}

void CHttpApiDevice::destroyWarnService()
{
    if(warnTcpServer != nullptr){

        disconnect(warnTcpServer,&WarnTcpServer::signal_WarnMsg,this,&CHttpApiDevice::slot_WarnMsg);
        warnTcpServer->destroySer();
        delete  warnTcpServer;
        warnTcpServer = nullptr;
    }
}

void CHttpApiDevice::createWarnService(QString ip,int port)
{
    if(warnTcpServer == nullptr){
        warnTcpServer = new WarnTcpServer;
        warnTcpServer->createSer(ip,port);
        connect(warnTcpServer,&WarnTcpServer::signal_WarnMsg,this,&CHttpApiDevice::slot_WarnMsg);
    }
}
void CHttpApiDevice::slot_WarnMsg(QMap<QString,QVariant> map)
{
    emit signal_ReadMsg(map);
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
void CHttpApiDevice::HttpSubscriptionWarn(QMap<QString,QVariant> map,QString msgid)
{

    bool isSubscription = map.value("isSubscription").toBool();
    JsonMsg_T info;
    sprintf(info.cmd, "%s", "");
    sprintf(info.msgID, "%s", msgid.toLatin1().data());
    sprintf(info.method, "%s", "request");
    sprintf(info.ssionID, "%s", "");

    if(isSubscription){
        sprintf(info.cmd, "%s", "alarmsubscription");
    }else {
        sprintf(info.cmd, "%s", "unalarmsubscription");
    }

    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject  alarmparamObj;

    CjsonMakeHttpHead(&msgObject, &info);

    alarmparamObj.insert("alarmtype", map.value("alarmtype").toInt());
    alarmparamObj.insert("ipaddr", map.value("ip").toString());
    alarmparamObj.insert("port", map.value("port").toInt());
    alarmparamObj.insert("timeout", 300);
    // dataObj.insert("date", QJsonValue(alarmparamObj));

    msgObject.insert("data", QJsonValue(alarmparamObj));
    SendRequestMsg(msgObject);
}

#include <QNetworkInterface>

QString CHttpApiDevice::read_ip_address()
{
    QString ip_address;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&  ipAddressesList.at(i).toIPv4Address())
        {
            QHostAddress hostaddr = ipAddressesList.at(i);

            ip_address = hostaddr.toString();

            QStringList iplist = g_ip.split(".");


            if(iplist.size() >= 3 && ip_address.contains(iplist[0]) && ip_address.contains(iplist[1]) && ip_address.contains(iplist[2])){


                break;
            }

            /*
            DebugLog::getInstance()->writeLog("get LocalHost:"+QString(ip_address));
            QString isGlobal = hostaddr.isGlobal()?"true":"false";
            QString isLoopback = hostaddr.isLoopback()?"true":"false";
            QString isLinkLocal = hostaddr.isLinkLocal()?"true":"false";
            QString isBroadcast = hostaddr.isBroadcast()?"true":"false";
            QString isMulticast = hostaddr.isMulticast()?"true":"false";
            QString isSiteLocal = hostaddr.isSiteLocal()?"true":"false";
            QString isUniqueLocalUnicast = hostaddr.isUniqueLocalUnicast()?"true":"false";
            DebugLog::getInstance()->writeLog("isGlobal:"+isGlobal);
            DebugLog::getInstance()->writeLog("isLoopback:"+isLoopback);
            DebugLog::getInstance()->writeLog("isLinkLocal:"+isLinkLocal);
            DebugLog::getInstance()->writeLog("isBroadcast:"+isBroadcast);
            DebugLog::getInstance()->writeLog("isMulticast:"+isMulticast);
            DebugLog::getInstance()->writeLog("isSiteLocal:"+isSiteLocal);
            DebugLog::getInstance()->writeLog("isUniqueLocalUnicast:"+isUniqueLocalUnicast);
            */
        }
    }

    DebugLog::getInstance()->writeLog("warn server ip_address:"+ip_address);

    if (ip_address.isEmpty())
        ip_address = QHostAddress(QHostAddress::LocalHost).toString();
    return ip_address;
}
void CHttpApiDevice::HttpSetIraInfo(QVariantMap value,QString msgid)
{

    JsonMsg_T info;
    sprintf(info.cmd, "%s", "setiradinfo");
    sprintf(info.msgID, "%s", msgid.toLatin1().data());
    sprintf(info.method, "%s", "request");
    sprintf(info.ssionID, "%s", "");
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject dataObj, alarmparamObj,ctrlparamObj;

    CjsonMakeHttpHead(&msgObject, &info);

    dataObj.insert("osdenable",value.value("osdenable").toInt());

    alarmparamObj.insert("enable", value.value("alarmtempEnable").toInt());
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

