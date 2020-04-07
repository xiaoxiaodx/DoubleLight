#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>

#include <fcntl.h>

#include "chttpapidevice.h"




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



    timeoutCnt = new QTimer;



}

CHttpApiDevice::~CHttpApiDevice()
{

}

bool CHttpApiDevice::createConnect() {

    if (g_tcpsocket == NULL)
    {
        g_tcpsocket = new QTcpSocket;
        //timeHttpSocket = new QTimer;

        timerReSendCmd = new QTimer;
        connect(timerReSendCmd, &QTimer::timeout, this, &CHttpApiDevice::slot_resendCmd);

        connect(g_tcpsocket, SIGNAL(readyRead()), this, SLOT(slot_ReadMsg()));
        // connect(g_tcpsocket, SIGNAL(connected()), this, SLOT(slot_Connected()));
        // connect(g_tcpsocket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
        // connect(timeHttpSocket, SIGNAL(timeout()), this, SLOT(slot_connectServer()));

        g_tcpsocket->setReadBufferSize(1024*1024);
        //g_tcpsocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    }


    g_tcpsocket->connectToHost(this->g_ip,this->g_port);

    if(g_tcpsocket->waitForConnected(1000)){
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


            if(curCmdState.value("cmd").toString().compare(cmd))
            {
                qDebug()<<"接收的命令与发送的命令不一致";
            }
            curCmdState.insert("state",cmdSendSucc);


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
                                qDebug() << "sessionid : " << sessionId;
                            }
                        }
                    }else {
                        qDebug()<<"value not object ";
                    }
                } else {
                    qDebug()<<"not find data ";
                    // return -1;
                }
            } else if ("loginout" == cmd) {
                memset(this->sessionId, 0, sizeof(this->sessionId));

            } else if ( "getosdparam" == cmd ) {
                if( object.contains("data") ) {

                    callbackMap.insert("enable",object.value("data").toObject().value("time").toObject().value("enabled").toInt());


                }
            }else if ( "getrecordparam" == cmd ) {
                if( object.contains("data") ) {
                    callbackMap.insert("alldayenabled",object.value("data").toObject().value("time").toObject().value("alldayenabled").toInt());


                }
            }


            qDebug()<<"接收的命:"<<cmd;
            emit signal_MsgReply(cmd);
            qDebug()<<"signal_ReadMsg   ";
            emit signal_ReadMsg(callbackMap);

        } else {
            qDebug()<<"not is document !";
        }
    }else {
        qDebug()<<"parse error ";
    }
}

void CHttpApiDevice::slot_resendCmd(){

    if(listResendCmd.size() <= 0){
        if(timerReSendCmd != nullptr)
            timerReSendCmd->stop();
        return;
    }


    QMap<QString,QVariant> map = listResendCmd.takeFirst();
    slot_httpParSet(map);

}
void CHttpApiDevice::slot_httpGetInitPar()
{
    qDebug()<<" slot_httpGetInitPar **************";
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

    qDebug()<<" slot_ReadMsg    ***1";
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
    this->g_httpTcpConnectState = true;
    // timeHttpSocket->stop();
    qDebug()<<"connetc success !";
}

void CHttpApiDevice::slot_disconnected() {
    qDebug()<<"disconnect ";
    // g_httpTcpConnectState = false;
}
void CHttpApiDevice::slot_connectServer() {
    qDebug()<<"time out  ";
    //    if(false == g_httpTcpConnectState) {
    //        g_tcpsocket->abort();
    //        g_tcpsocket->connectToHost(g_ip, g_port);
    //    } else {
    //        timeHttpSocket->stop();
    //    }
}


void CHttpApiDevice::LoginDevice(){
    JsonMsg_T info ={"login","request","","012345"};
    QJsonObject dataObj;
    QJsonObject msgObject;
    qDebug()<<"===="<<QString(QLatin1String(g_loginInfo.username));
    CjsonMakeHttpHead(&msgObject, &info);
    dataObj.insert("username", QString(QLatin1String(g_loginInfo.username)));
    dataObj.insert("password", QString(QLatin1String(g_loginInfo.password)));
    msgObject.insert("data", QJsonValue(dataObj));
    SendRequestMsg(msgObject);
}
void CHttpApiDevice::LogoutDevice(){
    JsonMsg_T info ={"loginout","request","","012345"};
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);
    QJsonObject msgObject;
    CjsonMakeHttpHead(&msgObject, &info);
    SendRequestMsg(msgObject);
}


void CHttpApiDevice::HttpGetOsdParam(){


    JsonMsg_T info ={"getosdparam","request","","012345"};
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);
    QJsonObject msgObject;
    CjsonMakeHttpHead(&msgObject, &info);
    SendRequestMsg(msgObject);


}
bool CHttpApiDevice::send_httpParSet(QMap<QString,QVariant> map)
{


    // QMutexLocker locker(&mMutex);
    QString cmd = map.value("cmd").toString();

    //QTimer timer;
    curCmdState.insert("cmd",cmd);
    curCmdState.insert("state",cmdSend);
    curCmdState.insert("checkedCount",0);

    qDebug()<<" send_httpParSet "<<map;


    QEventLoop loop ;
    QMetaObject::Connection connet = connect(this,&CHttpApiDevice::signal_MsgReply,[&](QString cmd){
            qDebug()<<"signal_MsgReply "<<cmd;

            if(loop.isRunning())
                loop.exit();
            disconnect(connet);});

    QTimer timer;
    QMetaObject::Connection timeconnet = connect(&timer,&QTimer::timeout,[&](){
         qDebug()<<"************* ";
            if(loop.isRunning())
                loop.exit();
            });
    timer.start(2000);

    if(cmd.compare("setosdparam")==0){
        bool enable = map.value("enable").toBool();
        HttpSetOsdParam(enable);
    }else if(cmd.compare("getosdparam")==0){
        HttpGetOsdParam();
    }else if(cmd.compare("login") ==0){
        if(createConnect()){
            qDebug()<<"http 连接成功";
            LoginDevice();
        }else{
            qDebug()<<"http 连接超时";
            if(loop.isRunning())
                loop.exit();

        }
    }else if(cmd.compare("loginout") ==0){
        LogoutDevice();
    }else if(cmd.compare("setcurrenttime") ==0){
        HttpSetDate();
    }

    qDebug()<<"开始循环***";
    loop.exec();
    qDebug()<<"结束循环***";

    disconnect(timeconnet);
    timer.stop();

    QString sendCmd = curCmdState.value("cmd").toString();
    if(cmd.compare(sendCmd) == 0 && curCmdState.value("state").toBool()){
        return true;
    }else{
        return false;
    }

}

void CHttpApiDevice::slot_httpParSet(QMap<QString,QVariant> map)
{

    qDebug()<<" slot_httpParSet";
    QMap<QString ,QVariant> mapSend;
    mapSend.insert("cmd","login");

    qDebug()<<"+****1";
    bool sendSucc = false;
    if(send_httpParSet(mapSend)){//登录
        qDebug()<<"+****2";
        if(send_httpParSet(map)){//登出
            qDebug()<<"+****3";
            mapSend.insert("cmd","loginout");
            send_httpParSet(mapSend);

            sendSucc = true;
        }
    }

    qDebug()<<"+**slot_httpParSet**1";
    if(sendSucc){
        qDebug()<<"一次数据请求成功"<<map.value("cmd").toString();

    }else{
        qDebug()<<"数据发送失败："<<map.value("cmd").toString();
        listResendCmd.append(map);

        if(!timerReSendCmd->isActive()){
            timerReSendCmd->start(500);
        }
    }

    qDebug()<<"+****2";
    if(g_tcpsocket != nullptr){
        g_tcpsocket->disconnectFromHost();
        g_tcpsocket->abort();
    }
}

void CHttpApiDevice::HttpSetDate()
{
    JsonMsg_T info ={"setcurrenttime","request","","012345"};
    if(!strlen(this->sessionId)) {
        qDebug() <<"ssionId error "<<sessionId;
        return ;
    }
    sprintf(info.ssionID, "%s", this->sessionId);

    QJsonObject msgObject;
    QJsonObject dataObj;
    QDateTime dateT = QDateTime::currentDateTime();

    CjsonMakeHttpHead(&msgObject, &info);
    dataObj.insert("utc", dateT.toString("yyyy-MM-ddThh:mm:ssZ"));//设置utc时间，格式："2000-10-10T03:39:44Z"
    msgObject.insert("data", QJsonValue(dataObj));
    SendRequestMsg(msgObject);
}

void CHttpApiDevice::HttpSetOsdParam(int osdTimeEnable){

    JsonMsg_T info ={"setosdparam","request","","012345"};
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
    //登录设备
    this->LoginDevice();

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
    this->LogoutDevice();
}
void CHttpApiDevice::HttpSetMotiondetectParam(int enable){
    //登录设备
    this->LoginDevice();

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
    //登录设备
    loginFlag = false;
    this->LoginDevice();

    timerEventCmd = "getrecordparam";
    timeoutCnt->start(200);

}

void CHttpApiDevice::HttpSetRecordParam(int enable){
    //登录设备
    loginFlag = false;
    this->LoginDevice();

    timerEventCmd = "setrecordparam";
    timeoutCnt->start(200);
    dataEnableParam = enable;
}

void CHttpApiDevice::slot_msgEvent() {
    timeoutCnt->stop();

    if(loginFlag == true) {
        if(timerEventCmd == "setosdparam") {
            JsonMsg_T info ={"setosdparam","request","","012345"};
            if(!strlen(this->sessionId)) {
                qDebug() <<"ssionId error "<<sessionId;
                return ;
            }
            sprintf(info.ssionID, "%s", this->sessionId);

            QJsonObject msgObject;
            QJsonObject dataObj, timeObj;

            CjsonMakeHttpHead(&msgObject, &info);

            timeObj.insert("enabled", dataEnableParam);
            dataObj.insert("time", QJsonValue(timeObj));
            msgObject.insert("data", QJsonValue(dataObj));
            SendRequestMsg(msgObject);
        }else if(timerEventCmd == "getosdparam") {
            JsonMsg_T info ={"getosdparam","request","","012345"};
            if(!strlen(this->sessionId)) {
                qDebug() <<"ssionId error "<<sessionId;
                return ;
            }
            sprintf(info.ssionID, "%s", this->sessionId);
            QJsonObject msgObject;
            CjsonMakeHttpHead(&msgObject, &info);
            SendRequestMsg(msgObject);
        }else if(timerEventCmd == "getrecordparam") {
            JsonMsg_T info ={"getrecordparam","request","","012345"};
            if(!strlen(this->sessionId)) {
                qDebug() <<"ssionId error "<<sessionId;
                return ;
            }
            sprintf(info.ssionID, "%s", this->sessionId);
            QJsonObject msgObject;
            CjsonMakeHttpHead(&msgObject, &info);
            SendRequestMsg(msgObject);
        } else if(timerEventCmd == "setrecordparam") {
            JsonMsg_T info ={"setrecordparam","request","","012345"};
            if(!strlen(this->sessionId)) {
                qDebug() <<"ssionId error "<<sessionId;
                return ;
            }
            sprintf(info.ssionID, "%s", this->sessionId);

            QJsonObject msgObject;
            QJsonObject dataObj, timeObj;

            CjsonMakeHttpHead(&msgObject, &info);
            timeObj.insert("alldayenabled", dataEnableParam);
            dataObj.insert("time", QJsonValue(timeObj));
            msgObject.insert("data", QJsonValue(dataObj));
            SendRequestMsg(msgObject);
        }
        //设备登出
        this->LogoutDevice();
    } else {
        timeoutCnt->start(100);
    }

}

