#include "datamodel.h"
#include<QVariant>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "debuglog.h"
#include <QDir>
#include <QTimer>
#include <QDateTime>
DataModel::DataModel(QObject *parent ):QAbstractListModel(parent)
{

    connect(&sendTimer,&QTimer::timeout,this,&DataModel::slot_sendtimerout);
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

void DataModel::funSetIp(QString ip)
{
    if(mip.compare(ip) != 0){

        mip = ip;
        isIpChange = true;
    }

}

void DataModel::funImportSingle(QString name,QString seq,QString imgPath)
{
    QVariantMap map;
    map.insert("cmd","addface");
    map.insert("name",name);
    map.insert("seq",seq);
    map.insert("filePath",imgPath);
    map.insert("msgid","single:"+imgPath);


    funSendCmd(map);
}
void DataModel::funImportBatch(QString folderPath)
{

    QDir dir(folderPath);
    if(!dir.exists())
    {
        DebugLog::getInstance()->writeLog("Batch import fail --- > folder path is not exists");

    }else {

        QFileInfoList list = dir.entryInfoList();


        int batchAddAmount = list.size();
        emit signal_batchAmount(batchAddAmount);
        for (int i=0;i<list.size();i++) {

            QFileInfo fileInfo = list.at(i);

            if(!fileInfo.isDir()){

                QString filename = fileInfo.fileName();
                QString filepath = fileInfo.filePath();
                qDebug()<<"filename:"<<filename;
                qDebug()<<"filepath:"<<filepath;

                if(filename.contains(".jpeg")){


                    QString usefulInfo = filename.replace(".jpeg","");

                    QStringList listinfo = usefulInfo.split("_");


                    QString name = listinfo.at(0);
                    QString seq = listinfo.at(1);


                    QVariantMap map;
                    map.insert("cmd","addface");
                    map.insert("name",name);
                    map.insert("seq",seq);
                    map.insert("filePath",filepath);
                    map.insert("msgid","batch:"+filepath);

                    funSendCmd(map);

                }else{

                    DebugLog::getInstance()->writeLog("import image isValid :"+filepath);
                }

            }
        }
    }
}


void DataModel::createFaceImport(QString ip,int port){

    if(isIpChange){

        if(faceImport != nullptr){
            emit signal_destroyConnect();
            faceImportThread->quit();
            faceImportThread->wait(1000);
            faceImport = nullptr;
            faceImportThread = nullptr;
        }

    }

    if(faceImport == nullptr){

        faceImport = new FaceImport;
        faceImportThread = new QThread;

        connect(this,&DataModel::signal_createFaceImportWork,faceImport,&FaceImport::slot_createConnect);
        connect(this,&DataModel::signal_sendMsg,faceImport,&FaceImport::slot_sendMsg);
        connect(faceImport,&FaceImport::signal_importCallback,this,&DataModel::slot_importCallback);
        connect(this, &DataModel::signal_destroyConnect,faceImport,&FaceImport::slot_destoryConnect);
        connect(faceImportThread, &QThread::finished,faceImport,&FaceImport::deleteLater);
        connect(faceImportThread, &QThread::finished,faceImport,&QThread::deleteLater);

        faceImport->moveToThread(faceImportThread);
        faceImportThread->start();
    }

    emit signal_createFaceImportWork(ip,port);

}
void DataModel::funSendCmd(QVariantMap map)
{


    map.insert("msgid",createMsgId(map.value("cmd").toString()));
    listMsg.append(map);
    if(!sendTimer.isActive()){
        sendTimer.start(200);
    }

}


void DataModel::slot_sendtimerout(){

    qDebug()<<"slot_sendtimerout    ";
    if(listMsg.size() > 0){

        if(!isConnected){

            createFaceImport(mip,port);

            return;
        }


        QVariantMap map =  listMsg.takeFirst();

        if(map.contains("sendTime")){

            qint64 curt = QDateTime::currentMSecsSinceEpoch();
            qint64 sendt = map.value("sendTime").toLongLong();

            if(curt - sendt > 2000){

                emit signal_destroyConnect();
            }
        }else {

            qint64 sendt = QDateTime::currentMSecsSinceEpoch();
            map.insert("sendTime",sendt);
            emit signal_sendMsg(map);
            //listMsg.append(map);
        }
    }else{
        sendTimer.stop();
    }
}

void DataModel::slot_importCallback(QVariantMap map)
{

    QString cmd = map.value("cmd").toString();
    QString msgid = map.value("msgid").toString();
    QString stateCode = map.value("stateCode").toString();

    if(cmd.compare("connect") == 0){

        if(map.value("state").toBool())
            isConnected = true;
        else
            isConnected = false;
    }else if(cmd.compare("addface") == 0){

        QVariantMap imginfomap = removeAlreadySend(cmd,msgid);

        if(msgid.contains("batch:") || msgid.contains("single:")){

            QString filepath = msgid.replace("batch:","");

            if(stateCode.compare("200")==0){

                QString filePath = imginfomap.value("filePath").toString();
                QString seq = imginfomap.value("seq").toString();
                QString name = imginfomap.value("name").toString();
                QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                DataModelData *modedata = new DataModelData(curSelect,filepath,seq,time,name);
                m_listData.append(modedata);

                if(msgid.contains("batch:")){

                    if(stateCode.compare("200")==0)
                        emit signal_batchAdd(true);
                    else
                        emit signal_batchAdd(false);

                }

                if(msgid.contains("single:")){
                    emit signal_singleAdd(true);
                }

            }else {

                if(msgid.contains("batch:")){

                    if(stateCode.compare("200")==0)
                        emit signal_batchAdd(true);
                    else
                        emit signal_batchAdd(false);

                }
                if(msgid.contains("single:")){
                    emit signal_singleAdd(false);
                }

            }

        }


        return;
    }

    removeAlreadySend(cmd,msgid);

}




QString DataModel::createMsgId(QString cmd){
    //QMutexLocker locker(&m_msgMutex);
    int msgId = 0;
    for (int i=0;i<listMsg.size();i++) {
        QMap<QString,QVariant> map = listMsg.at(i);
        QString tmpCmd = map.value("cmd").toString();
        QString msgid = map.value("msgid").toString();
        if(cmd.compare(tmpCmd)==0){
            int tmpid = msgid.toInt();
            if(msgId < tmpid){
                msgId = tmpid+1;
            }
        }
    }
    return QString::number(msgId);
}

QVariantMap DataModel::removeAlreadySend(QString cmd,QString msgid1){

    //QMutexLocker locker(&m_msgMutex);

    int msgLen = listMsg.size();
    for (int i=msgLen-1;i>=0;i--){
        QVariantMap map = listMsg.at(i);
        QString tmpCmd = map.value("cmd").toString();
        QString msgid = map.value("msgid").toString();
        if(cmd.compare(tmpCmd)==0 && msgid.compare(msgid1) ==0){
            listMsg.removeAt(i);
            DebugLog::getInstance()->writeLog("faceObject :remove msg "+cmd);
            return map;
        }
    }

    return QVariantMap();
}


