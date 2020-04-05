#ifndef XVIDEOTEMP_H
#define XVIDEOTEMP_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QList>
#include <QTimer>

#include <QSGSimpleTextureNode>
#include <QDateTime>
#include <QDir>

#include <QQuickWindow>
#include "tcpworker.h"

#include "render/renderthread.h"
#include "ffmpegcodec.h"
#include "mysearch1.h"
#include "youseeparse.h"
#include "chttpapidevice.h"

#include "debuglog.h"
class XVideoTemp : public QQuickPaintedItem
{
    Q_OBJECT
public:

    Q_INVOKABLE void startTemperatureVideo(float warnTemp);

    Q_INVOKABLE void    fun_timeSwitch(bool isChecked);

    Q_INVOKABLE void    fun_temSet(QVariant mvalue);
    Q_INVOKABLE void    fun_temDrift(QVariant mvalue);

    Q_INVOKABLE void fun_getInitPar();

    Q_INVOKABLE void fun_temMax(QVariant mvalue);
    Q_INVOKABLE void fun_temMin(QVariant mvalue);
    Q_INVOKABLE void fun_temOffset(QVariant mvalue);

    explicit XVideoTemp();
    ~XVideoTemp();


    static QMutex buffMutex;
    static ImageInfo *pBufferImginfo;//缓存的图片指针
signals:
    //qml
    void signal_loginStatus(QString str);
    //yousee的信号
    void signal_startinit();
    void signal_stop();
    void signal_areaMaxtemp(QVariant map);
    void signal_sendListRect(QVariant map);
    void signal_getInitPar();

    void signal_temp(float tempV);
    //红外参数
    void signal_parSet(QMap<QString,QVariant> map);

public slots:
    void slot_timeout();
    void slot_setDateTimeout();
    //void ready();

    void finishYouPull();
protected:
    //  QSGNode* updatePaintNode(QSGNode *old, UpdatePaintNodeData *);
    void paint(QPainter *painter);
private:

    void createYouseePull();

    QTimer timerUpdate;

    ImageInfo *pRenderImginfo = nullptr;//渲染使用的图片指针



    bool isFirstData = false;

    yuvInfo yuvData;
    QList<yuvInfo> listYuv;



    QThread *youseeThread = nullptr;
    YouSeeParse *mYouSeeParse = nullptr;


    float warnTemp;
};


#endif // XVIDEOTEMP_H
