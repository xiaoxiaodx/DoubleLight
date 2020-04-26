#ifndef XVideo_H
#define XVideo_H
#include <QQuickPaintedItem>
#include <QImage>
#include <QList>
#include <QTimer>

#include <QDateTime>
#include <QDir>

#include <QQuickWindow>
#include "tcpworker.h"
#include "ffmpegcodec.h"
#include "mysearch1.h"
#include "youseeparse.h"
#include "chttpapidevice.h"

#include "debuglog.h"
class XVideo : public QQuickPaintedItem
{
    Q_OBJECT
public:

    Q_INVOKABLE void startNormalVideo(float tp);
    Q_INVOKABLE void fun_timeSwitch(bool isChecked);
    Q_INVOKABLE void fun_temSet(QVariant mvalue);
    Q_INVOKABLE void fun_getInitPar();
    Q_INVOKABLE void fun_setListRect(QVariant);
    Q_INVOKABLE void fun_updateDate();
    Q_INVOKABLE void fun_setRectPar(int sx,int sy,int sw,int sh,int pw,int ph);
    Q_INVOKABLE void fun_initRedFrame(int w,int h);
    Q_INVOKABLE void fun_setInitPar(QString ip,int parentW,int parentH,int x,int y,int w,int h);
    Q_INVOKABLE void fun_setIraInfo(QVariantMap map);

    explicit XVideo();
    ~XVideo();

signals:
    //tcp
    void signal_connentSer(QString ip,int port);
    void signal_disconnentSer();
    void signal_tcpSendAuthentication(QString did,QString name,QString pwd);
    void signal_destoryTcpWork();
    //qml
    void signal_loginStatus(QString msg);
    void signal_waitingLoad(QString msgload);
    void signal_endLoad();

    //
    void signal_temp(float tempV);
    //私有流参数
    void signal_httpParSet(QMap<QString,QVariant> map);
    void signal_httpUiParSet(QVariant map);
    void signal_getInitPar();

    //http 搜索
    void signal_resetSearch();
    void signal_finishSearch();

public slots:
    void slot_recH264(char *buff,int len,quint64 time);

    void slog_HttpmsgCb(QMap<QString,QVariant>);
    void recSearchIp(QString ip);
    void slot_tcpConnected();
protected:
    //  QSGNode* updatePaintNode(QSGNode *old, UpdatePaintNodeData *);
    void paint(QPainter *painter);
private:
    void createSearchIp();
    void createTcpThread();
    void createFFmpegDecodec();

    void createHttpApi();

    void updateUi();


    FfmpegCodec *pffmpegCodec = nullptr;

    QMutex buffMutex;
    ImageInfo pRenderImginfo ;

    const int maxBuffLen = 5;
    QList<QImage *> listBuffImg;
    QMutex mMutex;
    QImage *pBuffImg = nullptr;

    bool isFirstData = false;

    MySearch1 *psearch = nullptr;
    QThread *searchThread = nullptr;

   // QString m_ip ="10.67.1.87";
   QString m_ip ="192.168.1.188" ;

    QThread *httpThread = nullptr;
    CHttpApiDevice *httpDevice = nullptr;

    QThread *m_readThread = nullptr;
    TcpWorker *worker = nullptr;

    QVariantList listRectInfo;

    float warnTemp ;

    qreal showRectX = 65;
    qreal showRectY = 41;
    qreal showRectW = 349;
    qreal showRectH = 327;
    qreal showParentW = 494;
    qreal showParentH = 369;

    qreal tempImgResW =0;
    qreal tempImgResH =0;
};

#endif // XVideo_H
