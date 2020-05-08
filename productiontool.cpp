#include "productiontool.h"
#include <QDebug>
#include <QFile>
ProductionTool::ProductionTool(QObject *parent) : QObject(parent)
{
    m_Process.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_Process,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_consoleOutput()));
}

void ProductionTool::funStartCmd(QString deviceid)
{

    QString cmd = "act_dv300_common.exe "+deviceid+" "+deviceid+" 5d7073359bb502922d0b0566b8d8a6eb" + " 883a63d3af6569f8997b625bd1162966f92139af";



    QFile file("lisence.key");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream  in(&file);
        in<<cmd<<endl;
        file.close();
    }
    m_Process.start(cmd);
}

void ProductionTool::slot_consoleOutput()
{

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
