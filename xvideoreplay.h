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

    Q_INVOKABLE void funStartOpen(QString str);


signals:

protected:
    QSGNode* updatePaintNode(QSGNode *old, UpdatePaintNodeData *);
public slots:

private:
    void ready();


    QThread *pThreadFfmpeg;
    RenderThread *m_renderThread{nullptr};

    QByteArray yuvArr;
    yuvInfo yuvData;
    QList<yuvInfo> listYuv;
    FFmpegReplay freplay;
    QFile *yuvfile;

    QTimer timer;
};

#endif // XVIDEOREPLAY_H
