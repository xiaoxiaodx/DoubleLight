#ifndef WRITEH264_H
#define WRITEH264_H

#include <QObject>
#include "ffmpegconvert.h"
#include <QFile>
class WriteH264 : public QObject
{
    Q_OBJECT
public:
    explicit WriteH264(QString capturePath=0,QString recordPath=0,int period=0);
    ~WriteH264();
    void setSavePath(QString path);
signals:

public slots:
    void slot_writeH264(QImage img,int capx,int capy,int capw,int caph);
    void slot_screenShot(QImage img,int capx,int capy,int capw,int caph,float warnTemp);
    void slot_recordPar(QString path,int recordPeriod);
    void slot_screenCapturePar(QString path);
    void slot_forceFinishRecord();
private:
    void closeAndSaveFile();
    FfmpegConvert convern;
    QString mRecordPath;
    QString mCaptureScreenPath;
    QFile file;
    int mPeriodIndex = 0;
    int mPeriod = 0;
    bool isReadyRecord = true;

};

#endif // WRITEH264_H
