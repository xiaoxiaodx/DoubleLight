#ifndef XVIDEOREPLAY_H
#define XVIDEOREPLAY_H

#include <QObject>
#include <QQuickItem>
#include "render/texturenode.h"
#include "render/renderthread.h"
#include <QQuickWindow>
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



};

#endif // XVIDEOREPLAY_H
