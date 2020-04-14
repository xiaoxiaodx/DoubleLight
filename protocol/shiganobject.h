#ifndef SHIGANOBJECT_H
#define SHIGANOBJECT_H

#include <QObject>
#include <QMutex>
#include "shigan.h"
#include <QTimer>
class ShiGanObject : public QObject
{
    Q_OBJECT
public:
    explicit ShiGanObject(QObject *parent = nullptr);
    void forceFinishThread();
signals:

public slots:
    void slot_loopRec();
    void sendHeart();
private:
    QMutex mMutex;
    bool isFinish = false;

    ShiGan shiGan;
    QTimer timer;
};

#endif // SHIGANOBJECT_H
