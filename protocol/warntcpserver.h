#ifndef WARNTCPSERVER_H
#define WARNTCPSERVER_H

#include <QObject>
#include <QVariant>
class QTcpServer;
class QTcpSocket;
class WarnTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit WarnTcpServer(QObject *parent = nullptr);

    void createSer(QString ip,int port);
    void destroySer();
signals:
    void signal_WarnMsg(QMap<QString,QVariant>);
public slots:

    void slot_newConnect();
    void slot_readByte();
private:
    int HttpMsgCallBack(char * pData);
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *cliSocket = nullptr;
    QString parseStr = "";

    QByteArray msgdata;
};

#endif // WARNTCPSERVER_H
