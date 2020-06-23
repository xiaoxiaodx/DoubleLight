#include "datamodel.h"
#include<QVariant>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "debuglog.h"

DataModel::DataModel(QObject *parent ):QAbstractListModel(parent)
{

}

int DataModel::rowCount(const QModelIndex &) const
{
    return m_listData.count();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_listData.size())
        return QVariant();

    if (role == IsSelect)
        return QVariant::fromValue(m_listData.at(index.row())->isSelect());
    else if(role == AvatarPath)
        return QVariant::fromValue(m_listData.at(index.row())->avatarPath());
    else if(role == Time)
        return QVariant::fromValue(m_listData.at(index.row())->time());
    else if(role == Name)
        return QVariant::fromValue(m_listData.at(index.row())->name());
    else if(role == Numbering)
        return QVariant::fromValue(m_listData.at(index.row())->numbering());
    return QVariant();
}


QHash<int, QByteArray> DataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IsSelect] = "isSelect";
    roles[AvatarPath] = "avatarPath";
    roles[Numbering] = "numbering";
    roles[Name] = "name";
    roles[Time]="time";
    return roles;
}


bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug()<<"setData  "<<role;
    DataModelData *modelData= m_listData[index.row()];
    if (data(index, role) != value) {
        if(role == IsSelect)
            modelData->setisSelect(value.toBool());
        else if(role == AvatarPath)
            modelData->setavatarPath(value.toString());
        else if(role == Numbering)
            modelData->setnumbering(value.toString());
        else if(role == Name)
            modelData->setname(value.toString());
        else if(role == Time)
            modelData->settime(value.toString());

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

void DataModel::removeIndex(int index)
{

}

void DataModel::removeAll()
{
    if(m_listData.size() <= 0)
        return;
    beginRemoveRows(QModelIndex(),0,m_listData.size()-1);
    while(m_listData.size()>0){

        DataModelData *date = m_listData.takeFirst();

        //        QFile::remove(date->absolutePath());
        if(date != nullptr || date != NULL || date)
            delete  date;

    }
    endRemoveRows();
}

//void DataModel::funDeleteIndex(int index)
//{
//    qDebug()<<" funDeleteIndex:"<<index;
//    if(m_listData.size() <= index){
//        DebugLog::getInstance()->writeLog("删除警报信息异常:index 越界");
//        return;
//    }

//    beginRemoveRows(QModelIndex(),index,index);
//    DataModelData *date = m_listData.takeAt(index);
//    QString fileAbsolutePath = date->absolutePath();
//    QFile::remove(fileAbsolutePath);
//    delete date;
//    endRemoveRows();

//    if(curPath == ""||curDate=="")
//        return;

//    //日志文件的绝对路径，重写文件,倒序写入文件
//    QString logAbsolutePath = curPath+"/log/"+curDate+".log";
//    QFile file(logAbsolutePath);
//    if(file.open(QIODevice::WriteOnly)){
//        for (int i=m_listData.size()-1;i>=0;i--) {
//            DataModelData *modelData = m_listData.at(i);
//            QString imgInfoStr = modelData->absolutePath()+" "+modelData->warnTemp()+" "+modelData->imgName()+".jpeg";
//            QTextStream out(&file);
//            out <<imgInfoStr << "\n";
//        }
//        file.close();
//    }

//}

