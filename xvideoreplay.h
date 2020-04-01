#ifndef XVIDEOREPLAY_H
#define XVIDEOREPLAY_H

#include <QObject>
#include <QQuickPaintedItem>
class XVideoReplay : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit XVideoReplay();

signals:

protected:
     QSGNode* updatePaintNode(QSGNode *old, UpdatePaintNodeData *);
public slots:

private:
     QThread *pThreadFfmpeg;


};

#endif // XVIDEOREPLAY_H
