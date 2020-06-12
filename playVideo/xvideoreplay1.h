#ifndef XVIDEOREPLAY1_H
#define XVIDEOREPLAY1_H

#include <QObject>
#include <QQuickPaintedItem>
#include "debuglog.h"
#include "ffmpegcodec.h"
class XVideoReplay1 : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit XVideoReplay1();

signals:

public slots:


protected:
    void paint(QPainter *painter);
private:
    void recH264(char* h264Arr,int arrlen);
    FfmpegCodec * createFFmpegDecodec(QString type);
    FfmpegCodec *pffmpegCodecH264 = nullptr;
};

#endif // XVIDEOREPLAY1_H
