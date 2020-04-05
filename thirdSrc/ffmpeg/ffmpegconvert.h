#ifndef FFMPEGCONVERT_H
#define FFMPEGCONVERT_H

#include <QObject>
#include <QImage>
extern "C"{
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
}
#include <QFile>
#include <QMutex>
class FfmpegConvert : public QObject
{
    Q_OBJECT
public:
    explicit FfmpegConvert(QObject *parent = nullptr);
  void rgb32ToH264(QImage img,QByteArray &arr,bool &gotpic);
  void rgbToYuv(QImage img,int resW,int resH,QFile &yuvFlie);
  void initConvert();
  void unInitConvert();
signals:

public slots:

private:
    SwsContext *swsCtx = nullptr;
    AVCodec *codec=nullptr;
    AVCodecContext *codecCtx=nullptr;
    unsigned char *rgbBuf = nullptr;
    QByteArray rgb32Arr;
    AVFrame *yuvFrame = nullptr;
    AVFrame *rgbFrame = nullptr;
    AVPacket pkt;
    QImage testImg;
    QFile *fileSaveYuv;
    QMutex mutex;
};

#endif // FFMPEGCONVERT_H
