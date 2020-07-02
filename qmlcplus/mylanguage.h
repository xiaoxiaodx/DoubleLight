#ifndef MYLANGUAGE_H
#define MYLANGUAGE_H

#include <QObject>

#include <QMap>
#include<QVariant>
class LanInfo{

public:
    QString lanName;
    QMap<QString,QString> entryMap;

};

#define QML_PROPERTY(type,name) \
    Q_PROPERTY(type name READ name WRITE set##name NOTIFY name##Change) \
    public: type name() const { return m_##name;} \
    public Q_SLOTS: void set##name(type arg) { m_##name = arg; emit name##Change(arg);} \
    Q_SIGNALS:  \
    void name##Change(type arg);\
    private: \
    type m_##name; \

class MyLanguage : public QObject
{


    Q_OBJECT
    //菜单
    QML_PROPERTY(QString,Masterview)
    QML_PROPERTY(QString,VideoPlayback)
    QML_PROPERTY(QString,DeviceManagement)
    QML_PROPERTY(QString,AlarmManagement)
    //主预览
    QML_PROPERTY(QString,DeviceList)
    QML_PROPERTY(QString,Add)
    QML_PROPERTY(QString,AlarmRecord)
    //录像回放
    QML_PROPERTY(QString,VideoNormal)
    QML_PROPERTY(QString,VideoWarn)
    QML_PROPERTY(QString,VideoLoss)
    //对话框
    QML_PROPERTY(QString,PopMsgSelectChannel)
    QML_PROPERTY(QString,AskMsgChannelChange)
    QML_PROPERTY(QString,AskMsgDelete)
    QML_PROPERTY(QString,AskMsgExit)
    QML_PROPERTY(QString,AskMsgEnsure)
    QML_PROPERTY(QString,AskMsgCancel)
    //设备管理
    QML_PROPERTY(QString,BatchDelete)
    QML_PROPERTY(QString,BatchSetting)
    QML_PROPERTY(QString,DeviceDid)
    QML_PROPERTY(QString,DeviceName)
    QML_PROPERTY(QString,DeviceChannel)
    QML_PROPERTY(QString,DeviceVideoSavePath)
    QML_PROPERTY(QString,DeviceNetState)
    QML_PROPERTY(QString,Operating)         //通用词条
    QML_PROPERTY(QString,DeviceAdd)
    QML_PROPERTY(QString,DeviceOnLine)
    QML_PROPERTY(QString,DeviceOffLine)
    QML_PROPERTY(QString,InputKeyWord)
    //告警管理
    QML_PROPERTY(QString,LogList)
    QML_PROPERTY(QString,WarnTime)
    QML_PROPERTY(QString,WarnTemp)
    QML_PROPERTY(QString,WarnSnapshot)
    //设备配置
    QML_PROPERTY(QString,DeviceConfig)
    QML_PROPERTY(QString,OsdSet)
    QML_PROPERTY(QString,Channel)
    QML_PROPERTY(QString,RecordSet)
    QML_PROPERTY(QString,RtmpSet)
    QML_PROPERTY(QString,TimeSet)
    QML_PROPERTY(QString,TempSet)

    //osd
    QML_PROPERTY(QString,Time)
    QML_PROPERTY(QString,Switch)
    QML_PROPERTY(QString,Name)
    QML_PROPERTY(QString,Show)
    //录像
    QML_PROPERTY(QString,Set)
    QML_PROPERTY(QString,VideoClose)
    QML_PROPERTY(QString,VideoAlarm)
    QML_PROPERTY(QString,VideoFull)
    QML_PROPERTY(QString,HD)//高清
    QML_PROPERTY(QString,SD)//标清
    QML_PROPERTY(QString,Smooth)//流畅
    QML_PROPERTY(QString,Quality) //视频质量
    QML_PROPERTY(QString,RecordPath)
    QML_PROPERTY(QString,RecordTime)
    QML_PROPERTY(QString,AllSelect)
    //周 1-7
    QML_PROPERTY(QString,Mon)
    QML_PROPERTY(QString,Tue)
    QML_PROPERTY(QString,Wed)
    QML_PROPERTY(QString,Thu)
    QML_PROPERTY(QString,Fri)
    QML_PROPERTY(QString,Sat)
    QML_PROPERTY(QString,Sun)
    //rtmp
    QML_PROPERTY(QString,TimeSwitch)
    QML_PROPERTY(QString,Resolution)
    QML_PROPERTY(QString,User)
    QML_PROPERTY(QString,Password)
    //时间设置
    QML_PROPERTY(QString,NtpSet)
    QML_PROPERTY(QString,TimeZoneSelect)
    QML_PROPERTY(QString,SummerTimeSet)
    //温度设置
    QML_PROPERTY(QString,ParaSet)
    QML_PROPERTY(QString,Alarm)
    QML_PROPERTY(QString,Snapshot)
    QML_PROPERTY(QString,Beer)
    QML_PROPERTY(QString,AlarmTempSet)
    QML_PROPERTY(QString,SnapshotPath)
    QML_PROPERTY(QString,TempDriftSet)
    QML_PROPERTY(QString,TempControlValue)

public:
    explicit MyLanguage(QObject *parent = nullptr);

    Q_INVOKABLE void updateLanguage(int filenam);
    Q_INVOKABLE QVariant getLanMenu();
    Q_INVOKABLE QString getCurPath();
signals:

    void signal_updateLan();
public slots:


private:
    bool addLanguage(QString lanFileName);

    void loadAllLanguage();

    QStringList lanList;
    QList<LanInfo> listLanInfo;
};

#endif // MYLANGUAGE_H
