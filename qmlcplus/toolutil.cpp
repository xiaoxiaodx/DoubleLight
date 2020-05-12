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


    QString cmd = "act_dv300_common.exe "+keystr+" "+keystr+" 5d7073359bb502922d0b0566b8d8a6eb 883a63d3af6569f8997b625bd1162966f92139af";

    //QString cmd = "ping 192.168.1.1";
    //qDebug()<<"funStartCmd  "<<cmd;
    /*act_dv300_common.exe 76b627633d6beb09edeae06a7952b2a2 76b627633d6beb09edeae06a7952b2a2 5d7073359bb502922d0b0566b8d8a6eb 883a63d3af6569f8997b625bd1162966f92139af*/

    m_Process.start(cmd);

}
void ToolUtil::setDidLisence(QString did,QString lisence)
{

    if(did == "" || lisence == "")
    {
        emit signal_tip(false,"id 或 lisence为空");
        return;
    }
    if(did == "已存入文件" || lisence == "已存入文件")
    {
        emit signal_tip(false,"已经存入");
        return;
    }
    QFile file("Did_Lisence.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream  in(&file);
        in<<did<<"\t"<<lisence<<endl;
        file.close();
        emit signal_tip(true,"保存成功");
    }else {
        emit signal_tip(false,"文件打开失败...请检测文件路径是否为英文");
    }

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

        if(msg.contains("sign ") && msg.contains("verify res: 0")){

            int lisenceIndex = msg.indexOf("sign ");

            lisenceIndex += 5;

            QString lisenceStr = msg.mid(lisenceIndex,msg.size() - lisenceIndex);

            in<<"Lisence:"<<lisenceStr<<endl;


            //QString tmpstr = "1bf55d1bbcf14fa1188535a73cd8c06eea103f8ad6c4bc6a60c8795bafcdb7c35a18b7e1994c50d9f238841997c9f251428d74e2cca3fb6ad2681fe52ba70d96";
            emit signal_sendLisence(lisenceStr);
            //emit signal_sendLisence(tmpstr);
        }

        file.close();
    }
}

void ToolUtil::readDidFile(QString path)
{

    myDidList.clear();
    didFilePath = path;
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

void ToolUtil::deleteFirstDid(){

    QFile rewriteDidfile(didFilePath);
    if(rewriteDidfile.open(QFile::WriteOnly| QFile::Text)){
        QTextStream out(&rewriteDidfile);
        for(int i=0;i<myDidList.size();i++) {

            out<<myDidList.at(i)<<endl;
        }
        rewriteDidfile.close();
    }
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
