#include "productiontool.h"
#include <QDebug>
#include <QFile>
ProductionTool::ProductionTool(QObject *parent) : QObject(parent)
{
    m_Process.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_Process,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_consoleOutput()));
}

void ProductionTool::funStartCmd(QString str)
{
    m_Process.start(str);
}

void ProductionTool::slot_consoleOutput()
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

void ProductionTool::readDidFile(QString path)
{

}
