#include "consoleprocess.h"

#include <QDebug>
ConsoleProcess::ConsoleProcess(QObject *parent) : QObject(parent)
{
    m_Process.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_Process,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_output()));

}

void ConsoleProcess::funStartCmd(QString str)
{
    m_Process.start(str);
}


void ConsoleProcess::slot_output()
{
    qDebug()<<"dsadsad";
    QByteArray qbt = m_Process.readAllStandardOutput();
    QString msg = QString::fromLocal8Bit(qbt);


    QFile file("lisence.key");

    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream  in(&file);
        in<<msg<<endl;
        file.close();
    }
    qDebug()<<msg;
}
