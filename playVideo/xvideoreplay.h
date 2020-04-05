#ifndef XVIDEOREPLAY_H
#define XVIDEOREPLAY_H

#include <QObject>
#include <QQuickItem>
#include "render/texturenode.h"
#include "render/renderthread.h"
#include <QQuickWindow>
#include "ffmpegreplay.h"
#include <QFile>
class XVideoReplay : public QQuickItem
{
    Q_OBJECT

public:
    explicit XVideoReplay();

//    Q_INVOKABLE void funPlay(QString str);
//    Q_INVOKABLE void funPause();
//    Q_INVOKABLE void funPlayVideoTimeChange(int pix);


signals:

protected:
    QSGNode* updatePaintNode(QSGNode *old, UpdatePaintNodeData *);
public slots:
    void ready();
private:
    QThread *pThreadFfmpeg;
    RenderThread *m_renderThread{nullptr};

    QByteArray yuvArr;

    /*播放的视频信息，键值对形式存在
        "videobuff":QByteArray buff   //视频数据buff,yuv格式
        "videoStart":(int)       //视频开始时间:时分秒 hhmmss
        "totalTime:":(int)      //视频总长
    */
    QVariant playMap;
    yuvInfo yuvData;
    QList<yuvInfo> listYuv;
    FFmpegReplay freplay;
    QFile *yuvfile;

    QTimer timer;
};

#endif // XVIDEOREPLAY_H
