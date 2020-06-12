#include "xvideoreplay1.h"

XVideoReplay1::XVideoReplay1()
{

}



void XVideoReplay1::paint(QPainter *painter)
{


//    if(pRenderImginfo.pImg == nullptr)
//        return;

//    QFont font("Microsoft Yahei", 20);
//    QPen pen(QBrush(QColor(0,255,0)),1);
//    painter->setPen(pen);
//    painter->setFont(font);

//    //    qDebug()<<""<<tempImgResW;


//    painter->drawImage(QRect(0,0,width(),height()), *pRenderImginfo.pImg);


    return;
}


FfmpegCodec * XVideoReplay1::createFFmpegDecodec(QString type)
{

    FfmpegCodec  *pffmpegCodec = new FfmpegCodec;
    if(type.compare("h264")==0)
        pffmpegCodec->vNakedStreamDecodeInit(AV_CODEC_ID_H264);
    else if(type.compare("h265")==0)
        pffmpegCodec->vNakedStreamDecodeInit(AV_CODEC_ID_H265);

    pffmpegCodec->aNakedStreamDecodeInit(AV_CODEC_ID_PCM_ALAW,AV_SAMPLE_FMT_S16,8000,1);
    pffmpegCodec->resetSample(AV_CH_LAYOUT_MONO,AV_CH_LAYOUT_MONO,8000,44100,AV_SAMPLE_FMT_S16,AV_SAMPLE_FMT_S16,160);


    return pffmpegCodec;
}

void XVideoReplay1::recH264(char* h264Arr,int arrlen)
{

    //qDebug()<<"slot_recH264 ";
    if(pffmpegCodecH264 == nullptr)
       pffmpegCodecH264 = createFFmpegDecodec("h264");


    if(pffmpegCodecH264 != nullptr){

        QImage *Img = nullptr;

            Img = pffmpegCodecH264->decodeVFrame((unsigned char*)h264Arr,arrlen);
            if (Img != nullptr )
            {
//                mMutex.lock();
//                if(listBuffImg.size() < maxBuffLen)
//                    listBuffImg.append(Img);
//                else
//                    delete Img;
//                mMutex.unlock();
            }else{

                qDebug()<<"图片为空";
            }

    }
}
