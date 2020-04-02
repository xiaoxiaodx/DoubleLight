#ifndef FFMPEGREPLAY_H
#define FFMPEGREPLAY_H


extern "C"{
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
}

#include <QObject>
#include <QDebug>
#include <QString>
#include <QImage>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QFile>

#include <QObject>



class FFmpegReplay : public QObject
{
    Q_OBJECT
public:
    explicit FFmpegReplay(QObject *parent = nullptr);


    ~FFmpegReplay();

public slots:
    void slot_openFile(QString path);

private:
    void initVariable();

    void consoleDebug(QString str);

    AVPacket m_AVPacket;
    AVFrame *m_pAVFrame;
    AVFormatContext *m_pAvformatCtx;

    //视频
    AVCodecContext  *m_pVCodecCtx;
    AVCodec         *m_pVCodec;
    uint8_t *m_pVoutBuffer;
    AVFrame *m_pVFrameBGR;
    SwsContext *m_pImg_convert_ctx ;

    //音频
    AVCodec         *m_pACodec;
    AVCodecContext  *m_pACodecCtx;

    //音频重采样
    // 设置数据源src和dst声道布局
    int64_t src_ch_layout , dst_ch_layout;
    // 设置src和dst采样率
    int src_rate , dst_rate ;
    uint8_t **src_data , **dst_data ;
    int src_nb_channels, dst_nb_channels;
    int src_linesize, dst_linesize;
    int src_nb_samples , dst_nb_samples, max_dst_nb_samples;
    // 设置src和dst音频格式
    enum AVSampleFormat src_sample_fmt , dst_sample_fmt ;
    int dst_bufsize;
    struct SwrContext *swr_ctx;


    QMutex finishMutex;
    QMutex pauseMutex;
    int m_VstreamIndex;
    int m_AstreamIndex;



    bool isPlay;

    bool isPause;



};

#endif // FFMPEGREPLAY_H
