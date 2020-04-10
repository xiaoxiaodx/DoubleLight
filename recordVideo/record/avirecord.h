#ifndef AVIRECORD_H
#define AVIRECORD_H

#include <QObject>
#include "avformat.h"
#include <QDebug>
#include <QDateTime>
#include "common1.h"
#include "ffmpegconvert.h"

class AviRecord : public QObject
{
    Q_OBJECT
public:
    explicit AviRecord(QString did);

   ~AviRecord();
signals:

    void signal_startSucc(bool isSucc);
    void signal_endSucc(bool isSucc);
public slots:
    void slot_writeAudio(char *buff,int len,long long tempTime);
    void slot_writeVedio(char *buff,int len,long long tempTime);
    void slot_endRecord();
    void slot_startRecord(QString did,long long pts,int w,int h);
    void slot_setAviSavePath(QString str);

    void slot_readyRecord(QString path);
    void slot_writeImage(QString path,QImage img,int capx,int capy,int capw,int caph);
private:
    int RecSetAVParam(MeidaVideo_T mediaInfo,void* pWriterHandle);
    int InitWriterHanle(void** pWriterHandle,MeidaVideo_T mediaInfo,char* fileName, char *idxName);
    int RecGetVideoAttr(MeidaVideo_T mediaInfo,VIDEO_PARAM_S*  bitMapInfo);



    void *pwriteHandle;
    QString mDid;
    qint64 startTime;
    QString mRecordingFilePath;
    FfmpegConvert *ffmpegConvert = nullptr;


    bool isWriteSucc = false;

    QString curFileAbsolutePath = "";
    int mPeriodIndex = 0;
    int mPeriod = 40;

    int curResW = 0;
    int curResH = 0;

};

#endif // AVIRECORD_H
