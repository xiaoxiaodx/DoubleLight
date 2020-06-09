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
  void rgb32ToH264(QImage img,int w,int h,QFile &file);
  void rgb32ToH264(QImage img,QByteArray &arr,bool &gotPic);
  void rgbToYuv(QImage img,int resW,int resH,QFile &yuvFlie);
  bool initConvert(int resW,int resH);
  void unInitConvert();

  QImage* yuv420ToRgb32(char* pbuff_in,int nwidth,int nheight);
  QImage* yuv422ToRgb32(char* pbuff_in,int nwidth,int nheight);

  void initYuv420ToRgb32(int nwidth,int nheight);
  void initYuv422ToRgb32(int nwidth,int nheight);
  void yuv420ToRgb32release();
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



    AVFrame    *m_pFrameRGB,*m_pFrameYUV;
    uint8_t *m_rgbBuffer,*m_yuvBuffer;
    struct SwsContext *m_img_convert_ctx;
};

#endif // FFMPEGCONVERT_H
