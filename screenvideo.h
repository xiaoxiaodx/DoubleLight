#ifndef SCREENVIDEO_H
#define SCREENVIDEO_H

#include <QObject>
#include <QPixmap>
#include "QQuickItem"


#include <QTimer>
#include "ffmpegwriteavi.h"
#include "opencvwriteavi.h"
#include <QThread>
#include "avirecord.h"

class ScreenVideo : public QObject
{
    Q_OBJECT
public:

    Q_INVOKABLE bool funStartScreenRecrod(QString str);
    Q_INVOKABLE void funScreenRecrod(QQuickWindow *quic,int x,int y,int w,int h);
    Q_INVOKABLE bool funEndScreenRecrod();
    Q_INVOKABLE bool funScreenShoot(QString path,QQuickWindow *quic,int x,int y,int w,int h);
    Q_INVOKABLE QString funGetCurPaht();
public:
    explicit ScreenVideo(QObject *parent = nullptr);

signals:
    void signal_readyWriteAvi(QString filePath);
    void signal_writeAvi(QImage img,int capx,int capy,int capw,int caph);
    void signal_endWriteAvi();


public slots:
    void slot_startSucc(bool isSucc);
    void slot_endSucc(bool isSucc);
private:

    void createOpencvW();



    QThread *writeAviThread= nullptr;
    AviRecord *writeAvi = nullptr;

    FfmpegConvert convern;

    int index = 1;
    QEventLoop loop;
    bool isStartSucc = false;
    bool isEndSucc = false;

};

#endif // SCREENVIDEO_H
