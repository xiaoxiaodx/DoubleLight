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

public:
    explicit ScreenVideo(QObject *parent = nullptr);

signals:
    void signal_readyWriteAvi(QString filePath);
    void signal_writeAvi(QImage img,int capx,int capy,int capw,int caph);
    void signal_endWriteAvi();


public slots:
private:

    void createOpencvW();



    QThread *writeAviThread= nullptr;
    AviRecord *writeAvi = nullptr;

    FfmpegConvert convern;

    int index = 1;

};

#endif // SCREENVIDEO_H
