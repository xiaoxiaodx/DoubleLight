#ifndef SHIGANOBJECT_H
#define SHIGANOBJECT_H

#include <QObject>
#include <QMutex>
#include "shigan.h"
class ShiGanObject : public QObject
{
    Q_OBJECT
public:
    explicit ShiGanObject(QObject *parent = nullptr);
    void forceFinishThread();
    bool startRec();
signals:

public slots:
    void slot_loopRec();

private:
    QMutex mMutex;
    bool isFinish = false;

    ShiGan shiGan;

};

#endif // SHIGANOBJECT_H
