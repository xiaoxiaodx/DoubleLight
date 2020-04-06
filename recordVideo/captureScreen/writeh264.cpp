#include "writeh264.h"
#include <QDateTime>
#include <debuglog.h>
#include <QDir>
#include <QTextStream>
WriteH264::WriteH264(QString capturePath,QString recordPath,int period)
{
    mRecordPath = recordPath;
    mCaptureScreenPath =capturePath;
    mPeriod = period;
}

void WriteH264::slot_recordPar(QString path,int period)
{
    mRecordPath = path;
    mPeriod = period;
}

void WriteH264::slot_screenCapturePar(QString path)
{
    mCaptureScreenPath = path;
}
void WriteH264::slot_writeH264(QImage img,int capx,int capy,int capw,int caph){

    if(mRecordPath == ""){
        DebugLog::getInstance()->writeLog("record video file path is null");
        return;
    }

    QDate date = QDateTime::currentDateTime().date();
    QString desFileDir = mRecordPath+"/"+date.toString("yyyyMMdd");

    QDir dir;
    if (!dir.exists(desFileDir)){
        bool res = dir.mkpath(desFileDir);
        if(res)
            DebugLog::getInstance()->writeLog("slot_writeH264 create new dir is succ");
        else
            DebugLog::getInstance()->writeLog("slot_writeH264 create new dir is fail");
    }



    QTime time = QDateTime::currentDateTime().time();
    QString filename = time.toString("hhmmss")+".yuv";


    if(isReadyRecord){
        //创建相对路径
        if(QDir::setCurrent(desFileDir))
            DebugLog::getInstance()->writeLog("slot_writeH264 set relative dir is false");
        file.setFileName(filename);
        isReadyRecord = false;
    }

    if(!file.isOpen()){
        if(!file.open(QIODevice::WriteOnly)){
            DebugLog::getInstance()->writeLog("record video file open fail:"+filename);
            return;
        }
    }

    QImage img1 = img.copy( capx, capy, capw, caph);
    QImage img2 = img1.scaled(QSize(960,600),Qt::IgnoreAspectRatio);

    if(mPeriodIndex*mPeriod < 1800000){
        convern.rgbToYuv(img2,960,600,file);
        mPeriodIndex++;
    }else {
        //超过30分钟，关闭文件，这样在继续录文件时会重新打开一个文件
        closeAndSaveFile();
    }
}

void WriteH264::closeAndSaveFile()
{
    QString curFileName = file.fileName();
    int index = curFileName.indexOf(".h264");
    int timelong = mPeriodIndex*mPeriod;

    QString newName = curFileName.insert(index,QString::number(timelong));
    DebugLog::getInstance()->writeLog("****:"+QString::number(timelong));
    file.rename(newName);
    mPeriodIndex = 0;
    file.close();
    isReadyRecord = true;

}

void WriteH264::slot_screenShot(QImage img,int capx,int capy,int capw,int caph,float warnTemp)
{
    if(mCaptureScreenPath == ""){
        DebugLog::getInstance()->writeLog("scrennshot file path is null");
        return;
    }

    QImage img1 = img.copy( capx, capy, capw, caph);
    QImage img2 = img1.scaled(QSize(960,600),Qt::IgnoreAspectRatio);

    QDateTime curDateTime =  QDateTime::currentDateTime();
    QString  curDatetimeStr = curDateTime.toString("yyMMdd_hhmmss");

    QString imgAbsolutePath = mCaptureScreenPath+"image/"+curDatetimeStr+".png";
    if(!img.save(imgAbsolutePath,"PNG")){
        DebugLog::getInstance()->writeLog("scrennshot save fail");
    }else{
        //存报警图片信息
        /*  告警名录下 建立一个日志文件夹 一个图片文件夹，
            日志文件夹下放30个日志文件，一个日志文件代表一天，
            图片文件夹下放入告警抓拍图片
        */
        QString warnLogAbsolutePath = mCaptureScreenPath + "log/"+curDateTime.date().toString("yyyyMMdd")+".log";
//        QFile imgInfofile(absolutePath);
//        if(imgInfofile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
//            QString imgInfoStr = absolutePath+" "+QString::number(warnTemp)+" "+imgInfofile.fileName();
//            QTextStream out(&file);
//            out <<imgInfoStr << "\n";
//            imgInfofile.close();
//        }

    }
}

void WriteH264::slot_forceFinishRecord()
{
    mRecordPath = "";
    if(file.isOpen()){
        closeAndSaveFile();
    }
}

WriteH264::~WriteH264()
{
    closeAndSaveFile();
    convern.unInitConvert();
}
