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

class FfmpegConvert : public QObject
{
    Q_OBJECT
public:
    explicit FfmpegConvert(QObject *parent = nullptr);
  void rgb32ToH264(QImage img,QByteArray &arr,bool &gotpic);
  void initConvert();
  void unInitConvert();
signals:

public slots:
private:



    SwsContext *swsCtx = nullptr;
    AVCodec *codec=nullptr;
    AVCodecContext *codecCtx=nullptr;
    unsigned char *rgbBuf = nullptr;
    AVFrame *yuvFrame = nullptr;
    AVFrame *rgbFrame = nullptr;
    AVPacket pkt;
    QImage testImg;
};

#endif // FFMPEGCONVERT_H
