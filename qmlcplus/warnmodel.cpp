#include "warnmodel.h"
#include<QVariant>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "debuglog.h"
WarnModel::WarnModel(QObject *parent ):QAbstractListModel(parent)
{

//    m_listWarn.append(new WarnModelData(true,"19911103","1111111","sasads","aaaaa"));
//    m_listWarn.append(new WarnModelData(true,"19911103","1111111","sasads","aaaaa"));
//    m_listWarn.append(new WarnModelData(true,"19911103","1111111","sasads","aaaaa"));
//    m_listWarn.append(new WarnModelData(true,"19911103","1111111","sasads","aaaaa"));
}


int WarnModel::rowCount(const QModelIndex &) const
{
    return m_listWarn.count();
}

QVariant WarnModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_listWarn.size())
        return QVariant();

    if (role == IsSelect)
        return QVariant::fromValue(m_listWarn.at(index.row())->isSelect());
    else if(role == WarnTime)
        return QVariant::fromValue(m_listWarn.at(index.row())->warnTime());
    else if(role == WarnTemp)
        return QVariant::fromValue(m_listWarn.at(index.row())->warnTemp());
    else if(role == ImgName)
        return QVariant::fromValue(m_listWarn.at(index.row())->imgName());
    else if(role == AbsolutePath)
        return QVariant::fromValue(m_listWarn.at(index.row())->absolutePath());
    return QVariant();
}

QHash<int, QByteArray> WarnModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IsSelect] = "isSelect";
    roles[WarnTime] = "warnTime";
    roles[WarnTemp] = "warnTemp";
    roles[ImgName] = "imgName";
    roles[AbsolutePath]="absolutePath";
    return roles;
}


bool WarnModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug()<<"setData  "<<role;
    WarnModelData *modelData= m_listWarn[index.row()];
    if (data(index, role) != value) {
        if(role == IsSelect)
            modelData->setIsSelect(value.toBool());
        else if(role == WarnTime)
            modelData->setWarnTime(value.toString());
        else if(role == WarnTemp)
            modelData->setWarnTemp(value.toString());
        else if(role == ImgName)
            modelData->setImgName(value.toString());
        else if(role == AbsolutePath)
            modelData->setImgName(value.toString());
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags WarnModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

void WarnModel::removeIndex(int index)
{

}

void WarnModel::removeAll()
{
    if(m_listWarn.size() <= 0)
        return;
    beginRemoveRows(QModelIndex(),0,m_listWarn.size()-1);
    while(m_listWarn.size()>0){

        WarnModelData *date = m_listWarn.takeFirst();
        if(date != nullptr || date != NULL || date)
            delete  date;

    }
    endRemoveRows();
}

void WarnModel::funDeleteIndex(int index)
{
    qDebug()<<" funDeleteIndex:"<<index;
    if(m_listWarn.size() <= index){
        DebugLog::getInstance()->writeLog("删除警报信息异常:index 越界");
        return;
    }

    beginRemoveRows(QModelIndex(),index,index);
    WarnModelData *date = m_listWarn.takeAt(index);
    delete date;
    endRemoveRows();
}
void WarnModel::funFlushWarnInfo(QString capturePath,QString logFileName)
{
    qDebug()<<"logPath:"<<capturePath<<"    logFileName"<<logFileName;

    QString logAbsolutePath = capturePath+"/log/"+logFileName+".log";
    QFile file(logAbsolutePath);
    if(file.open(QIODevice::ReadOnly)){

        removeAll();
        QTextStream in(&file);
        QString warnStr = in.readLine();


        while (!warnStr.isNull()) {
            QStringList strlist = warnStr.split(" ");
            //qDebug()<<"strlist:"<<strlist;
            if(strlist.size() != 3){
                DebugLog::getInstance()->writeLog("读取警报日志数据异常"+strlist.size());
                break;
            }
            QString absolutepath = strlist[0];
            QString fileName = strlist[2];
            QString temp = strlist[1];
            QString datetime = fileName.remove(".png");
            QStringList datetStr = datetime.split("_");
            if(datetStr.size() != 2){
                DebugLog::getInstance()->writeLog("对日期和时间区分时数据异常"+datetStr.size());
                break;
            }
            QString date = datetStr[0].mid(0,2)+"-"+datetStr[0].mid(2,2)+"-"+datetStr[0].mid(4,2);
            QString time = datetStr[1].mid(0,2)+":"+datetStr[1].mid(2,2)+":"+datetStr[1].mid(4,2);

            m_listWarn.append(new WarnModelData(false,date+" "+time,temp,fileName,absolutepath));
            warnStr = in.readLine();
        }
        if(m_listWarn.size() >0){
            beginInsertRows(QModelIndex(),0,m_listWarn.size()-1);
            endInsertRows();
        }

    }else {
        qDebug()<<"日志文件打开失败";
    }
}
int WarnModel::funFindIndex(QString h,QString m,QString s){

    qDebug()<<" funFindIndex："<<h<<"    "<<m<<" "<<s<<" "<<m_listWarn.size();
    //返回第一个大于等于 当前时分秒的时间点
    if(m_listWarn.size() > 0){
        for (int i=0;i<m_listWarn.size();i++) {
            WarnModelData *date = m_listWarn.at(i);
            QString datetime = date->warnTime();
            //qDebug()<<" datetime:"<<datetime;
            QStringList datetimelist = datetime.split(" ");
            if(datetimelist.size() != 2){
                DebugLog::getInstance()->writeLog("对日期和时间区分时数据异常:"+datetimelist.size());
                continue;
            }

            QStringList time = datetimelist[1].split(":");
            if(time.size() != 3){
                DebugLog::getInstance()->writeLog("区分时分秒数据异常:"+datetimelist.size());
                continue;
            }

            int timeH = time[0].toInt();
            int timeM = time[1].toInt();
            int timeS = time[2].toInt();

            int curH = h.toInt();
            int curM = m.toInt();
            int curS = s.toInt();


            //找出大于当前时间的点（先比较时在比较分在比较秒）
            if(timeH > curH){
                return i;
            }else if (timeH == curH) {
                if(timeM > curM)
                    return i;
                else if (timeM == curM) {
                    if(timeS > curS)
                        return i;
                    else if (timeS == curS) {
                        return i;
                    }
                }
            }

        }
    }
    return -1;
}


void WarnModel::funSetAllSelect(bool isSelect)
{
    for (int i=0;i<m_listWarn.size();i++) {
        m_listWarn.at(i)->setIsSelect(isSelect);
    }
    endResetModel();
}
void WarnModel::funDeleteSelect(){
    beginResetModel();

    for (int i=m_listWarn.size()-1;i>=0;i--) {
        if(m_listWarn.at(i)->isSelect()){
            delete m_listWarn.takeAt(i);
        }
    }

    endResetModel();
}
