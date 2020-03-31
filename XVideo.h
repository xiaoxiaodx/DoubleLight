
#ifndef XVideo_H
#define XVideo_H
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

#include "avirecord.h"
#include "chttpapidevice.h"


class XVideo : public QQuickPaintedItem
{
    Q_OBJECT
public:
    Q_INVOKABLE void funScreenShot();
    Q_INVOKABLE void funSetShotScrennFilePath(QString str);
    Q_INVOKABLE void funSetRecordingFilePath(QString str);

    Q_INVOKABLE void startNormalVideo();
    Q_INVOKABLE void startTemperatureVideo();

    Q_INVOKABLE void    fun_timeSwitch(bool isChecked);
    Q_INVOKABLE void    fun_warnSwith(bool mchecked);
    Q_INVOKABLE void    fun_screenShotSwith(bool mchecked);
    Q_INVOKABLE void    fun_beerSwith(bool mchecked);
    Q_INVOKABLE void    fun_recordSwith(bool mchecked);
    Q_INVOKABLE void    fun_temSet(QVariant mvalue);
    Q_INVOKABLE void    fun_screenShotPathSet(QVariant mvalue);
    Q_INVOKABLE void    fun_recordPathSet(QVariant mvalue);
    Q_INVOKABLE void    fun_temDrift(QVariant mvalue);

    Q_INVOKABLE void fun_getInitPar();
    Q_INVOKABLE void fun_getListRect(QVariant);


    Q_INVOKABLE void fun_temMax(QVariant mvalue);
    Q_INVOKABLE void fun_temMin(QVariant mvalue);
    Q_INVOKABLE void fun_temOffset(QVariant mvalue);
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
    void signal_update();
    //搜索的信号
    void signal_startSearch();
    void signal_forceFinishSearch();
    void signal_sendSearch();
    void signal_resetSearch();
    void signal_finishSearch();
    //yousee的信号
    void signal_startinit();
    void signal_stop();
    void signal_tempPar(QVariant map);
    void signal_sendListRect(QVariant map);
    //record
    void signal_recordAudio(char *buff,int len,long long tempTime);
    void signal_recordVedio(char *buff,int len,long long tempTime);
    void signal_startRecord(QString did,long long tempTime);
    void signal_endRecord();
    void signal_setRecordingFilePath(QString str);
    //
    void signal_temp(float tempV);
    //私有流参数
    void signal_httpParSet(QMap<QString,QVariant> map);
    void signal_httpUiParSet(QVariant map);
    void signal_getInitPar();
    //红外参数


public slots:
    void slot_recH264(char *buff,int len,quint64 time);
    void slot_recPcmALaw(char *buff,int len,quint64 time);
    void slog_HttpmsgCb(QMap<QString,QVariant>);
    void slot_timeout();

    //void ready();

    void recSearchIp(QString ip);
    void finishYouPull();
protected:
    //  QSGNode* updatePaintNode(QSGNode *old, UpdatePaintNodeData *);
    void paint(QPainter *painter);
private:

    void createSearchIp();
    void createTcpThread();
    void createFFmpegDecodec();
    void createYouseePull();
    void createAviRecord();
    void createHttpApi();
    void initVariable();

    QList<ImageInfo> listImgInfo;

    FfmpegCodec *pffmpegCodec = nullptr;

    QTimer timerUpdate;

    ImageInfo m_Imginfo;

    bool isImgUpdate;

    int minBuffLen = 15;

    bool isPlayAudio;
    bool isRecord;
    bool isStartRecord;//是否启动录像
    bool isScreenShot;
    bool isAudioFirstPlay;
    bool isFirstData;


    quint64 preAudioTime;


    QByteArray audioArr;
    yuvInfo yuvData;
    QList<yuvInfo> listYuv;
    QString mTcpIp;
    int mTcpPort;
    QString mDid;
    QString mAccount;
    QString mPassword;

    QString mshotScreenFilePath;

    RenderThread *m_renderThread{nullptr};

    MySearch1 *psearch = nullptr;
    QThread *searchThread = nullptr;
    QString m_ip = "";

    QThread *youseeThread = nullptr;
    YouSeeParse *mYouSeeParse = nullptr;

    QThread *recordThread = nullptr;
    AviRecord *aviRecord = nullptr;

    QThread *httpThread = nullptr;
    CHttpApiDevice *httpDevice = nullptr;

    QThread *m_readThread = nullptr;
    TcpWorker *worker = nullptr;

    static QVariantList listRectInfo;
};

#endif // XVideo_H
