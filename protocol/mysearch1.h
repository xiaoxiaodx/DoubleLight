#ifndef MySearch1_H
#define MySearch1_H

#include <QObject>
#include <QUdpSocket>

#include <QTimer>
class MySearch1 : public QObject
{
    Q_OBJECT
public:
    explicit MySearch1(QObject *parent = nullptr);
    ~MySearch1();

    void createSearch();
signals:
    void signal_sendIp(QString ip,QString ver);
public slots:
    void startSearch();
    void forceFinishSearch();
    void sendSearch();
    void resetSearch();
    void readResultMsg();//udp数据回调

    void slot_timeout();
private:
    QUdpSocket *s_searchsocket = nullptr;//udp搜索socket
    QTimer timer;
};

#endif // MySearch1_H
