#ifndef REPLAYDATESEARCH_H
#define REPLAYDATESEARCH_H

#include <QObject>
#include <QTime>
enum IntervalType{
    VIDEOLOSS = 0,
    VIDEONORMAL,
    VIDEOALARM,
    TIMELINE24H=10,
    TIMELINE2H,
    TIMELINE1H,
    TIMELINE30M,
};

#define RECORD_LENGHT (14)//录像文件名长度 如：1212121800.avi，开始时间12：12：12 结束时间12：42：12
#define IMAGE_LENGHT (10)//png图片名长度
#define RECORD_FORMAT ("avi")
#define IMAGE_FORMAT ("png")
#define RECORD_TIME (30*60)//30分钟一个录像文件

class TimeInterval{
public:
    IntervalType type;
    QTime startTime;
    QTime endTime;
    int sec;
};

typedef struct _RECORDTIME_T {
    unsigned int startTime;
    unsigned int endTime;
    int type;
}RECORDTIME_T;

class ReplayDateSearch : public QObject
{
    Q_OBJECT
public:
    explicit ReplayDateSearch(QObject *parent = nullptr);
    ~ReplayDateSearch();
signals:






private:
    int getTimeByRecord(TimeInterval *requestinterval, QList<TimeInterval> &respondIntervalList);
    int GetRecordFilePath(QTime &reqTime, TimeInterval &resInterval);

    int FindRecordFile(const QString &strFilePath);
    QString filePath;
    QList<TimeInterval> replayTimeList;
};

#endif // REPLAYDATESEARCH_H
