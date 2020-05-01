#ifndef CUPCONTROL_H
#define CUPCONTROL_H

#include <QObject>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QNetworkAccessManager>

class CUpControl : public QObject
{
    Q_OBJECT
public:
    explicit CUpControl(QObject *parent = 0);
    ~CUpControl();
    void startsendFile(QString url,QString file);

signals:
    void signalFinishied(int);
    void signalError();
    void signalProgressUpdate(qint64 ,qint64);

public slots:

   void replyFinished();
   void loadError(QNetworkReply::NetworkError);
   void loadProgress(qint64 ,qint64);
private:
      QNetworkAccessManager *accessManager1 ;
      QNetworkReply *m_reply;

};

#endif // CUPCONTROL_H
