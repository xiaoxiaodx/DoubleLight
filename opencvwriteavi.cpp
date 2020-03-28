#include "opencvwriteavi.h"
#include <QDebug>

cv::VideoWriter mvideoWriter("d:\\1.avi",cv::CAP_OPENCV_MJPEG,40,cv::Size(640, 480));
OpencvWriteAvi::OpencvWriteAvi(QObject *parent) : QObject(parent)
{

}

void OpencvWriteAvi::slot_readyWriteAvi(QString m_screenRecordPath)
{

}

cv::Mat OpencvWriteAvi::ImageToMat(QImage img,QString imgFormat)
{
    if(img.isNull())
        return cv::Mat();
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer,imgFormat.toLatin1().data());

    cv::_InputArray arrSrc(ba.data(), ba.size());

    cv::Mat mat = imdecode(arrSrc, CV_LOAD_IMAGE_COLOR);
    return mat;
}


void OpencvWriteAvi::slot_writeAvi(QImage *img,int capx,int capy,int capw,int caph)
{



    if(img->isNull() || !img)
        return;
    QImage img1 = img->copy(QRect(capx,capy,capw,caph));
    delete img;


    QImage img2 = img1.scaled(QSize(960,600),Qt::IgnoreAspectRatio);


    cv::Size frameSize;
    frameSize.width=960;
    frameSize.height=600;
    //cv::VideoWriter mvideoWriter;


//    //VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));
    mvideoWriter.open("d:\\1.avi",cv::CAP_OPENCV_MJPEG,40,frameSize);
    if(mvideoWriter.isOpened()){
        qDebug()<<"打开成功";

    }else{
        qDebug()<<"打开失败`````````````````````";
    }

    mvideoWriter<<ImageToMat(img2,"PNG");
  //  mvideoWriter.write(ImageToMat(img2,"PNG"));



}


void OpencvWriteAvi::slot_endWriteAvi()
{

    mvideoWriter.release();
}
