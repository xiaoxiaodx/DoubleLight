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
void DebugLog::writeLog(QString str)
{

    if(file == NULL){

        file = new QFile("activexDebug.log");
    }

    if(file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){

        qDebug()<<" writeLog    "<<str;
        QTextStream out(file);
        out << str << "\n";
        file->close();
    }
}

