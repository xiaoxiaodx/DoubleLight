#include "avirecord.h"
#include <QDir>
AviRecord::AviRecord(QString did)
{

    mDid = did;
    pwriteHandle = nullptr;
    mRecordingFilePath = "";


}

void AviRecord::slot_writeImage(QImage img,int capx,int capy,int capw,int caph)
{

    if(!isRecording)
        return;
    QImage img1 = img.copy(QRect(capx,capy,capw,caph));
    QImage img2 = img1.scaled(QSize(960,600),Qt::IgnoreAspectRatio);


    QByteArray arr;
    arr.clear();
    bool isGotPic = false;
    ffmpegConvert.rgb32ToH264(img2,arr,isGotPic);


    qDebug()<<"h264 录像数据:"<<arr.length();
    if(isGotPic)
        slot_writeVedio(arr.data(),arr.length(),1000);

}

void AviRecord::slot_readyRecord(QString path){

    if(mRecordingFilePath.compare(path) != 0)
        mRecordingFilePath = path;


    slot_startRecord(path,1000);
}

void AviRecord::slot_writeAudio(char* buff,int len,long long tempTime)
{



    if(pwriteHandle == nullptr)
        return;

    qint64 tmpPts = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qint64  u64Pts = (tmpPts  - startTime)/1000;

    int s32Ret = writer_writeframe(pwriteHandle, 1, u64Pts,
                                   (unsigned char*)buff, len, 1);
    if(s32Ret != KEY_TRUE)
    {
        DF_DEBUG("writer_writeframe audio failed %s \n");

    }
}

void AviRecord::slot_writeVedio(char* buff,int len,long long tempTime)
{

    //qDebug()<<"write avi vedio";
    if(pwriteHandle == nullptr)
        return;

    qint64 tmpPts = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qint64  u64Pts = (tmpPts  - startTime)/1000;

    // qDebug()<<"writer_writeframe vedio";
    int s32Ret = writer_writeframe(pwriteHandle, 0, u64Pts,(unsigned char*)buff, len, 1);
    qDebug()<<"write avi vedio  "<<s32Ret;
    if(s32Ret != KEY_TRUE)
    {
        DF_DEBUG("writer_writeframe vedio failed %s \n");
    }
}

void AviRecord::slot_startRecord(QString did,long long pts)
{


    if(pwriteHandle != nullptr)
        return;
    QDir dir;


    QString desFileDir = mRecordingFilePath;


    if (!dir.exists(desFileDir))
    {
        bool res = dir.mkpath(desFileDir);
        qDebug() << "新建最终目录是否成功:" << res;
    }


     startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    QString filename = desFileDir+ "/"+mDid+"_"+QString::number(startTime)+".avi";


    QByteArray tmpArr = filename.toLatin1();

    qDebug()<<"filename"<<"   "<<tmpArr.data();

    int s32Ret = KEY_FALSE;
    s32Ret = writer_create(&pwriteHandle,tmpArr.data());

    //free(tmpCh);
    if(s32Ret != KEY_TRUE)
    {
        qDebug()<<"file writer create failed "<<s32Ret;
        return;
    }

    int NoUseVar = 0;
    int* ps32VideoIndex = &NoUseVar;
    int* ps32AudioIndex = &NoUseVar;

    VIDEO_PARAM_S  bitMapInfo = {0};

    bitMapInfo.eCodeid = CODEC_TYPE_H264;//编码格式
    bitMapInfo.u32BiWidth = 960;
    bitMapInfo.u32BiHeight = 600;
    bitMapInfo.u32BitCount = 16;
    bitMapInfo.u32samplePerSec = 15;
    bitMapInfo.u32avgBitRate = 1024;
    bitMapInfo.u32IsVBR = 1;
    bitMapInfo.u32NTSC  = 0;
    s32Ret = writer_setvideoparam(pwriteHandle, ps32VideoIndex, &bitMapInfo);

    AUDIO_PARAM_S  wavp = {0};
    wavp.eCodeid = CODEC_TYPE_G711A;
    wavp.u16BitsPerSample = 16;//位宽，需根据真实的值来填写
    wavp.u16BlockAlign = 1;
    wavp.u32Channels = 1;  //声道数，需根据真实的值来填写,目前为单声道，先写死
    wavp.u32SamplesPerSec = 8000; //SampleRateEnToU32(struAIAttr.enSampleRate); //采样率
    wavp.u32AvgBytesPerSec = wavp.u32SamplesPerSec * wavp.u16BlockAlign;
    s32Ret = writer_setaudioparam(pwriteHandle, ps32AudioIndex, &wavp);// test by chendan
    if(s32Ret != KEY_TRUE)
    {
        qDebug()<<"writer_setvideoparam failed :"<<s32Ret;
        return;
    }

    QString path = did+".dat";


    s32Ret = writer_begin(pwriteHandle);//打开文件


    if(s32Ret != KEY_TRUE)
    {
        qDebug()<<"open file writer failed ";//<<QString::number(s32Ret);
        return;
    }

    isRecording = true;
}

void AviRecord::slot_setAviSavePath(QString str){

    if(mRecordingFilePath.compare(str)!=0)
        mRecordingFilePath = str;
}

void AviRecord::slot_endRecord()
{

    qDebug()<<"slot_endRecord";
    isRecording = false;
    int s32Ret;
    if(pwriteHandle)
    {
        s32Ret = writer_end(pwriteHandle);
        if(0 != s32Ret)
        {
            DF_DEBUG("writer_end return error, the s32Ret is %d\n", s32Ret);
        }
    }

    if(pwriteHandle)
    {
        s32Ret = writer_destroy(pwriteHandle);
        if(0 != s32Ret)
        {
            DF_DEBUG("writer_destroy return error, the s32Ret is %d\n", s32Ret);
        }
    }

    pwriteHandle = nullptr;
}
//罗勇:
int AviRecord::InitWriterHanle(void** pWriterHandle,MeidaVideo_T mediaInfo,char* fileName, char *idxName) //pWriterHandle是一个ADAPT_S类型的句柄，里面包含写文件的工具的信息
{
    int s32Ret = KEY_FALSE;

    s32Ret = writer_create(pWriterHandle, fileName);
    if(s32Ret != KEY_TRUE)
    {
        qDebug()<<("file writer create failed :%x\n");
        return KEY_FALSE;
    }

    s32Ret = RecSetAVParam(mediaInfo, *pWriterHandle);
    if(KEY_TRUE != s32Ret)
    {
        qDebug()<<("REC_SetAVParam failed :%x\n");
        return KEY_FALSE;
    }

    s32Ret = writer_begin(*pWriterHandle);//打开文件
    if(s32Ret != KEY_TRUE)
    {
        qDebug()<<("open file writer failed :%x\n");
        return KEY_FALSE;
    }

    return s32Ret;
}

//罗勇:

int AviRecord::RecSetAVParam(MeidaVideo_T mediaInfo,void* pWriterHandle)
{
    int s32Ret;
    int NoUseVar = 0;
    int* ps32VideoIndex = &NoUseVar;
    int* ps32AudioIndex = &NoUseVar;

    VIDEO_PARAM_S  bitMapInfo = {0};
    AUDIO_PARAM_S  wavp = {0};

    //qDebug()<<("=====%d %d %d %d\n", mediaInfo.head.mediaType, mediaInfo.info.fps, mediaInfo.info.reslution.width, mediaInfo.info.reslution.height);
    if((mediaInfo.head.mediaType == MediaType_H264) || (mediaInfo.head.mediaType == MediaType_H265) || \
            (mediaInfo.head.mediaType == MediaType_JPEG) || (mediaInfo.head.mediaType == MediaType_MPEG4))
    {
        s32Ret = RecGetVideoAttr(mediaInfo, &bitMapInfo);//pbitMapInfo作为出参
        if(s32Ret != KEY_TRUE)
        {
            qDebug()<<("get video attr failed :%x\n");
            return s32Ret;
        }

        s32Ret = writer_setvideoparam(pWriterHandle, ps32VideoIndex, &bitMapInfo);// test by chendan
        if(s32Ret != KEY_TRUE)
        {
            qDebug()<<("writer_setvideoparam failed :%x\n");
            return KEY_FALSE;
        }
    }

    wavp.eCodeid = CODEC_TYPE_G711A;
    wavp.u16BitsPerSample = 16;//位宽，需根据真实的值来填写
    wavp.u16BlockAlign = 1;
    wavp.u32Channels = 1;  //声道数，需根据真实的值来填写,目前为单声道，先写死
    wavp.u32SamplesPerSec = 8000; //SampleRateEnToU32(struAIAttr.enSampleRate); //采样率
    wavp.u32AvgBytesPerSec = wavp.u32SamplesPerSec * wavp.u16BlockAlign;
    s32Ret = writer_setaudioparam(pWriterHandle, ps32AudioIndex, &wavp);// test by chendan
    if(s32Ret != KEY_TRUE)
    {
        qDebug()<<("writer_setvideoparam failed :%x\n",s32Ret);
        return KEY_FALSE;
    }

    return KEY_TRUE;
}


int AviRecord::RecGetVideoAttr(MeidaVideo_T mediaInfo,VIDEO_PARAM_S*  bitMapInfo)

{
    int s32Ret;

    if(mediaInfo.head.mediaType == MediaType_H264)
    {
        bitMapInfo->eCodeid = CODEC_TYPE_H264;//编码格式
    }
    else if(mediaInfo.head.mediaType == MediaType_MPEG4)
    {
        bitMapInfo->eCodeid = CODEC_TYPE_MJPEG;
    }
    else if(mediaInfo.head.mediaType == MediaType_H265)
    {
        //bitMapInfo->eCodeid = CODEC_TYPE_H265;
    }
    else
    {
        qDebug()<<("invalid video format:%d\n",mediaInfo.head.mediaType);
        return KEY_FALSE;
    }

    bitMapInfo->u32BiWidth = mediaInfo.info.reslution.width;
    bitMapInfo->u32BiHeight = mediaInfo.info.reslution.height;
    bitMapInfo->u32BitCount = 16;
    bitMapInfo->u32samplePerSec = mediaInfo.info.fps;
    bitMapInfo->u32avgBitRate = mediaInfo.info.bitrate;
    bitMapInfo->u32IsVBR = mediaInfo.info.rcmode;
    bitMapInfo->u32NTSC  = 0;
    qDebug()<<" video format:"<<mediaInfo.info.fps;

    return KEY_TRUE;
}


AviRecord::~AviRecord(){

    qDebug()<<"析构   AviRecord";
    slot_endRecord();
}
