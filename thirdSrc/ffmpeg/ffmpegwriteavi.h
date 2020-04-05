#ifndef FFMPEGWRITEAVI_H
#define FFMPEGWRITEAVI_H

#include <QObject>
extern "C"{
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
}


class FfmpegWriteAvi : public QObject
{
    Q_OBJECT
public:
    explicit FfmpegWriteAvi(QObject *parent = nullptr);

signals:

public slots:
    void slot_readyWriteAvi(QString filePath);
    void slot_writeAvi(QImage *img,int capx,int capy,int capw,int caph);
    void slot_endWriteAvi();
private:
    AVStream *add_vidio_stream(AVFormatContext *oc, enum AVCodecID codec_id);
    void initScreenRecord();
    void rgb2h264(QImage *imgrgb);
    AVPacket pkt;
    AVFormatContext *ofmt_ctx = NULL;

    AVStream *out_stream;
};

#endif // FFMPEGWRITEAVI_H
