#include "upcontrol.h"
#include <QHttpPart>
#include <QHttpMultiPart>
CUpControl::CUpControl(QObject *parent) : QObject(parent)
{
    accessManager1 = new QNetworkAccessManager(this);

}
CUpControl::~CUpControl()
{

}
void CUpControl::startsendFile(QString url,QString filename)
{

    qDebug() << "__________" + url;
    qDebug() << "__________" + filename;
    // QFile* file=new QFile()
    accessManager1->setNetworkAccessible(QNetworkAccessManager::Accessible);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data;name=\"file\";filename=\"gz_upgrade.ov\""));

    QFile *file = new QFile(filename);

    if(file->open(QIODevice::ReadOnly)){
         qDebug()<<"url  startsendFile openfiel succ";
    }else{
        qDebug()<<"url  startsendFile openfiel fail";
    }

    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    // request.setPriority(QNetworkRequest::HighPriority);
    m_reply = accessManager1->post(request,multiPart);
    //   m_reply = accessManager1->post(request, multiPart);
    multiPart->setParent(m_reply);

    connect(m_reply,SIGNAL(finished()),this,SLOT(replyFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
}
void CUpControl::replyFinished()
{
    //  m_reply->abort();
       qDebug() << "__________";
    QVariant statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (m_reply->error() == QNetworkReply::NoError)
    {

    }
    else
    {
        QString strError = m_reply->errorString();
        //  qDebug() << "__________" + strError;
    }

    // m_reply->deleteLater();
    emit signalFinishied(statusCode.toInt());
}
void CUpControl::loadError(QNetworkReply::NetworkError code)
{
    // m_reply->deleteLater();
    QString strError = m_reply->errorString();
    qDebug() << "__________" + strError;

    emit signalError();
}
void CUpControl::loadProgress(qint64 send,qint64 total)
{
    // qDebug() << QString("%1").arg(send * 100 / total + 1);
    emit signalProgressUpdate(send,total);
}
