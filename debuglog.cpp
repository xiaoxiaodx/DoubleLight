#include "debuglog.h"

DebugLog* DebugLog::Log = NULL;

DebugLog::DebugLog(QObject *parent) : QObject(parent)
{

}

DebugLog* DebugLog::getInstance()
{
    if(Log == NULL){
        Log = new DebugLog();
        return Log;
    }else {
        return Log;
    }
}
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QDir>
void DebugLog::writeLog(QString str)
{

    QMutexLocker locker(&fileMutex);
    if(file == NULL){

        QDir dir;
        if (dir.exists("activexDebug.log"))
        {
           QFile::remove("activexDebug.log");
        }


        file = new QFile("activexDebug.log");

    }

    if(file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss   ");
       // qDebug()<<" writeLog    "<<str;
        QTextStream out(file);
        out <<curTime<< str << "\n";
        file->close();
    }
}

