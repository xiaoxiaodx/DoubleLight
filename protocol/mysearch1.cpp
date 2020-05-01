#include "mysearch1.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#define SEARCH_HOSTADDR ("239.255.255.250")
#define SEARCH_PORT (3808)
#define SOCKET_RECEIVE_BUFFER (1024*1024*8)
#include <debuglog.h>
MySearch1::MySearch1(QObject *parent) : QObject(parent)
{
    connect(&timer,&QTimer::timeout,this,&MySearch1::slot_timeout);
}

MySearch1::~MySearch1()
{
    qDebug()<<" MySearch1 析构";
   // s_searchsocket->abort();
}
void MySearch1::createSearch()
{
    qDebug()<<"create ip search ";
    if(s_searchsocket == nullptr){
        s_searchsocket = new QUdpSocket(this);//udp
        connect(s_searchsocket,SIGNAL(readyRead()),this,SLOT(readResultMsg()));
//        if( !s_searchsocket->bind(SEARCH_PORT, QUdpSocket::ReuseAddressHint) ) {
//            qDebug()<<"bind ********** !"<<s_searchsocket->state();
//        }else{
//            qDebug()<<"bind 成功" ;
//        }
    }
    timer.start(1000);
}
void MySearch1::slot_timeout()
{
    sendSearch();
}

//初始化搜索
void MySearch1::startSearch()
{
    qDebug()<<"startSearch ***";
    if(s_searchsocket == nullptr){

        s_searchsocket = new QUdpSocket(this);//udp
        if(s_searchsocket->state()!=s_searchsocket->BoundState) {
             qDebug()<<"bind ********** !"<<s_searchsocket->state();

            if( !s_searchsocket->bind(SEARCH_PORT, QUdpSocket::ReuseAddressHint) ) {
                qDebug()<<"bind ********** !"<<s_searchsocket->state();
            }else{
                qDebug()<<"bind 成功" ;
            }
        }else {
            qDebug()<<"socket state failed , UDP search initialization error !"<<endl;
        }
        //qDebug()<<"startSearch ***2";
        if(1) { //s_searchsocket->joinMulticastGroup(QHostAddress(SEARCH_HOSTADDR))
           // s_searchsocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
            //设置缓冲区
            s_searchsocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, SOCKET_RECEIVE_BUFFER);
            s_searchsocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 2);
            //连接接收信号槽
            connect(s_searchsocket,SIGNAL(readyRead()),this,SLOT(readResultMsg()));
            sendSearch();
        } else {
            qDebug()<<s_searchsocket->error();
            qDebug()<<"join multicast Group failed , UDP search initialization errro !"<<endl;
        }
    }
}


void MySearch1::resetSearch()
{
    if(s_searchsocket != nullptr)
    {
        disconnect(s_searchsocket,SIGNAL(readyRead()),this,SLOT(readResultMsg()));
        s_searchsocket->abort();
        s_searchsocket->close();
        s_searchsocket= nullptr;
    }
    startSearch();
}
void MySearch1::forceFinishSearch()
{
    if(s_searchsocket != nullptr)
    {
        disconnect(s_searchsocket,SIGNAL(readyRead()),this,SLOT(readResultMsg()));
        s_searchsocket->abort();
        s_searchsocket->close();
        s_searchsocket= nullptr;
    }
}

//读取搜索回复数据
void MySearch1::readResultMsg()
{
    char bodyData[1024]={0};
    char msg[2048]={0};
    memset(&bodyData,0,sizeof(bodyData));
    memset(&msg,0,sizeof(msg));

    //QByteArray tmpdata=s_searchsocket->readAll();
    s_searchsocket->readDatagram(msg,2048);


    qDebug()<<" readSearchResultMsg   ************";


    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(msg, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("data")) {  // 包含指定的 key
                QJsonValue value = object.value("data");
                if ( value.isObject() ) {  // Page 的 value 是对象
                    QJsonObject obj = value.toObject();
                    if (obj.contains("ip")) {
                        QJsonValue value = obj.value("ip");
                        if (value.isString()) {
                            QString ip = value.toString();
                            qDebug() << "ip : " << ip;

                            DebugLog::getInstance()->writeLog("ip:"+ip);

                            timer.stop();
                            emit signal_sendIp(ip);
                        }
                    }
                }else {
                    qDebug()<<"value not object ";
                }
            } else {
                qDebug()<<"not find data ";
            }
        } else {
            qDebug()<<"not is document !";
        }
    }else {
        qDebug()<<"parse error ";
    }
    return ;
}


void MySearch1::sendSearch()
{
    QJsonObject dataObject;
    dataObject.insert("devtype", "G7");
    dataObject.insert("protocolver", "ver1.0");

    QJsonObject simp_ayjson;
    simp_ayjson.insert("cmd", "devsearch");
    simp_ayjson.insert("method", "request");
    simp_ayjson.insert("msgid", "0123456");
    simp_ayjson.insert("data", QJsonValue(dataObject));


    QJsonDocument document;
    document.setObject(simp_ayjson);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    qDebug() <<"byrearray:"<<byteArray;

    if( !s_searchsocket->writeDatagram(byteArray.data(),byteArray.length(), QHostAddress(SEARCH_HOSTADDR), SEARCH_PORT) ) {
        qDebug()<<"write search msg error !";
    }
}
