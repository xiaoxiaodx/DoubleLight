#include "toolUtil.h"

#include <QFile>
#include <QDebug>

ToolUtil::ToolUtil(QObject *parent) : QObject(parent)
{
    m_Process.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_Process,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_consoleOutput()));
}

void ToolUtil::funStartCmd(QString keystr)
{

    QString cmd = "act_dv300_common.exe "+keystr+" "+keystr+" 5d7073359bb502922d0b0566b8d8a6e1 883a63d3af6569f8997b625bd1162966f92139a2";

    /*act_dv300_common.exe 76b627633d6beb09edeae06a7952b2a2 76b627633d6beb09edeae06a7952b2a2 5d7073359bb502922d0b0566b8d8a6eb 883a63d3af6569f8997b625bd1162966f92139af*/

    m_Process.start(cmd);
}

void ToolUtil::slot_consoleOutput()
{

    QByteArray qbt = m_Process.readAllStandardOutput();
    QString msg = QString::fromLocal8Bit(qbt);

    qDebug()<<"slot_consoleOutput "<<msg;
    QFile file("lisence.key");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream  in(&file);
        in<<msg<<endl;
        file.close();
    }

    emit signal_sendLisence(msg);
}

void ToolUtil::readDidFile(QString path)
{

    QFile file(path);

    if(file.open(QFile::ReadOnly | QFile::Text)){

        QTextStream in(&file);
        while (!in.atEnd()) {
            myDidList.append(in.readLine());
        }
        file.close();
    }else
        return;

    setWriteDidLabel();
}


void ToolUtil::setWriteDidLabel()
{

    if(myDidList.size() <= 0){

       emit signal_setDidInfo("","","");
    }else {

        QString str = myDidList.takeFirst();

        QStringList writeDidInfo = str.split("\t");

        if(writeDidInfo.size() != 3){
            qDebug()<<"数据异常";
            return;
        }
        qDebug()<<"writeDidInfo "<<writeDidInfo;

        QString did = writeDidInfo.at(0);
        QString key = writeDidInfo.at(1).split(",").at(1);
        QString pushKey = writeDidInfo.at(2).split("#").at(1);


         emit signal_setDidInfo(did,key,pushKey);
    }
}
