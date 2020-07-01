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

void DataModel::funSetInitSelectFalse()
{
    curSelect = false;

}

void DataModel::funSendReviseRequest(int index,QString name,QString number)
{
    QVariantMap map;
    map.insert("cmd","modifyface");
    map.insert("name",name);
    map.insert("seq",number);
    map.insert("index",index);
    funSendCmd(map);
}

void DataModel::reviseIndex(int index,QString name,QString number)
{

    qDebug()<<"     reviseIndex "<<name<<"  "<<number;
    beginResetModel();
    DataModelData *date = m_listData.takeAt(index);
   QString tmpFilePath = date->avatarPath();



    QString oriFilePath = tmpFilePath.replace("file:///","");

    QFileInfo fileinfo(oriFilePath);

    QString desFilePath = oriFilePath.replace(fileinfo.fileName(),"") + name+"_"+number+".jpeg";


    qDebug()<<"reviseIndex:"<<tmpFilePath<<"    "<<oriFilePath<<"    "<<desFilePath;
    QFile file(tmpFilePath);

    file.rename(desFilePath);
    m_listData.insert(index,new DataModelData(date->isSelect(),"file:///"+desFilePath,number,date->time(),name));

    delete  date;
    endResetModel();


    rewriteLogFile();
}

void DataModel::removeIndex(int index)
{

    qDebug()<<" funDeleteIndex:"<<index;
    if(m_listData.size() <= index){
        DebugLog::getInstance()->writeLog("删除人脸信息异常:index 越界");
        return;
    }

    beginRemoveRows(QModelIndex(),index,index);
    DataModelData *date = m_listData.takeAt(index);
    QString tmpFilePath = date->avatarPath();
    delete date;
    QString desFilePath = tmpFilePath.replace("file:///","");
    QFile file(desFilePath);
    file.remove();
    endRemoveRows();

    rewriteLogFile();
}

void DataModel::rewriteLogFile()
{

    //日志文件的绝对路径，重写文件,倒序写入文件
    QString curDir = QDir::currentPath();
    QString logAbsolutePath = curDir+"/Face import/"+mdid+"/Log/"+curCalendarDate+".log";

    QFile file(logAbsolutePath);
    if(file.open(QIODevice::WriteOnly)){
        for (int i=m_listData.size()-1;i>=0;i--) {
            DataModelData *modelData = m_listData.at(i);

            QString name = modelData->name();
            QString filepath = modelData->avatarPath().replace("file:///","");
            QString number = modelData->numbering();
            QString importtime = modelData->time();

            QString imgInfoStr = filepath+"***"+name+"***"+number+"***"+importtime;
            QTextStream out(&file);
            out <<imgInfoStr << "\n";
        }
        file.close();
    }

}


void DataModel::funSendDeleteSelectRequest()
{


    for (int i=m_listData.size()-1;i>=0;i--) {
        if(m_listData.at(i)->isSelect()){

            funSendDeleteRequest(i);
        }
    }
    curSelect = false;
}

void DataModel::funSendDeleteRequest(int index)
{

    qDebug()<<" funSendDeleteRequest:"<<index;
    if(m_listData.size() <= index){
        DebugLog::getInstance()->writeLog("删除人脸信息异常:index 越界");
        return;
    }

    DataModelData *date = m_listData.at(index);
    QVariantMap map;
    map.insert("cmd","delface");
    map.insert("name",date->name());
    map.insert("seq",date->numbering());
    map.insert("index",index);
    funSendCmd(map);

}

void DataModel::funSetAllSelect(bool isSelect)
{
    curSelect = isSelect;
    beginResetModel();
    for (int i=0;i<m_listData.size();i++) {
        m_listData.at(i)->setisSelect(isSelect);
    }
    endResetModel();
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

void DataModel::funSetDeviceInfo(QString ip,QString did)
{
    if(mip.compare(ip) != 0){
        mdid = did;
        mip = ip;
        isIpChange = true;

        funUpdateCurCalendarDate(curCalendarDate);
    }

}

void DataModel::funImportSingle(QString name,QString seq,QString imgPath)
{
    QVariantMap map;
    map.insert("cmd","addface");
    map.insert("name",name);
    map.insert("seq",seq);
    map.insert("filePath",imgPath);
    map.insert("importType","single");


    funSendCmd(map);
}
void DataModel::funImportBatch(QString folderPath)
{

    QDir dir(folderPath);
    if(!dir.exists())
    {
        DebugLog::getInstance()->writeLog("Batch import fail --- > folder path is not exists");

    }else {
        QStringList filter;
        filter<<"*.jpeg";
        dir.setNameFilters(filter);

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
                    map.insert("importType","batch");


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

        isIpChange = false;
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

    qDebug()<<"funSendCmd   "<<map;
    listMsg.append(map);
    if(!sendTimer.isActive()){
        sendTimer.start(500);
    }

}


void DataModel::slot_sendtimerout(){

    qDebug()<<"slot_sendtimerout    "<<listMsg.size();
    if(listMsg.size() > 0){

        if(!isConnected){
            createFaceImport(mip,port);
            return;
        }

        QVariantMap map =  listMsg.takeFirst();

        //    if(map.contains("sendTime")){
        // qint64 curt = QDateTime::currentMSecsSinceEpoch();
        // qint64 sendt = map.value("sendTime").toLongLong();
        //            if(curt - sendt > 2000){
        //                emit signal_destroyConnect();
        //            }
        //        }else {

        qint64 sendt = QDateTime::currentMSecsSinceEpoch();
        map.insert("sendTime",sendt);
        emit signal_sendMsg(map);



        // }
        listMsg.append(map);

    }else{
        sendTimer.stop();
    }
}

void DataModel::funUpdateCurCalendarDate(QString datetime){

    curCalendarDate = datetime;

    qDebug()<<"funUpdateCurCalendarDate "<<datetime;

    QString curDate = QDate::currentDate().toString("yyyyMMdd");

    //同一天的数据则不进行刷新，会在列表中添加完后在进行写文件
//    if(!isFirst && curDate.compare(curCalendarDate)==0)
//        return;

    if(mdid == "")
        return;

    //不是同一天则移除原有数据,刷新新的列表
    removeAll();
    QString curDir = QDir::currentPath();
    QString logAbsolutePath = curDir+"/Face import/"+mdid+"/Log/"+curCalendarDate+".log";

    qDebug()<<"logAbsolutePath  "<<logAbsolutePath;
    QFile file(logAbsolutePath);
    //每次读取都是往0位置插入值
    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        QString warnStr = in.readLine();

        while (!warnStr.isNull()) {
            QStringList strlist = warnStr.split("***");
            qDebug()<<"strlist:"<<strlist;
            if(strlist.size() != 4){
                DebugLog::getInstance()->writeLog("读取人脸日志数据异常 "+strlist.size());
                break;
            }

            QString absolutepath = strlist[0];
            QString name = strlist[1];
            QString number = strlist[2];
            QString datetime = strlist[3];


            m_listData.insert(0,new DataModelData(curSelect,"file:///"+absolutepath,number,datetime,name));
            warnStr = in.readLine();
        }

        if(m_listData.size() >0){
            beginInsertRows(QModelIndex(),0,m_listData.size()-1);
            endInsertRows();
        }

    }else {
        qDebug()<<"日志文件打开失败";
    }


}

void DataModel::slot_importCallback(QVariantMap map)
{

    QString cmd = map.value("cmd").toString();
    QString msgid = map.value("msgid").toString();
    QString stateCode = map.value("stateCode").toString();
    QVariantMap tmpInfoMap = removeAlreadySend(cmd,msgid);
    qDebug()<<"slot_importCallback1:"<<tmpInfoMap;
    if(cmd.compare("connect") == 0){

        if(map.value("state").toBool())
            isConnected = true;
        else
            isConnected = false;
    }else if(cmd.compare("addface") == 0){

        if(tmpInfoMap.contains("importType")){

            QString importType = tmpInfoMap.value("importType").toString();
            QString filePath = tmpInfoMap.value("filePath").toString();
            QString seq = tmpInfoMap.value("seq").toString();
            QString name = tmpInfoMap.value("name").toString();

            bool isImportSucc = false;
            bool isBatchImport = false;

            if(importType.compare("single") == 0)
                isBatchImport = false;

            if(importType.compare("batch") == 0)
                isBatchImport = true;


            if(stateCode.compare("200") == 0)
                isImportSucc = true;
            else
                isImportSucc = false;

            addInfoInList(isBatchImport,isImportSucc,name,seq,filePath);
        }
    }else if(cmd.compare("delface") == 0){

        int index = tmpInfoMap.value("index").toInt();
        removeIndex(index);

    }else if(cmd.compare("modifyface") == 0){

        int index =  tmpInfoMap.value("index").toInt();
        QString name = tmpInfoMap.value("name").toString();
        QString number = tmpInfoMap.value("seq").toString();

        reviseIndex(index,name,number);

    }

}


void DataModel::addInfoInList(bool isBatch,bool isSucc,QString name,QString number,QString filePath)
{

    if(isSucc){

        QString desFilePath;

        QString desPath = filePath;

        QString newname = "";
        if(isBatch){

            emit signal_batchAdd(true);

        }else {//单个导入的图片修改文件名
            newname = name+"_"+number+".jpeg";
            emit signal_singleAdd(true);

        }

        desPath = transferImportFile(isSucc,filePath,newname);


        qDebug()<<"addInfoInList:"<<desPath;

        writeLogInfo(desPath,name,number,QDateTime::currentDateTime());

        qDebug()<<"addInfoInList:222"<<desPath;
    }else{

        QString newname = "";
        if(isBatch){

            emit signal_batchAdd(false);

        }else{
            newname = name+"_"+number+".jpeg";
            emit signal_singleAdd(false);

        }

        transferImportFile(isSucc,filePath,newname);
    }
}

void DataModel::writeLogInfo(QString filepath,QString name,QString number,QDateTime importdatetime)
{

    qDebug()<<"writeLogInfo ";

    qDebug()<<"writeLogInfo "<<filepath;
    qDebug()<<"writeLogInfo "<<name;
    qDebug()<<"writeLogInfo "<<number;
    qDebug()<<"writeLogInfo 1";

    QString curpath = QDir::currentPath();

    QString dirpath = curpath + "/Face import/"+mdid+ "/Log";


    QDir dir;
    if (!dir.exists(dirpath)){
        bool res = dir.mkpath(dirpath);
        if(res)
            DebugLog::getInstance()->writeLog("batch import create new dir is succ");
        else{
            DebugLog::getInstance()->writeLog("batch import create new dir is fail");
            //return "";
        }
    }

    qDebug()<<"writeLogInfo 2";
    QString time = QDate::currentDate().toString("yyyyMMdd");

    QString desfile = dirpath+"/"+time+".log";

    QFile imgInfofile(desfile);
    if(imgInfofile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){

        QString importtime = importdatetime.toString("yyyy-MM-dd hh:mm:ss");
        QString imgInfoStr = filepath+"***"+name+"***"+number+"***"+importtime;
        QTextStream out(&imgInfofile);
        out <<imgInfoStr << "\n";
        imgInfofile.close();

        QString date = importdatetime.toString("yyyyMMdd");
        QString time = importdatetime.toString("hh:mm:ss");


        qDebug()<<"writeLogInfo 3";
        //是当前日期才加入列表日志
        if(curCalendarDate.compare(date)==0){
            qDebug()<<"writeLogInfo 4";
            beginInsertRows(QModelIndex(),0,0);
            DataModelData *modedata = new DataModelData(curSelect,"file:///"+filepath,number,importtime,name);
            m_listData.insert(0,modedata);
            endInsertRows();
            qDebug()<<"writeLogInfo 5";
        }

    }else {
        DebugLog::getInstance()->writeLog("writeLogInfo open log file is fail");

    }

}

QString DataModel::transferImportFile(bool isSucc,QString filePath,QString newName)
{

    QString tmpStr = "";

    if(isSucc)
        tmpStr = "succ";
    else
        tmpStr = "fail";

    QFile file(filePath);
    QFileInfo fileinfo(filePath);
    QString desFileDir = QDir::currentPath();

    desFileDir = desFileDir+"/Face import/"+mdid+ "/"+tmpStr;

    qDebug()<<"desFileDir   "<<desFileDir;
    QDir dir;
    if (!dir.exists(desFileDir)){
        bool res = dir.mkpath(desFileDir);
        if(res)
            DebugLog::getInstance()->writeLog("batch import create new dir is succ");
        else{
            DebugLog::getInstance()->writeLog("batch import create new dir is fail");
            return "";
        }
    }
    QString desFilePath;
    if(newName.compare("") == 0)
        desFilePath = desFileDir + "/"+fileinfo.fileName();
    else {
        desFilePath = desFileDir + "/"+newName;
    }
    qDebug()<<"desFilePath:"<<desFilePath;
    if(file.copy(desFilePath)){


        //file.remove();
        return  desFilePath;
    }

    return "";
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
                msgId = tmpid;
            }
        }

    }
    return QString::number(msgId+1);
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
