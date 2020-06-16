#include "mylanguage.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextCodec>
MyLanguage::MyLanguage(QObject *parent) : QObject(parent)
{


    loadAllLanguage();
}


void MyLanguage::updateLanguage(int index)
{

    //qDebug()<<">>>>>>   updateLanguage:"<<index;

    if(index < listLanInfo.size()){

        LanInfo laninfo = listLanInfo.at(index);
        //qDebug()<<">>>>>>   laninfo:"<<laninfo.lanName;
        QMap<QString,QString> entryMap = laninfo.entryMap;
        QMap<QString, QString>::const_iterator i = entryMap.constBegin();

        while(i != entryMap.constEnd()){
           // qDebug()<<">>>>>> "<<i.key()<<" "<<i.value();
            //主预览
            if(i.key().compare("Masterview")==0){
                setMasterview(i.value());
            }else if(i.key().compare("VideoPlayback")==0){
                setVideoPlayback(i.value());
            }else if(i.key().compare("DeviceManagement")==0){
                setDeviceManagement(i.value());
            }else if(i.key().compare("AlarmManagement")==0){
                setAlarmManagement(i.value());
            }else if(i.key().compare("DeviceList")==0){
                setDeviceList(i.value());
            }else if(i.key().compare("Add")==0){
                setAdd(i.value());
            }else if(i.key().compare("AlarmRecord")==0){
                setAlarmRecord(i.value());
            }
            //录像回放
            else if(i.key().compare("VideoNormal")==0){
                setVideoNormal(i.value());
            }else if(i.key().compare("VideoWarn")==0){
                setVideoWarn(i.value());
            }else if(i.key().compare("VideoLoss")==0){
                setVideoLoss(i.value());
            }
            //对话框
            else if(i.key().compare("PopMsgSelectChannel")==0){
                setPopMsgSelectChannel(i.value());
            }else if(i.key().compare("AskMsgDelete")==0){
                setAskMsgDelete(i.value());
            }else if(i.key().compare("AskMsgExit")==0){
                setAskMsgExit(i.value());
            }else if(i.key().compare("AskMsgEnsure")==0){
                setAskMsgEnsure(i.value());
            }else if(i.key().compare("AskMsgCancel")==0){
                setAskMsgCancel(i.value());
            }else if(i.key().compare("AskMsgChannelChange")==0){
                setAskMsgChannelChange(i.value());
            }
            //设备管理
            else if(i.key().compare("BatchDelete")==0){
                setBatchDelete(i.value());
            }else if(i.key().compare("BatchSetting")==0){
                setBatchSetting(i.value());
            }else if(i.key().compare("DeviceDid")==0){
                setDeviceDid(i.value());
            }else if(i.key().compare("DeviceName")==0){
                setDeviceName(i.value());
            }else if(i.key().compare("DeviceChannel")==0){
                setDeviceChannel(i.value());
            }else if(i.key().compare("DeviceVideoSavePath")==0){
                setDeviceVideoSavePath(i.value());
            }else if(i.key().compare("DeviceNetState")==0){
                setDeviceNetState(i.value());
            }else if(i.key().compare("Operating")==0){
                setOperating(i.value());
            }else if(i.key().compare("DeviceAdd")==0){
                setDeviceAdd(i.value());
            }else if(i.key().compare("DeviceOnLine")==0){
                setDeviceOnLine(i.value());
            }else if(i.key().compare("DeviceOffLine")==0){
                setDeviceOffLine(i.value());
            }else if(i.key().compare("InputKeyWord")==0){
                setInputKeyWord(i.value());
            }
            //告警管理
            else if(i.key().compare("LogList")==0){
                setLogList(i.value());
            }else if(i.key().compare("WarnTime")==0){
                setWarnTime(i.value());
            }else if(i.key().compare("WarnTemp")==0){
                setWarnTemp(i.value());
            }else if(i.key().compare("WarnSnapshot")==0){
                setWarnSnapshot(i.value());
            }
            //设备配置
            else if(i.key().compare("DeviceConfig")==0){
                setDeviceConfig(i.value());
            }else if(i.key().compare("Channel")==0){
                setChannel(i.value());
            }else if(i.key().compare("RecordSet")==0){
                setRecordSet(i.value());
            }else if(i.key().compare("RtmpSet")==0){
                setRtmpSet(i.value());
            }else if(i.key().compare("TimeSet")==0){
                setTimeSet(i.value());
            }else if(i.key().compare("TempSet")==0){
                setTempSet(i.value());
            }else if(i.key().compare("OsdSet")==0){
                setOsdSet(i.value());
            }
            //osd
            else if(i.key().compare("Time")==0){
                setTime(i.value());
            }else if(i.key().compare("Switch")==0){
                setSwitch(i.value());
            }else if(i.key().compare("Name")==0){
                setName(i.value());
            }else if(i.key().compare("Show")==0){
                setShow(i.value());
            }
            //录像
            else if(i.key().compare("Set")==0){
                setSet(i.value());
            }else if(i.key().compare("VideoClose")==0){
                setVideoClose(i.value());
            }else if(i.key().compare("VideoAlarm")==0){
                setVideoAlarm(i.value());
            }else if(i.key().compare("VideoFull")==0){
                setVideoFull(i.value());
            }else if(i.key().compare("HD")==0){
                setHD(i.value());
            }else if(i.key().compare("SD")==0){
                setSD(i.value());
            }else if(i.key().compare("Smooth")==0){
                setSmooth(i.value());
            }else if(i.key().compare("Quality")==0){
                setQuality(i.value());
            }else if(i.key().compare("RecordPath")==0){
                setRecordPath(i.value());
            }else if(i.key().compare("RecordTime")==0){
                setRecordTime(i.value());
            }else if(i.key().compare("AllSelect")==0){
                setAllSelect(i.value());
            }
            //周 1-7
            else if(i.key().compare("Mon")==0){
                setMon(i.value());
            }else if(i.key().compare("Tue")==0){
                setTue(i.value());
            }else if(i.key().compare("Wed")==0){
                setWed(i.value());
            }else if(i.key().compare("Thu")==0){
                setThu(i.value());
            }else if(i.key().compare("Fri")==0){
                setFri(i.value());
            }else if(i.key().compare("Sat")==0){
                setSat(i.value());
            }else if(i.key().compare("Sun")==0){
                setSun(i.value());
            }
            //rtmp
            else if(i.key().compare("TimeSwitch")==0){
                setTimeSwitch(i.value());
            }else if(i.key().compare("Resolution")==0){
                setResolution(i.value());
            }else if(i.key().compare("User")==0){
                setUser(i.value());
            }else if(i.key().compare("Password")==0){
                setPassword(i.value());
            }
            //时间设置
            else if(i.key().compare("NtpSet")==0){
                setNtpSet(i.value());
            }else if(i.key().compare("TimeZoneSelect")==0){
                setTimeZoneSelect(i.value());
            }else if(i.key().compare("SummerTimeSet")==0){
                setSummerTimeSet(i.value());
            }
            //温度设置
            else if(i.key().compare("ParaSet")==0){
                setParaSet(i.value());
            }else if(i.key().compare("Alarm")==0){
                setAlarm(i.value());
            }else if(i.key().compare("Snapshot")==0){
                setSnapshot(i.value());
            }else if(i.key().compare("Beer")==0){
                setBeer(i.value());
            }else if(i.key().compare("AlarmTempSet")==0){
                setAlarmTempSet(i.value());
            }else if(i.key().compare("SnapshotPath")==0){
                setSnapshotPath(i.value());
            }else if(i.key().compare("TempDriftSet")==0){
                setTempDriftSet(i.value());
            }else if(i.key().compare("TempControlValue")==0){
                setTempControlValue(i.value());
            }

            ++i;
        }

        emit signal_updateLan();
    }
}

QString MyLanguage::getCurPath(){

    return QDir::currentPath();
}


void MyLanguage::loadAllLanguage()
{

    qDebug()<<">>>loadAllLanguage:"<<QDir::currentPath();

    QString curPath = QDir::currentPath();
    QString lanMenuPath = curPath+"/language/lanMenu";

    QFile fileLanMenu(lanMenuPath);

    if(fileLanMenu.open(QIODevice::ReadOnly | QIODevice::Text)){

        qDebug()<<"打开语言菜单文件成功";
        QTextStream outStream(&fileLanMenu);
        QTextCodec *code=QTextCodec::codecForName("utf8");
        outStream.setCodec(code);
        while (!outStream.atEnd()) {
            QString tmpStr = outStream.readLine();
            qDebug()<<"readLine:"<<tmpStr;
            lanList.append(tmpStr);
        }
    }else {
        qDebug()<<"打开语言菜单文件失败";
    }

    for (int i=0;i<lanList.size();i++) {

        QString lanStr = lanList.at(i);

        QString lanfilePath = curPath+"/language/"+lanStr;

        if(!addLanguage(lanfilePath)){
            lanList.removeAt(i);
            i--;
        }

    }


}

QVariant MyLanguage::getLanMenu(){

    return lanList;
}
bool MyLanguage::addLanguage(QString filename)
{

    QFile file(filename);


    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        LanInfo info;
        QFileInfo fileinfo(filename);
        info.lanName = fileinfo.fileName();
        qDebug()<<"addLanguage "<<info.lanName;
        QTextStream txtstream(&file);
        QTextCodec *code=QTextCodec::codecForName("utf8");
        txtstream.setCodec(code);
        while (!txtstream.atEnd()){
            QString str = txtstream.readLine();
            QStringList entryKey = str.split("=");

            if(entryKey.size() != 2){
                return false;
            }

            // qDebug()<<"language:"<<entryKey[0]<<"   "<<entryKey[1];
            info.entryMap.insert(entryKey[0],entryKey[1]);

        }

        listLanInfo.append(info);
        return true;
    }else
        return false;

}


