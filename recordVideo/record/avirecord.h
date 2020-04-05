#ifndef AVIRECORD_H
#define AVIRECORD_H

#include <QObject>
#include "avformat.h"
#include <QDebug>
#include <QDateTime>
#include "common.h"
#include "ffmpegconvert.h"

class AviRecord : public QObject
{
    Q_OBJECT
public:
    explicit AviRecord(QString did);
    bool isStart = false;
    bool isWriteSucc = false;
   ~AviRecord();
signals:

    void signal_startSucc(bool isSucc);
    void signal_endSucc(bool isSucc);
public slots:
    void slot_writeAudio(char *buff,int len,long long tempTime);
    void slot_writeVedio(char *buff,int len,long long tempTime);
    void slot_endRecord();
    void slot_startRecord(QString did,long long pts);
    void slot_setAviSavePath(QString str);

    void slot_readyRecord(QString path);
    void slot_writeImage(QImage img,int capx,int capy,int capw,int caph);
private:
    int RecSetAVParam(MeidaVideo_T mediaInfo,void* pWriterHandle);
    int InitWriterHanle(void** pWriterHandle,MeidaVideo_T mediaInfo,char* fileName, char *idxName);
    int RecGetVideoAttr(MeidaVideo_T mediaInfo,VIDEO_PARAM_S*  bitMapInfo);



    void *pwriteHandle;
    QString mDid;
    qint64 startTime;
    QString mRecordingFilePath;
    FfmpegConvert ffmpegConvert;

};

#endif // AVIRECORD_H
