#ifndef OPENCVWRITEAVI_H
#define OPENCVWRITEAVI_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include <QBuffer>
#include <QImage>

class OpencvWriteAvi : public QObject
{
    Q_OBJECT
public:
    explicit OpencvWriteAvi(QObject *parent = nullptr);

signals:

public slots:
    void slot_readyWriteAvi(QString filePath);
    void slot_writeAvi(QImage *img,int capx,int capy,int capw,int caph);
    void slot_endWriteAvi();
private:
    cv::Mat ImageToMat(QImage img,QString imgFormat);

    void initScreenRecord();
    bool g_needstop =false;

};

#endif // OPENCVWRITEAVI_H
