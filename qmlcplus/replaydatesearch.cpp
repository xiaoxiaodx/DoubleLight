#include "replaydatesearch.h"
#include <QDir>
#include <QDebug>
ReplayDateSearch::ReplayDateSearch(QObject *parent) : QObject(parent)
{
   // this->filePath = path;
}

ReplayDateSearch::~ReplayDateSearch()
{
    this->filePath.clear();
    replayTimeList.clear();
}


int ReplayDateSearch::FindRecordFile(const QString &strFilePath) {
    QDir fileDir;
    fileDir.setPath(strFilePath);
    fileDir.setFilter(QDir::Files);//设置过滤，接收文件 //Dirs

    //QFileInfoList fileInfoList = fileDir.entryInfoList();//fileInfoList.at(i).filePath();取绝对路径+文件名
    QStringList fileNameList = fileDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    //qDebug()<<"length:"<<fileNameList.size();
    if(fileNameList.size() <= 0 ) {//文件夹目录为空
        return -1;
    }
    replayTimeList.clear();

    QString fileType;
    RECORDTIME_T tmpRecordTime;

    TimeInterval tmpTimeInterval;
    memset(&tmpRecordTime, 0, sizeof(RECORDTIME_T));
    for (int i = 0; i < fileNameList.size(); i++) {
        fileType = fileNameList.at(i);
        QString tmpTime = fileType.split(".").at(0);
        //long long int fileLengthType = tmpTime.toLongLong();
        //avi格式文件//录像文件长度//录像文件名全为数字，暂定
        if( (RECORD_FORMAT != fileType.split(".").at(fileType.split(".").size()-1)) || \
                (RECORD_LENGHT != fileNameList.at(i).size() ) || tmpTime.toLongLong() == 0 ||\
                (tmpTime.mid(6,4).toInt() <= 0 || tmpTime.mid(6,4).toInt()>RECORD_TIME) ) {
            //qDebug()<<", start time:"<<tmpTime.mid(0,2)<<tmpTime.mid(2,2)<<tmpTime.mid(4,2)<<"("<<(RECORD_FORMAT != fileType.split(".").at(fileType.split(".").size()-1)) <<"),("<< \
                      (RECORD_LENGHT != fileNameList.at(i).size() ) <<","<< (tmpTime.toLongLong() == 0) <<"),("<<\
                      (tmpTime.mid(6,4).toInt() <= 0) <<","<< (tmpTime.mid(6,4).toInt()>RECORD_TIME)<<")";
            continue;
        }
        tmpTimeInterval.startTime.setHMS(tmpTime.mid(0,2).toInt(),tmpTime.mid(2,2).toInt(),tmpTime.mid(4,2).toInt());
        tmpTimeInterval.endTime = tmpTimeInterval.startTime.addSecs(tmpTime.mid(6,4).toInt());//文件名后4位表示时长
        if(tmpTimeInterval.startTime.hour() == 23 && tmpTimeInterval.endTime.hour() == 0) {
            tmpTimeInterval.endTime.setHMS(23,59,59);
        }
        tmpTimeInterval.type = TIMELINE30M;
        tmpTimeInterval.sec = tmpTime.mid(6,4).toInt();
        replayTimeList.append(tmpTimeInterval);
        //        //最大值86399+30*60：23点59分59+30分钟也属于当天的录像文件
        //        tmpRecordTime.startTime = tmpTime.mid(0,2).toInt()*60*60+tmpTime.mid(2,2).toInt()*60+tmpTime.mid(4,2).toInt();
        //        tmpRecordTime.endTime = tmpRecordTime.startTime+tmpTime.mid(6,3).toInt();//结束时间比开始时间多半小时
        //        qDebug()<<fileType<<",start time:"<<tmpRecordTime.startTime<<"end time:"<<tmpRecordTime.endTime;
        //        list.append(tmpRecordTime);
    }
    fileDir.cleanPath(strFilePath);
    fileNameList.clear();

//    qDebug()<<"lsit length:"<<replayTimeList.size();
//    for(int i = 0; i < replayTimeList.size(); i++) {
//        TimeInterval testTime = replayTimeList.at(i);
//        qDebug()<<"找到录像文件："<<i<<", start time:"<<testTime.startTime.toString("yy:MM:dd hh:mm:ss")<<", end time:"<<testTime.endTime.toString("hh:mm:ss")<<", sec:"<<testTime.sec;
//    }
    return 0;
}


//参数：
//      requestinterval:的起始时间必须是x:00:00,分钟、秒钟多为0
int ReplayDateSearch::getTimeByRecord(TimeInterval *requestinterval, QList<TimeInterval> &respondIntervalList) {
    //得到所有 半个小时录像
    if( (this->filePath.isEmpty()) || -1 == FindRecordFile(this->filePath) || replayTimeList.size() == 0 || (requestinterval->endTime.hour() == requestinterval->startTime.hour() && requestinterval->endTime.minute() == requestinterval->startTime.minute())) {
        return -1;
    }
    if(respondIntervalList.size() != 0) {
        respondIntervalList.clear();
    }

    TimeInterval tmpItem;
    memset(&tmpItem, 0, sizeof(tmpItem));

    qDebug()<<"录像区间:("<<requestinterval->startTime.toString("hh:mm:ss")<<", "<<requestinterval->endTime.toString("hh:mm:ss")<<")";
    //interval的时间数据只有小时有参数，分钟和秒钟必须都是0
    //list1：包含       (01:40 ----------------------------------------- 03:10)
    //list2：左、中、右  (01:40 ---- 02:10) (2:10 ---- 02:40) (02:40 ---- 03:10)
    //区间（interval）      02:00 -------------------------------- 03:00
    for(int i = 0; i < replayTimeList.size(); i++) {
        //1.在区间的左边 （list开始的小时<区间开始的小时 && （list的结束的小时>区间开始的小时 && （list结束的小时<区间结束的小时 || （list结束的小时==区间结束的小时&&list结束的分钟<区间结束的分钟））））
        if  (replayTimeList.at(i).startTime.hour() < requestinterval->startTime.hour() && \
             (replayTimeList.at(i).endTime.hour() >= requestinterval->startTime.hour() && replayTimeList.at(i).endTime.hour() <= requestinterval->endTime.hour())) {
            //左边区间只有list的结束时间的秒钟刚好卡边上，去掉
            if (replayTimeList.at(i).endTime.hour()==requestinterval->startTime.hour() && replayTimeList.at(i).endTime.minute()==requestinterval->endTime.minute() && (replayTimeList.at(i).endTime.second()-requestinterval->endTime.second() == 0) ){
                continue;
            }
            //qDebug()<<"000 replayTimeList start time:"<<replayTimeList.at(i).startTime.toString("yy:MM:dd hh:mm:ss")<<", end time:"<<replayTimeList.at(i).endTime.toString("hh:mm:ss");
            //qDebug()<<"000 interval start time:"<<interval->startTime.toString("yy:MM:dd hh:mm:ss")<<", end time:"<<interval->endTime.toString("hh:mm:ss");
            tmpItem.startTime = requestinterval->startTime;
            tmpItem.endTime = replayTimeList.at(i).endTime;
            tmpItem.type = replayTimeList.at(i).type;
            //tmpItem.sec = replayTimeList.at(i).sec;
        //2. 在区间段的中间
        } else if (replayTimeList.at(i).startTime.hour() >= requestinterval->startTime.hour() && replayTimeList.at(i).endTime.hour() <= requestinterval->endTime.hour() ) {//&& replayTimeList.at(i).endTime.hour() != interval->endTime.hour()
            //qDebug()<<"List:("<<replayTimeList.at(i).startTime.toString("yy:MM:dd hh:mm:ss")<<", "<<replayTimeList.at(i).endTime.toString("hh:mm:ss")<<"), tmp:("<<tmpItem.startTime.toString("yy:MM:dd hh:mm:ss")<<", "<<tmpItem.endTime.toString("hh:mm:ss")<<")";
            if ( (replayTimeList.at(i).startTime.hour()>requestinterval->endTime.hour() || (replayTimeList.at(i).startTime.hour()==requestinterval->endTime.hour() && (replayTimeList.at(i).startTime.minute()>requestinterval->endTime.minute()) || (replayTimeList.at(i).startTime.minute()==requestinterval->endTime.minute() && replayTimeList.at(i).startTime.second()>requestinterval->endTime.second())))) {
                continue;
            }
            //上一个结束时间 的小时、分钟、秒钟拼接不上 下一个的开始时间的，这2个不在同一个连续区间里面
            if ( (replayTimeList.at(i).startTime.hour() != tmpItem.endTime.hour()) || \
                    (replayTimeList.at(i).startTime.minute() != tmpItem.endTime.minute()) || \
                    (replayTimeList.at(i).startTime.second() - tmpItem.endTime.second() != 0) ) {
                //tmpItem没有赋值，第一次就在中间区域
                if (!(tmpItem.endTime.hour()==0 && tmpItem.endTime.minute()==0 && tmpItem.endTime.second()==0)) {
                    tmpItem.sec = ((long long)tmpItem.endTime.hour()*3600+tmpItem.endTime.minute()*60+tmpItem.endTime.second()) - ((long long)tmpItem.startTime.hour()*3600+tmpItem.startTime.minute()*60+tmpItem.startTime.second());
                    respondIntervalList.append(tmpItem);
                    memset(&tmpItem, 0, sizeof(tmpItem));
                }
                tmpItem.startTime = replayTimeList.at(i).startTime;
                tmpItem.endTime = replayTimeList.at(i).endTime;
                if(tmpItem.endTime.hour()==requestinterval->endTime.hour() && tmpItem.endTime.minute()>requestinterval->endTime.minute()) {
                    tmpItem.endTime.setHMS(tmpItem.endTime.hour(), requestinterval->endTime.minute(), requestinterval->endTime.second());
                }
                tmpItem.type = replayTimeList.at(i).type;
                //tmpItem.sec = replayTimeList.at(i).sec;
            } else if ( replayTimeList.at(i).endTime.hour()>requestinterval->endTime.hour() || (replayTimeList.at(i).endTime.hour()==requestinterval->endTime.hour() && (replayTimeList.at(i).endTime.minute()>requestinterval->endTime.minute() || (replayTimeList.at(i).endTime.minute()==requestinterval->endTime.minute() && replayTimeList.at(i).endTime.second()>requestinterval->endTime.second()))) ) {
                tmpItem.endTime = requestinterval->endTime;
                //tmpItem.sec += requestinterval->sec;

            } else {
                /*if ( (replayTimeList.at(i).endTime.minute() - interval->endTime.minute() <= 0) || (replayTimeList.at(i).endTime.second()-interval->endTime.second() <= 0) ) {
                    tmpItem.endTime = replayTimeList.at(i).endTime;
                } else {
                    tmpItem.endTime = interval->endTime;
                }*/
                tmpItem.endTime = replayTimeList.at(i).endTime;
                tmpItem.type = requestinterval->type;
                //tmpItem.sec += replayTimeList.at(i).sec;
            }
        //3. 在区间的右边
        } else if ( (replayTimeList.at(i).startTime.hour() <= requestinterval->startTime.hour() && (replayTimeList.at(i).startTime.minute()<requestinterval->endTime.minute())) && ((replayTimeList.at(i).endTime.hour() > requestinterval->endTime.hour())) ){
            //qDebug()<<"333 replayTimeList start time:"<<replayTimeList.at(i).startTime.toString("yy:MM:dd hh:mm:ss")<<", end time:"<<replayTimeList.at(i).endTime.toString("hh:mm:ss");
            //qDebug()<<"333 interval start time:"<<interval->startTime.toString("yy:MM:dd hh:mm:ss")<<", end time:"<<interval->endTime.toString("hh:mm:ss");
            //左边区间只有list的结束时间的秒钟刚好卡边上，去掉
            if (replayTimeList.at(i).startTime.hour()==requestinterval->endTime.hour() && replayTimeList.at(i).startTime.minute()==requestinterval->endTime.minute() && replayTimeList.at(i).startTime.second()==requestinterval->endTime.second()){
                continue;
            }
            //上一个结束时间 的小时、分钟、秒钟拼接不上 下一个的开始时间的，这2个不在同一个连续区间里面
            if ( (replayTimeList.at(i).startTime.hour() != tmpItem.endTime.hour()) || \
                    (replayTimeList.at(i).startTime.minute() != tmpItem.endTime.minute()) || \
                    (replayTimeList.at(i).startTime.second() - tmpItem.endTime.second() != 0) ) {
                //tmpItem没有赋值，第一次就在区间的右边
                if (!(tmpItem.endTime.hour()==0 && tmpItem.endTime.minute()==0 && tmpItem.endTime.second()==0)) {
                    tmpItem.sec = ((long long)tmpItem.endTime.hour()*3600+tmpItem.endTime.minute()*60+tmpItem.endTime.second()) - ((long long)tmpItem.startTime.hour()*3600+tmpItem.startTime.minute()*60+tmpItem.startTime.second());
                    respondIntervalList.append(tmpItem);
                    memset(&tmpItem, 0, sizeof(tmpItem));
                }
                tmpItem.startTime = replayTimeList.at(i).startTime;
                tmpItem.endTime = requestinterval->endTime;
                tmpItem.type = requestinterval->type;
            } else {
                if ( (replayTimeList.at(i).endTime.minute() - requestinterval->endTime.minute() <= 0) || (replayTimeList.at(i).endTime.second()-requestinterval->endTime.second() <= 0) ) {
                    tmpItem.endTime = replayTimeList.at(i).endTime;
                } else {
                    tmpItem.endTime = requestinterval->endTime;
                }
            }
        //4. 把区间包含进去了
        } else if (replayTimeList.at(i).startTime.hour()<requestinterval->startTime.hour() && replayTimeList.at(i).endTime.hour()>requestinterval->endTime.hour()) {
            tmpItem.startTime = requestinterval->startTime;
            tmpItem.endTime = requestinterval->endTime;
            tmpItem.type = requestinterval->type;

            tmpItem.sec = ((long long)tmpItem.endTime.hour()*3600+tmpItem.endTime.minute()*60+tmpItem.endTime.second()) - ((long long)tmpItem.startTime.hour()*3600+tmpItem.startTime.minute()*60+tmpItem.startTime.second());
            respondIntervalList.append(tmpItem);
            memset(&tmpItem, 0, sizeof(tmpItem));
        }
    }
    //最后一次
    if(!(tmpItem.endTime.hour()==0 && tmpItem.endTime.minute()==0 && tmpItem.endTime.second()==0)) {
        tmpItem.sec = ((long long)tmpItem.endTime.hour()*60*60+tmpItem.endTime.minute()*60+tmpItem.endTime.second()) - ((long long)tmpItem.startTime.hour()*3600+tmpItem.startTime.minute()*60+tmpItem.startTime.second());
        respondIntervalList.append(tmpItem);
    }

//    memset(&tmpItem, 0, sizeof(tmpItem));
//    qDebug()<<"区间大小: "<<respondIntervalList.size();
//    for(int i = 0; i < respondIntervalList.size(); i++) {
//        TimeInterval testTime = respondIntervalList.at(i);
//        qDebug()<<"start time:"<<testTime.startTime.toString("yy:MM:dd hh:mm:ss")<<", end time:"<<testTime.endTime.toString("hh:mm:ss");
//    }
    return 0;
}

int ReplayDateSearch::GetRecordFilePath(QTime &reqTime, TimeInterval &resInterval) {
    if((this->filePath.isEmpty()) || -1 == FindRecordFile(this->filePath) || replayTimeList.size() == 0 ) {
        return -1;
    }

    long long tmpStartTimeSec, tmpEndTimeSec,tmpTimeSec;
    //char path[128] = {0};
    //从RECORD_TIME秒的录像中找出文件名
    for(int i = 0 ; i < replayTimeList.size(); i++) {
        tmpStartTimeSec = replayTimeList.at(i).startTime.hour()*60*60 + replayTimeList.at(i).startTime.minute()*60 + replayTimeList.at(i).startTime.second();
        tmpEndTimeSec = replayTimeList.at(i).endTime.hour()*60*60 + replayTimeList.at(i).endTime.minute()*60 + replayTimeList.at(i).endTime.second();
        tmpTimeSec = reqTime.hour()*60*60 + reqTime.minute()*60 + reqTime.second();
        if(tmpTimeSec >= tmpStartTimeSec && tmpTimeSec <= tmpEndTimeSec) {
            //sprintf(path, "%s/%02d%02d%02d%04d.%s",this->filePath,replayTimeList.at(i).startTime.hour(), replayTimeList.at(i).startTime.minute(), replayTimeList.at(i).startTime.second(), (tmpEndTimeSec-tmpStartTimeSec), RECORD_FORMAT);
            //resPath = QString::fromUtf8(path);
            resInterval.startTime = replayTimeList.at(i).startTime;
            resInterval.endTime = replayTimeList.at(i).endTime;
            resInterval.type = replayTimeList.at(i).type;
            resInterval.sec = replayTimeList.at(i).sec;
            break;
        }
    }
    return 0;
}
