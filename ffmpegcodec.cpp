#include "ffmpegcodec.h"

FfmpegCodec::FfmpegCodec(QObject *parent) : QObject(parent)
{

    initVariable();
    av_register_all();

//    audioSrc = new QFile("playAudioSrc.pcm");
//    if (!audioSrc->open(QIODevice::ReadOnly  |QIODevice::WriteOnly))
//        return;

}


void FfmpegCodec::initVariable()
{

    audioSrc = nullptr;
    m_pVoutBuffer = nullptr;
    m_VstreamIndex = -1;
    m_AstreamIndex = -1;

    m_pAvformatCtx = nullptr;
    //音视频共用
    m_pAVFrame = nullptr;
    //视频
    m_videofirstParse = true;
    m_pVFrameBGR = nullptr;
    m_pVCodecCtx = nullptr;

    m_pVCodec = nullptr;
    m_pImg_convert_ctx = nullptr;

    //音频
    m_pACodec = nullptr;
    m_pACodecCtx = nullptr;



    //控制播放和线程结束
    isFinish = false;
    isPlay = false;
    isPause = false;

}

void FfmpegCodec::aNakedStreamDecodeInit(AVCodecID codecId,AVSampleFormat sample_fmt1, int sample_rate, int channels)
{

    m_pACodec = avcodec_find_decoder(codecId);
    if (!m_pACodec) {
        consoleDebug("Codec not found audio codec id\n");
        return ;
    }


    if(m_pACodecCtx == nullptr){
        m_pACodecCtx = avcodec_alloc_context3(m_pACodec);
        if (!m_pACodecCtx) {
            printf("Could not allocate audio codec context\n");
            return ;
        }
        m_pACodecCtx->sample_fmt = sample_fmt1;
        //m_pACodecCtx->sample_rate = sample_rate;
        m_pACodecCtx->channels = channels;

    }
    if (avcodec_open2(m_pACodecCtx, m_pACodec, NULL) < 0) {
        consoleDebug("Could not open codec\n");
        return ;
    }



    av_init_packet(&m_AVPacket);
    if(m_pAVFrame == nullptr){
        m_pAVFrame = av_frame_alloc();
        if (NULL == m_pAVFrame)
            return ;
    }

    qDebug()<<"音频频裸流解析初始化完成";
}

void FfmpegCodec::vNakedStreamDecodeInit(AVCodecID codecId)
{


    if(m_pVCodec == nullptr){
        m_pVCodec = avcodec_find_decoder(codecId);//找到编解码器类型，AVCodec         *m_pCodec，是存储解码器的信息的结构体

        if (!m_pVCodec)
        {
            consoleDebug("avcodec_find_encoder failed");
            return ;
        }
    }

    if(m_pAVFrame == nullptr){

        m_pAVFrame = av_frame_alloc();
        if (!m_pAVFrame)
        {
            consoleDebug("m_pAVFrame 分配内存失败");
            return ;
        }
    }

    if(m_pVFrameBGR == nullptr){

        m_pVFrameBGR = av_frame_alloc();

        if (!m_pVFrameBGR)
        {
            consoleDebug("m_pVFrameBGR 分配内存失败");


            return ;
        }
    }

    if(m_pVCodecCtx == nullptr){

        m_pVCodecCtx = avcodec_alloc_context3(m_pVCodec);//为m_pCodecCtx分配内存空间，将解码器将解码器信息传入
        if (!m_pVCodecCtx)
        {
            consoleDebug("avcodec_alloc_context3 failed");
            return ;
        }
    }


    if (avcodec_open2(m_pVCodecCtx,m_pVCodec, NULL) < 0)//打开视频解码器 ,将解码器信息传入
    {
        consoleDebug("avcodec_open2 failed");
        return ;
    }

    av_init_packet(&m_AVPacket);

    qDebug()<<"视频裸流解析初始化完成";
}

void FfmpegCodec::decodeVFrame(uchar *inbuff,int inbufflen,uchar *&outbuff,uint &width,uint &height)
{
    m_AVPacket.data = inbuff;
    m_AVPacket.size = inbufflen;

    if( avcodec_send_packet(m_pVCodecCtx,&m_AVPacket) == 0)
    {

        if(0 == avcodec_receive_frame(m_pVCodecCtx,m_pAVFrame))
        {
            //解决：deprecated pixel format used, make sure you did set range correctly
            AVPixelFormat pixFormat;
            switch (m_pVCodecCtx->pix_fmt)
            {
            case AV_PIX_FMT_YUVJ420P:

                pixFormat = AV_PIX_FMT_YUV420P;
                break;
            case AV_PIX_FMT_YUVJ422P:
                qDebug()<<"yuv格式异常:AV_PIX_FMT_YUVJ422P";
                pixFormat = AV_PIX_FMT_YUV422P;
                break;
            case AV_PIX_FMT_YUVJ444P:
                qDebug()<<"yuv格式异常:AV_PIX_FMT_YUVJ444P";
                pixFormat = AV_PIX_FMT_YUV444P;
                break;
            case AV_PIX_FMT_YUVJ440P:
                qDebug()<<"yuv格式异常:AV_PIX_FMT_YUVJ440P";
                pixFormat = AV_PIX_FMT_YUV440P;
                break;
            default:
                pixFormat = m_pVCodecCtx->pix_fmt;
            }

            width = m_pVCodecCtx->width;
            height = m_pVCodecCtx->height;

            uint wh = width * height;

            try
            {
                if(m_videofirstParse){

                    outbuff = new uchar[m_pVCodecCtx->width * m_pVCodecCtx->height * 3 /2];
                }

            }
            catch(std::bad_alloc &memExp)
            {
            // 失败以后，要么 abort 要么重分配
              outbuff = nullptr;
              return;
            }

            m_videofirstParse = false;
            //outbuff = new uchar[m_pVCodecCtx->width * m_pVCodecCtx->height * 3 /2];

            memcpy(outbuff,m_pAVFrame->data[0],wh);
            memcpy(outbuff+wh,m_pAVFrame->data[1],wh/4);
            memcpy(outbuff+wh*5/4,m_pAVFrame->data[2],wh/4);
            m_AVPacket.data = NULL;
            m_AVPacket.size = 0;
        }
    }
}

QImage* FfmpegCodec::decodeVFrame(uint8_t *buff,int bufflen)
{
    m_AVPacket.data = buff;
    m_AVPacket.size = bufflen;

    if( avcodec_send_packet(m_pVCodecCtx,&m_AVPacket) == 0)
    {

        if(0 == avcodec_receive_frame(m_pVCodecCtx,m_pAVFrame))
        {

            //解决：deprecated pixel format used, make sure you did set range correctly
            AVPixelFormat pixFormat;
            switch (m_pVCodecCtx->pix_fmt)
            {
            case AV_PIX_FMT_YUVJ420P:
                pixFormat = AV_PIX_FMT_YUV420P;
                break;
            case AV_PIX_FMT_YUVJ422P:
                pixFormat = AV_PIX_FMT_YUV422P;
                break;
            case AV_PIX_FMT_YUVJ444P:
                pixFormat = AV_PIX_FMT_YUV444P;
                break;
            case AV_PIX_FMT_YUVJ440P:
                pixFormat = AV_PIX_FMT_YUV440P;
                break;
            default:
                pixFormat = m_pVCodecCtx->pix_fmt;
            }
            //


            //qDebug()<<" m_pVCodecCtx->width, m_pVCodecCtx->height   "<<m_pVCodecCtx->width<<m_pVCodecCtx->height;
            if (m_videofirstParse ){
                qDebug()<<"***first_time***";
                m_pImg_convert_ctx = sws_getContext(m_pVCodecCtx->width, m_pVCodecCtx->height, pixFormat, m_pVCodecCtx->width, m_pVCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
                int size = avpicture_get_size(AV_PIX_FMT_RGB32, m_pVCodecCtx->width, m_pVCodecCtx->height);
                m_pVoutBuffer = (uint8_t *)av_malloc(size);
                avpicture_fill((AVPicture *)m_pVFrameBGR, m_pVoutBuffer, AV_PIX_FMT_RGB32, m_pVCodecCtx->width, m_pVCodecCtx->height); // allocator memory for BGR buffer
                m_videofirstParse = false;
                return nullptr;
            }
            else
            {

                sws_scale(m_pImg_convert_ctx, (const uint8_t* const*)m_pAVFrame->data, m_pAVFrame->linesize, 0, m_pVCodecCtx->height, m_pVFrameBGR->data, m_pVFrameBGR->linesize);
                QImage *pImage = nullptr;
                try {

                    pImage = new QImage((uchar*)m_pVoutBuffer, m_pVCodecCtx->width, m_pVCodecCtx->height, QImage::Format_RGB32);

                    // 其它代码
                } catch ( const std::bad_alloc& e ) {

                    qDebug()<<"图片分配内存失败     ";
                    return nullptr;
                }

                return pImage;

            }
        }
    }
    return nullptr;

}

void FfmpegCodec::decodeAFrame(uint8_t *buff,int bufflen,QByteArray &arr)
{

    //    if(audioSrc != nullptr)
    //        audioSrc->write((char*)buff,bufflen);

    m_AVPacket.data = buff;
    m_AVPacket.size = bufflen;


    //确保获取的是一帧数据
    int ret = av_packet_from_data(&m_AVPacket, m_AVPacket.data, m_AVPacket.size);
    if (ret <0)
    {
        printf("av_packet_from_data error \n");
        return ;
    }

    if(0 == avcodec_send_packet(m_pACodecCtx,&m_AVPacket)){

        if(0==avcodec_receive_frame(m_pACodecCtx,m_pAVFrame)){


            double  t = 0;
            /* compute destination number of samples */
            dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, src_rate) +
                                            src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);
            if (dst_nb_samples > max_dst_nb_samples) {
                av_freep(&dst_data[0]);
                ret = av_samples_alloc(dst_data, &dst_linesize, dst_nb_channels,
                                       dst_nb_samples, dst_sample_fmt, 1);
                if (ret < 0){
                    return;
                }
                max_dst_nb_samples = dst_nb_samples;
            }

            /* convert to destination format */
            // 重采样操作
            ret = swr_convert(swr_ctx, dst_data, dst_nb_samples, (const uint8_t **)m_pAVFrame->data, src_nb_samples);
            if (ret < 0) {
                qDebug()<< "Error while converting";
                return;
            }
            dst_bufsize = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,
                                                     ret, dst_sample_fmt, 1);
            //audioSrc->write((char*)dst_data[0],dst_bufsize);
            if (dst_bufsize < 0) {
                qDebug()<< "Could not get sample buffer size";
                return;
            }
            //qDebug()<<"***   "<<dst_bufsize;
            arr.append((char*)dst_data[0],dst_bufsize);
            //qDebug()<<"hex:"<<arr.toHex();



        }

    }
}



void FfmpegCodec::resetSample(int64_t srcCh_layout,int64_t dstCh_layout,int srcSampleRate,int desSampleRate,AVSampleFormat srcSample_fmt,AVSampleFormat dstSample_fmt, int srcNb_samples)
{

    // 设置数据源src和dst声道布局
    this->src_ch_layout = srcCh_layout, dst_ch_layout = dstCh_layout;
    // 设置src和dst采样率
    src_rate = srcSampleRate,dst_rate = desSampleRate;
    src_data = NULL, dst_data = NULL;
    src_nb_channels = 0, dst_nb_channels = 0;
    //src_linesize, dst_linesize;
    src_nb_samples = srcNb_samples, dst_nb_samples, max_dst_nb_samples;
    // 设置src和dst音频格式
    src_sample_fmt = srcSample_fmt, dst_sample_fmt = dstSample_fmt;


    const char *fmt;
    // 重采样上下文，包含resample信息

    int ret;
    swr_ctx = swr_alloc();
    if (!swr_ctx) {
        qDebug()<<"Could not allocate resampler context";
        ret = AVERROR(ENOMEM);
        return;
    }

    /* set options */
    // 将resample信息写入resample上下文
    av_opt_set_int(swr_ctx, "in_channel_layout",    src_ch_layout, 0);
    av_opt_set_int(swr_ctx, "in_sample_rate",       src_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", src_sample_fmt, 0);

    av_opt_set_int(swr_ctx, "out_channel_layout",    dst_ch_layout, 0);
    av_opt_set_int(swr_ctx, "out_sample_rate",       dst_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", dst_sample_fmt, 0);

    /* initialize the resampling context */
    if ((ret = swr_init(swr_ctx)) < 0) {
        qDebug()<<"Failed to initialize the resampling context";
        return;
    }


    src_nb_channels = av_get_channel_layout_nb_channels(src_ch_layout);

    qDebug()<<"src_nb_samples   "<<src_nb_samples<<"    "<<src_rate<<"  "<<dst_rate;
    ret = av_samples_alloc_array_and_samples(&src_data, &src_linesize, src_nb_channels,
                                             src_nb_samples, src_sample_fmt, 0);
    if (ret < 0) {
        qDebug()<<"Could not allocate source samples";
        return;
    }


    max_dst_nb_samples = dst_nb_samples =
            av_rescale_rnd(src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);

    /* buffer is going to be directly written to a rawaudio file, no alignment */
    dst_nb_channels = av_get_channel_layout_nb_channels(dst_ch_layout);
    ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels,
                                             dst_nb_samples, dst_sample_fmt, 0);
    if (ret < 0) {
        qDebug()<<"Could not allocate destination samples";
        return;
    }
}

void FfmpegCodec::finishWork()
{

    QMutexLocker locker(&finishMutex);
    isFinish = true;
}

void FfmpegCodec::consoleDebug(QString str)
{
    qDebug()<<"FfmpegCodec: "<<str;
}

FfmpegCodec::~FfmpegCodec()
{

    qDebug()<<"析构   FfmpegCodec";


    if(m_pAVFrame != nullptr)
        av_frame_free(&m_pAVFrame);

    if(m_pVFrameBGR != nullptr)
        av_frame_free(&m_pVFrameBGR);

    if(m_pACodecCtx != nullptr)
        avcodec_close(m_pACodecCtx);

    if(m_pVCodecCtx != nullptr)
        avcodec_close(m_pVCodecCtx);

    if(m_pAvformatCtx != nullptr)
        avformat_free_context(m_pAvformatCtx);

    if(m_pVoutBuffer != nullptr)
        av_free(m_pVoutBuffer);
     qDebug()<<"析构   FfmpegCodec 结束";
}




