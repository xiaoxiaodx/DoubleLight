#include "toolutil.h"

#include <QFile>
#include <QDebug>

toolUtil::toolUtil(QObject *parent) : QObject(parent)
{
    m_Process.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_Process,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_consoleOutput()));
}

void toolUtil::funStartCmd(QString str)
{
    m_Process.start(str);
}

void toolUtil::slot_consoleOutput()
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

void toolUtil::readDidFile(QString path)
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


void toolUtil::setWriteDidLabel()
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
