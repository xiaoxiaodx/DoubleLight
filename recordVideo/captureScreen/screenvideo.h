#ifndef SCREENVIDEO_H
#define SCREENVIDEO_H

#include <QObject>
#include "QQuickItem"
#include "writeh264.h"
#include <QThread>
#include "recordVideo/record/avirecord.h"
class ScreenVideo : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE void funCreateScreenThread(QString capturePath,QString recordPath,int period);
    Q_INVOKABLE void funScreenRecrod(QString path,QQuickWindow *quic,int x,int y,int w,int h);
    Q_INVOKABLE bool funScreenShoot(QString path,QQuickWindow *quic,int capx,int capy,int capw,int caph,float warnTemp);
    Q_INVOKABLE void funSetRecordPar(QString path,int period);
    Q_INVOKABLE void funSetScreenCapturePar(QString path);
    Q_INVOKABLE void funForfinishRecord();
    Q_INVOKABLE void funCreateAviRecordThread(QString recordPath,int period);

    Q_INVOKABLE QString funGetCurPath();
    Q_INVOKABLE QString funIsExitCurCapturePath(QString path);
    Q_INVOKABLE QString funIsExitCurRecordPath(QString path);
    Q_INVOKABLE QString funGetFileName(QString path);
    Q_INVOKABLE bool funIsExitFile(QString path);
public:
    explicit ScreenVideo(QObject *parent = nullptr);

signals:
    void signal_writeH264(QString path,QImage img,int capx,int capy,int capw,int caph);
    void signal_screenShot(QImage img,int capx,int capy,int capw,int caph,float warnTemp);
    void signal_screenCapturePar(QString path);
    void signal_recordPar(QString path,int period);
    void signal_forceFinishRecord();
private:
    void createWriteThread();
    QThread *writeThread= nullptr;
    WriteH264 *writeH264 = nullptr;

    QThread *aviThread= nullptr;
    AviRecord *aviRecord = nullptr;
    QString writepath;
};

#endif // SCREENVIDEO_H
