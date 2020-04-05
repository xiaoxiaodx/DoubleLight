#include "ffmpegconvert.h"
#include <debuglog.h>
#include <QDebug>
FfmpegConvert::FfmpegConvert(QObject *parent) : QObject(parent)
{

    initConvert();
    //    fileSaveYuv = new QFile("F:/work/doubleLight/avi/testAvi.yuv");
    //    if(fileSaveYuv->open(QIODevice::WriteOnly)){
    //        qDebug()<<" 读取yuv数据的文件已经打开    ";
    //    }
}
void FfmpegConvert::unInitConvert()
{
    // 释放RGB缓冲区
    if(rgbBuf != nullptr)
        delete rgbBuf;

    //关闭编码器
    avcodec_close(codecCtx);

    //清理编码器上下文
    avcodec_free_context(&codecCtx);

    //清理视频重采样上下文
    sws_freeContext(swsCtx);
}
void FfmpegConvert::initConvert()
{

    int width = 960;
    int height = 600;
    // 注册所有和编解码器有关的组件
    av_register_all();

    // 创建RGB缓冲区同时分配内存

    try {
        rgbBuf = new unsigned char[width*height * 4];
        // 其它代码
    } catch ( const std::bad_alloc& e ) {
        DebugLog::getInstance()->writeLog("ffmppeg rgb32分配内存失败");
        rgbBuf = nullptr;
    }



    /* 2 创建编码器 */
    // 查找编码器
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec)
    {
        qDebug() << "avcodec_find_encoder AV_CODEC_ID_H264 failed!" ;

    }
    // 给编码器分配内存，返回对应编码器上下文
    codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx)
    {
        qDebug() << "avcodec_alloc_context3  failed!" << endl;

    }

    // 配置编码器上下文的成员
    codecCtx->width = width; // 设置编码视频宽度
    codecCtx->height = height; // 设置编码视频高度
    codecCtx->time_base.num = 1;
    codecCtx->time_base.den = 25; // 设置帧率，num为分子，den为分母，如果是1/25则表示25帧/s
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P; // 设置输出像素格式
    codecCtx->gop_size=250;
    codecCtx->bit_rate = 400000;

    // 打开编码器
    int ret = avcodec_open2(codecCtx, codec, NULL);
    if (ret < 0)
    {
        qDebug() << "avcodec_open2  failed!" << endl;

    }
    qDebug() << "avcodec_open2 success!" << endl;

    // 3 创建视频重采样上下文：指定源和目标图像分辨率、格式
    swsCtx = NULL;

    swsCtx = sws_getContext(
                width, height, AV_PIX_FMT_BGRA,
                width, height, AV_PIX_FMT_YUV420P,
                SWS_BICUBIC,
                NULL, NULL, NULL
                );

    //4 创建YUV视频帧并配置
    yuvFrame = av_frame_alloc();
    yuvFrame->format = AV_PIX_FMT_YUV420P;
    yuvFrame->width = width;
    yuvFrame->height = height;
    ret = av_frame_get_buffer(yuvFrame, 32);
    if (ret < 0)
    {
        qDebug() << "av_frame_get_buffer  failed!" << endl;

    }

    av_opt_set(codecCtx->priv_data, "tune", "zerolatency", 0);
    av_init_packet(&pkt);
}


void FfmpegConvert::rgbToYuv(QImage img,int resW,int resH,QFile &yuvFlie)
{
//    QMutexLocker locker(&mutex);
//    try {

//        rgbBuf = new unsigned char[2304000];
//        // 其它代码
//    } catch ( const std::bad_alloc& e ) {
//        DebugLog::getInstance()->writeLog("ffmppeg rgb32分配内存失败");
//        rgbBuf = nullptr;
//    }

//    if(rgbBuf == nullptr)
//        return;

//    return;
    memcpy(rgbBuf,img.bits(),img.byteCount());

    //5 创建RGB视频帧并绑定RGB缓冲区（avpicture_fill是给rgbFrame初始化一些字段，并且会自动填充data和linesize）
    rgbFrame = av_frame_alloc();
    avpicture_fill((AVPicture *)rgbFrame, rgbBuf, AV_PIX_FMT_BGRA, resW, resH);

    //qDebug()<<" rgbFrame :"<<rgbFrame->linesize[0]<<"   "<<rgbFrame->linesize[1]<<" "<<rgbFrame->linesize[2];

    //7 像素格式转换，转换后的YUV数据存放在yuvFrame
    int outSliceH = sws_scale(swsCtx,  (const uint8_t* const*)rgbFrame->data, rgbFrame->linesize, 0, 600,
                              yuvFrame->data, yuvFrame->linesize
                              );
    if (outSliceH <= 0)
        qDebug()<<"outSliceH fail";

    qDebug()<<"***width***:"<<yuvFrame->linesize[0]<<yuvFrame->linesize[1]<<yuvFrame->linesize[2];
    int wh = 960*600;
    yuvFlie.write((char*)yuvFrame->data[0],wh);
    yuvFlie.write((char*)yuvFrame->data[1],wh/4);
    yuvFlie.write((char*)yuvFrame->data[2],wh/4);



}

int pts = 0;
int count = 0;
void FfmpegConvert::rgb32ToH264(QImage img,QByteArray &arr,bool &gotpic)
{
    memcpy(rgbBuf,img.bits(),img.byteCount());
    int ret = 0;
    //5 创建RGB视频帧并绑定RGB缓冲区（avpicture_fill是给rgbFrame初始化一些字段，并且会自动填充data和linesize）
    rgbFrame = av_frame_alloc();
    avpicture_fill((AVPicture *)rgbFrame, rgbBuf, AV_PIX_FMT_BGRA, 960, 600);

    //qDebug()<<" rgbFrame :"<<rgbFrame->linesize[0]<<"   "<<rgbFrame->linesize[1]<<" "<<rgbFrame->linesize[2];

    //7 像素格式转换，转换后的YUV数据存放在yuvFrame
    int outSliceH = sws_scale(swsCtx,  (const uint8_t* const*)rgbFrame->data, rgbFrame->linesize, 0, 600,
                              yuvFrame->data, yuvFrame->linesize
                              );
    if (outSliceH <= 0)
        qDebug()<<"outSliceH fail";


    /* 8 H264编码 */
    // 将未压缩的AVFrame数据(yuv)给编码器
    yuvFrame->pts = count++ * (codecCtx->time_base.num * 1000 / codecCtx->time_base.den);

    yuvFrame->pts = (count++) * (codecCtx->time_base.den)  / (codecCtx->time_base.num *25);

    ret = avcodec_send_frame(codecCtx, yuvFrame);
    if (ret != 0)
    {
        qDebug()<<"*-****************** fail 1";
        return;
    }
    // 将编码数据保存在AVPacket
    ret = avcodec_receive_packet(codecCtx, &pkt);
    if (ret != 0){
        qDebug()<<"*-****************** fail 2  "<<ret;
        return;
    }

    qDebug()<<"获取一幅图:"<<pkt.size;
    QFile file1("testAvi.h264");
    if(file1.open(QIODevice::WriteOnly | QIODevice::Append)){

        int len =file1.write((char*)pkt.data,pkt.size);

        qDebug()<<"write  h264 succ :"<<len;
    }else{
        qDebug()<<"file open fail:";
    }


    gotpic = true;
    arr.append((char*)pkt.data,pkt.size);

    //av_free_packet(&pkt);
    //9 写入H264文件
    //fwrite(pkt.data, 1, pkt.size, fpout);
    //av_packet_unref(&pkt);

}
