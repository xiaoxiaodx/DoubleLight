#ifndef J07DEVICE_H
#define J07DEVICE_H

#include <QObject>
#include "tcpworker.h"
#include "debuglog.h"

class J07Device : public QObject
{
    Q_OBJECT
public:
    explicit J07Device(QString vip="10.67.1.180",int type = -1,QObject *parent = nullptr);
    ~J07Device();

    void startRec();
signals:
    //tcp
    void signal_connentSer(QString ip,int port);
    void signal_disconnentSer();
    void signal_tcpSendAuthentication(QString did,QString name,QString pwd);
    void signal_destoryTcpWork();

public slots:
    void slot_recH264(char *buff,int len,quint64 time,int resw,int resh);
    void slot_recImg(QImage *img,int len,quint64 time,int resw,int resh);
private:
    QString m_ip ="";
    void createTcpThread();
    //void createFFmpegDecodec();
    //FfmpegCodec *pffmpegCodec = nullptr;
    QThread *m_readThread = nullptr;
    TcpWorker *worker = nullptr;
    char *rgbBuff = nullptr;
    int m_type = -1;
};

#endif // J07DEVICE_H
