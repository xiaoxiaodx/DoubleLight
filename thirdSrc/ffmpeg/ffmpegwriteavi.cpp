#include "ffmpegwriteavi.h"
#include <QDebug>
#include <QQuickWindow>
#include <QBuffer>


FfmpegWriteAvi::FfmpegWriteAvi(QObject *parent) : QObject(parent)
{

}



void FfmpegWriteAvi::slot_readyWriteAvi(QString m_screenRecordPath)
{
    //其包含码流参数较多，是一个贯穿始终的数据结构，很多函数都要用到它作为参数
    const char *out_filename = m_screenRecordPath.toLatin1();//"d:\\out.mp4";//输出文件路径，在这里也可以将mkv改成别的ffmpeg支持的格式，如mp4，flv，avi之类的
    int ret;//返回标志
    av_register_all();//初始化解码器和复用器
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);//初始化一个用于输出的AVFormatContext结构体，视频帧率和宽高在此函数里面设置
    if (!ofmt_ctx)
    {
        printf("Could not create output context\n");
        return;
    }

    out_stream = add_vidio_stream(ofmt_ctx, AV_CODEC_ID_MJPEG);//创造输出视频流
    //out_stream = add_vidio_stream(ofmt_ctx, AV_CODEC_ID_PNG);//创造输出视频流(第三个参数指向图片的格式)
    av_dump_format(ofmt_ctx, 0, out_filename, 1);//该函数会打印出视频流的信息，如果看着不开心可以不要

    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE))//打开输出视频文件
    {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            printf("Could not open output file '%s'", out_filename);
            return;
        }
    }

    if (avformat_write_header(ofmt_ctx, NULL) < 0)//写文件头（Write file header）
    {
        printf("Error occurred when opening output file\n");
        return;
    }

    //int frame_index = 0;//放入视频的图像计数
    //unsigned char *mydata = new unsigned char[DATASIZE];



}



void FfmpegWriteAvi::slot_writeAvi(QImage *img,int capx,int capy,int capw,int caph)
{

    if(img->isNull() || !img)
        return;
    QImage img1 = img->copy(QRect(capx,capy,capw,caph));
    delete img;


    QImage img2 = img1.scaled(QSize(960,600),Qt::IgnoreAspectRatio);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img2.save(&buffer, "MJPEG");

    //qDebug()<<"****1";
    unsigned char *buff = new unsigned char[ba.length()];
    memcpy(buff,ba.data(),ba.length());

    av_init_packet(&pkt);
    pkt.flags |= AV_PKT_FLAG_KEY;
    pkt.stream_index = out_stream->index;//获取视频信息，为压入帧图像做准备

    pkt.size = ba.length();
    pkt.data = buff;

    if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0) //写入图像到视频
    {
        qDebug()<<("Error muxing packet\n");
    }
     //qDebug()<<"23213";
    buffer.close();

}

void FfmpegWriteAvi::slot_endWriteAvi()
{
    qDebug()<<"funEndScreenRecrod   0";
    av_free_packet(&pkt);//释放掉帧数据包对象
    av_write_trailer(ofmt_ctx);//写文件尾（Write file trailer）

    if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx->pb);//关闭视频文件
    avformat_free_context(ofmt_ctx);//释放输出视频相关数据结构
}


#define AV_CODEC_FLAG_GLOBAL_HEADER (1 << 22)
#define CODEC_FLAG_GLOBAL_HEADER AV_CODEC_FLAG_GLOBAL_HEADER
AVStream *FfmpegWriteAvi::add_vidio_stream(AVFormatContext *oc, enum AVCodecID codec_id)//用以初始化一个用于输出的AVFormatContext结构体
{
    AVStream *st;
    AVCodec *codec;

    st = avformat_new_stream(oc, NULL);
    if (!st)
    {
        printf("Could not alloc stream\n");
        exit(1);
    }
    codec = avcodec_find_encoder(codec_id);
    if (!codec)
    {
        printf("codec not found\n");
        exit(1);
    }
    avcodec_get_context_defaults3(st->codec, codec);//申请AVStream->codec(AVCodecContext对象)空间并设置默认值(由avcodec_get_context_defaults3()设置

    st->codec->bit_rate = 40000;//设置采样参数，即比特率
    st->codec->width = 960;//设置视频宽高，这里跟图片的宽高保存一致即可
    st->codec->height = 600;
    st->codec->time_base.den = 1;//设置帧率
    st->codec->time_base.num = 1;

    st->codec->pix_fmt = AV_PIX_FMT_RGB32;//设置像素格式
    st->codec->codec_tag = 0;
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)//一些格式需要视频流数据头分开
         st->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    return st;
}


