#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <QObject>
#include <QFile>
#include <QMutex>
class DebugLog : public QObject
{
    Q_OBJECT
public:
    explicit DebugLog(QObject *parent = nullptr);


    static DebugLog *Log;
    static DebugLog *getInstance();
    void writeLog(QString str);
signals:

public slots:

private:
     QMutex fileMutex;
    QFile *file = NULL;
};

#endif // DEBUGLOG_H
