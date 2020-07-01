#include "warnmodel.h"
#include<QVariant>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "debuglog.h"
#include <QGuiApplication>
#include <QClipboard>

/*
    在报警时会往文件末尾写入一行警报信息，所以就意味着时间大的在文件的后面
    但是列表显示要将离当前时间近的排前面，也就是时间大的排前面
    所以每次将文件读取到的一行（小的时间点）都是插入在0行，已保证大的时间点在列表前面
    从list写入文件时都是倒着写入文件的
*/
WarnModel::WarnModel(QObject *parent ):QAbstractListModel(parent)
{

    //    m_listWarn.append(new WarnModelData(true,"19911103","1111111","sasads","aaaaa"));
    //    m_listWarn.append(new WarnModelData(true,"19911103","1111111","sasads","aaaaa"));
    //    m_listWarn.append(new WarnModelData(true,"19911103","1111111","sasads","aaaaa"));
    //    m_listWarn.append(new WarnModelData(true,"19911103","1111111","sasads","aaaaa"));
}

void WarnModel::setClipbord(QString str)
{
    QGuiApplication::clipboard()->setText(str,QClipboard::Clipboard);
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
    else if(role == Name)
        return QVariant::fromValue(m_listWarn.at(index.row())->name());
    else if(role == Numbering)
        return QVariant::fromValue(m_listWarn.at(index.row())->numering());
    else if(role == MaskRecognition)
        return QVariant::fromValue(m_listWarn.at(index.row())->maskRecognition());
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
    roles[MaskRecognition]="maskRecognition";
    roles[Numbering]="numbering";
    roles[Name]="name";
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
        else if(role == Name)
            modelData->setName(value.toString());
        else if(role == Numbering)
            modelData->setNumering(value.toString());
        else if(role == MaskRecognition)
            modelData->setMaskRecognition(value.toString());
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

        //        QFile::remove(date->absolutePath());
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
    QString fileAbsolutePath = date->absolutePath();
    QFile::remove(fileAbsolutePath);
    delete date;
    endRemoveRows();

    if(curPath == ""||curDate=="")
        return;

    //日志文件的绝对路径，重写文件,倒序写入文件
    QString logAbsolutePath = curPath+"/log/"+curDate+".log";
    QFile file(logAbsolutePath);
    if(file.open(QIODevice::WriteOnly)){
        for (int i=m_listWarn.size()-1;i>=0;i--) {
            WarnModelData *modelData = m_listWarn.at(i);
            QString imgInfoStr = modelData->absolutePath()+" "+modelData->warnTemp()+" "+modelData->imgName()+".jpeg";
            QTextStream out(&file);
            out <<imgInfoStr << "\n";
        }
        file.close();
    }
}

void WarnModel::funFlushWarnInfo(QString capturePath,QString logFileName)//这里文件名就是日期"yyMMdd"
{
    qDebug()<<"logPath:"<<capturePath<<"    logFileName"<<logFileName;


    //同一天的数据则不进行刷新，会在列表中添加完后在进行写文件
    if(curDate.compare(logFileName)==0)
        return;


    curDate = logFileName;
    curPath = capturePath;

    //不是同一天则移除原有数据
    removeAll();
    QString logAbsolutePath = capturePath+"/log/"+logFileName+".log";
    QFile file(logAbsolutePath);
    //每次读取都是往0位置插入值
    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        QString warnStr = in.readLine();

        while (!warnStr.isNull()) {
            QStringList strlist = warnStr.split(" ");
            //qDebug()<<"strlist:"<<strlist;
            if(strlist.size() != 6){
                DebugLog::getInstance()->writeLog("读取警报日志数据异常"+strlist.size());
                break;
            }

               //  QString imgInfoStr = imgAbsolutePath1+" "+tempStr+" "+name+" "+number+" "+QString::number(alarmtype)+" "+curDatetimeStr+".jpeg";

            QString absolutepath = strlist[0];
            QString fileName = strlist[5];
            QString temp = strlist[1];
            QString name = strlist[2];
            QString number = strlist[3];
            QString type = strlist[4];

            QString datetime = fileName.remove(".jpeg");
            QStringList datetStr = datetime.split("_");
            if(datetStr.size() != 2){
                DebugLog::getInstance()->writeLog("对日期和时间区分时数据异常"+datetStr.size());
                break;
            }
            QString date = datetStr[0].mid(0,4)+"-"+datetStr[0].mid(4,2)+"-"+datetStr[0].mid(6,2);
            QString time = datetStr[1].mid(0,2)+":"+datetStr[1].mid(2,2)+":"+datetStr[1].mid(4,2);

            m_listWarn.insert(0,new WarnModelData(false,date+" "+time,temp,fileName,absolutepath,name,number,type=="82"?"0":"1"));
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

    QTime needfindTime(h.toInt(),m.toInt(),s.toInt());
    int findIndex = -1;
    int minDeT = 3600*24;
    if(m_listWarn.size() > 0){
        for (int i=m_listWarn.size()-1;i>=0;i--) {
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
            QTime curtime(timeH,timeM,timeS);

            int tmpDt = curtime.secsTo(needfindTime);
            int absTmpDt = abs(tmpDt);
            if(absTmpDt==0){
                return i;
            }

            if(absTmpDt < minDeT){
                minDeT = absTmpDt;
                findIndex = i;
            }
        }
    }

    qDebug()<<" findIndex:"<<findIndex;
    return findIndex;
}

void WarnModel::funSetInitSelectFalse()
{
    curSelect = false;
}

void WarnModel::funSetAllSelect(bool isSelect)
{
    curSelect = isSelect;
    beginResetModel();
    for (int i=0;i<m_listWarn.size();i++) {
        m_listWarn.at(i)->setIsSelect(isSelect);
    }
    endResetModel();
}



void WarnModel::funDeleteSelect(){
    beginResetModel();
    for (int i=m_listWarn.size()-1;i>=0;i--) {
        if(m_listWarn.at(i)->isSelect()){
            WarnModelData* modeldate = m_listWarn.takeAt(i);
            QString fileAbsolutePath = modeldate->absolutePath();
            QFile::remove(fileAbsolutePath);
            delete modeldate;

        }
    }
    curSelect = false;
    endResetModel();



    //日志文件的绝对路径，重写文件,倒序写入文件
    QString logAbsolutePath = curPath+"/log/"+curDate+".log";
    QFile file(logAbsolutePath);
    if(file.open(QIODevice::WriteOnly)){
        for (int i=m_listWarn.size()-1;i>=0;i--) {
            WarnModelData *modelData = m_listWarn.at(i);
            QString imgInfoStr = modelData->absolutePath()+" "+modelData->warnTemp()+" "+modelData->imgName()+".jpeg";
            QTextStream out(&file);
            out <<imgInfoStr << "\n";
        }
        file.close();
    }
}
//hardisk 第二次需求变更,存人脸检测图片，发送给qml显示，同时存版本发来的抓拍，并存日志列表
void WarnModel::funProcessPushAlarm2(QString path,QVariantMap map){

    if(flagI > 100000)
        flagI = 0;

    qDebug()<<"funProcessPushAlarm******** :"<<map.value("year").toInt() <<"    "<<map.value("month").toInt()<<"    "<<map.value("day").toInt()<<"  name:"<<map.value("name").toString();
    QDate tmpDate(map.value("year").toInt(),map.value("month").toInt(),map.value("day").toInt());
    QTime tmptime(map.value("hour").toInt(),map.value("min").toInt(),map.value("sec").toInt());
    QString name = map.value("name").toString();
    QString number = map.value("seq").toString();
    float warnTemp = map.value("temperature").toFloat();
    int alarmtype = map.value("alarmtype").toInt();

    QString datestr = tmpDate.toString("yyyyMMdd");

    QDateTime curDateTime(tmpDate,tmptime);

    QString  curDatetimeStr = curDateTime.toString("yyyyMMdd_hhmmss");

    QString desFileDir = path+"/image1";

    QString imgAbsolutePath = path+"/image1/"+curDatetimeStr+".jpeg";

    qDebug()<<" tmpDate "<<tmpDate<<"   tmptime"<<tmptime;

    QDir dir;
    if (!dir.exists(desFileDir)){
        bool res = dir.mkpath(desFileDir);
        if(res)
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is succ");
        else
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is fail");
    }

    //创建相对路径
    if(!QDir::setCurrent(desFileDir)){
        DebugLog::getInstance()->writeLog("slot_screenShot set relative dir is false");
        return;
    }

    if(map.contains("imagedata")){

        QFileInfo fileInfo(imgAbsolutePath);
        if(fileInfo.isFile()){
            imgAbsolutePath =  path+"/image1/"+QString::number(flagI)+"_"+curDatetimeStr+".jpeg";
            flagI++;
        }

        QString imgData = map.value("imagedata").toString();
        QByteArray imgArrBase64 = imgData.toLatin1();
        QByteArray imgArr = QByteArray::fromBase64(imgArrBase64);
        QFile file(imgAbsolutePath);
        if(file.open(QIODevice::WriteOnly)){
            file.write(imgArr,imgArr.length());

            qDebug()<<"funProcessPushAlarm******** file open:"+imgAbsolutePath;
            file.close();
            //存完文件 将消息发给qml
            emit signal_sendWarnMsg(alarmtype,imgAbsolutePath,curDateTime.toString("yyyy-MM-dd hh:mm:ss"),warnTemp,name,number);
        }else {
            DebugLog::getInstance()->writeLog("slot_screenShot open log file is fail");
            return ;
        }

    }else{
        qDebug()<<"不包含  imagedata";
    }


    //抓拍
    if(!map.contains("snapimagedata"))
        return;

    qDebug()<<"snapimagedata    ";
    QString snapimagedata = map.value("snapimagedata").toString();

    QByteArray snapimgArrBase64 = snapimagedata.toLatin1();
    QByteArray snapimgArr = QByteArray::fromBase64(snapimgArrBase64);

    QString desFileDir1 = path+"/image";

    QDir dir1;
    if (!dir1.exists(desFileDir1)){
        bool res = dir1.mkpath(desFileDir1);
        if(res)
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is succ");
        else
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is fail");
    }
    QString imgAbsolutePath1 = path+"/image/"+curDatetimeStr+".jpeg";

    QFile file1(imgAbsolutePath1);
    if(file1.open(QIODevice::WriteOnly)){
        file1.write(snapimgArr,snapimgArr.length());
        file1.close();

        //存报警图片信息
        /*  告警名录下 建立一个日志文件夹 一个图片文件夹，
                    日志文件夹下放30个日志文件，一个日志文件代表一天，
                    图片文件夹下放入告警抓拍图片
                */
        QString warnLogAbsolutePath = path + "/log";
        QString warnLogAbsoluteFileName = warnLogAbsolutePath + "/"+curDateTime.date().toString("yyyyMMdd")+".log";
        if(!dir.exists(warnLogAbsolutePath)){
            bool res = dir.mkpath(warnLogAbsolutePath);
            if(res)
                DebugLog::getInstance()->writeLog("slot_screenShot create new log dir is succ");
            else
                DebugLog::getInstance()->writeLog("slot_screenShot create new log dir is fail");
        }

        /*  抓拍是一种频繁的操作 ，为了优化性能，
            在抓拍时往文件尾写数据，因为抓拍时间都是顺序后延的
            同时往数据模型前添加数据
        */
        QFile imgInfofile(warnLogAbsoluteFileName);
        if(imgInfofile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            QString tempStr = QString::number(warnTemp,'f',2);
            QString imgInfoStr = imgAbsolutePath1+" "+tempStr+" "+name+" "+number+" "+QString::number(alarmtype)+" "+curDatetimeStr+".jpeg";
            QTextStream out(&imgInfofile);
            out <<imgInfoStr << "\n";
            imgInfofile.close();

            QString date = curDateTime.toString("yyyy-MM-dd");
            QString time = curDateTime.toString("hh:mm:ss");

            //是当前日期才加入列表日志
            if(curDate.compare(datestr)==0){
                beginInsertRows(QModelIndex(),0,0);
                m_listWarn.insert(0,new WarnModelData(curSelect,date+" "+time,tempStr,curDatetimeStr,imgAbsolutePath1,name,number,alarmtype == 82?"0":"1"));
                endInsertRows();
            }

        }else {
            DebugLog::getInstance()->writeLog("slot_screenShot open log file is fail");
            return ;
        }
    }

}

//hardisk 第一次需求变更,存人脸检测图片，并保存本地
void WarnModel::funProcessPushAlarm1(QString path,QVariantMap map){

    qDebug()<<"funProcessPushAlarm********  "<<map.value("year").toInt() <<"    "<<map.value("month").toInt()<<"    "<<map.value("day").toInt();
    QDate tmpDate(map.value("year").toInt(),map.value("month").toInt(),map.value("day").toInt());
    QTime tmptime(map.value("hour").toInt(),map.value("min").toInt(),map.value("sec").toInt());
    float warnTemp = map.value("temperature").toFloat();
    int alarmtype = map.value("alarmtype").toInt();
    QString imgData = map.value("imagedata").toString();



    QString datestr = tmpDate.toString("yyyyMMdd");



    QByteArray imgArrBase64 = imgData.toLatin1();
    QByteArray imgArr = QByteArray::fromBase64(imgArrBase64);


    QDateTime curDateTime(tmpDate,tmptime);

    QString  curDatetimeStr = curDateTime.toString("yyyyMMdd_hhmmss");

    QString desFileDir = path+"/image1";

    QString imgAbsolutePath = path+"/image1/"+curDatetimeStr+".jpeg";

    qDebug()<<" tmpDate "<<tmpDate<<"   tmptime"<<tmptime;

    QDir dir;
    if (!dir.exists(desFileDir)){
        bool res = dir.mkpath(desFileDir);
        if(res)
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is succ");
        else
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is fail");
    }

    //创建相对路径
    if(!QDir::setCurrent(desFileDir)){
        DebugLog::getInstance()->writeLog("slot_screenShot set relative dir is false");
        return;
    }


    QFile file(imgAbsolutePath);
    if(file.open(QIODevice::WriteOnly)){
        file.write(imgArr,imgArr.length());

        qDebug()<<"funProcessPushAlarm******** file open:"+imgAbsolutePath;
        file.close();
        //存完文件 将消息发给qml
      //  emit signal_sendWarnMsg(alarmtype,imgAbsolutePath,curDateTime.toString("yyyy-MM-dd hh:mm:ss"),warnTemp,name,);
    }else {
        DebugLog::getInstance()->writeLog("slot_screenShot open log file is fail");
        return ;
    }
}

//hardisk 第一次需求
void WarnModel::funProcessPushAlarm(QString path,QVariantMap map)
{

    qDebug()<<"funProcessPushAlarm********  "<<map.value("year").toInt() <<"    "<<map.value("month").toInt()<<"    "<<map.value("day").toInt();
    QDate tmpDate(map.value("year").toInt(),map.value("month").toInt(),map.value("day").toInt());
    QTime tmptime(map.value("hour").toInt(),map.value("min").toInt(),map.value("sec").toInt());
    float warnTemp = map.value("temperature").toFloat();
    int alarmtype = map.value("alarmtype").toInt();
    QString imgData = map.value("snapimagedata").toString();



    QString datestr = tmpDate.toString("yyyyMMdd");



    QByteArray imgArrBase64 = imgData.toLatin1();
    QByteArray imgArr = QByteArray::fromBase64(imgArrBase64);


    QDateTime curDateTime(tmpDate,tmptime);

    QString  curDatetimeStr = curDateTime.toString("yyyyMMdd_hhmmss");

    QString desFileDir = path+"/image";

    QString imgAbsolutePath = path+"/image/"+curDatetimeStr+".jpeg";

    qDebug()<<" tmpDate "<<tmpDate<<"   tmptime"<<tmptime;

    QDir dir;
    if (!dir.exists(desFileDir)){
        bool res = dir.mkpath(desFileDir);
        if(res)
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is succ");
        else
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is fail");
    }

    //创建相对路径
    if(!QDir::setCurrent(desFileDir)){
        DebugLog::getInstance()->writeLog("slot_screenShot set relative dir is false");
        return;
    }


    QFile file(imgAbsolutePath);
    if(file.open(QIODevice::WriteOnly)){
        file.write(imgArr,imgArr.length());

        qDebug()<<"funProcessPushAlarm******** file open:"+imgAbsolutePath;
        file.close();

        //emit signal_sendWarnMsg(alarmtype,imgAbsolutePath,curDateTime.toString("yyyy-MM-dd hh:mm:ss"),warnTemp);
        //        if(alarmtype != 80){//不是超温告警  不记录log信息
        //            return;
        //        }

        //存报警图片信息
        /*  告警名录下 建立一个日志文件夹 一个图片文件夹，
                    日志文件夹下放30个日志文件，一个日志文件代表一天，
                    图片文件夹下放入告警抓拍图片
                */
        QString warnLogAbsolutePath = path + "/log";
        QString warnLogAbsoluteFileName = warnLogAbsolutePath + "/"+curDateTime.date().toString("yyyyMMdd")+".log";
        if(!dir.exists(warnLogAbsolutePath)){
            bool res = dir.mkpath(warnLogAbsolutePath);
            if(res)
                DebugLog::getInstance()->writeLog("slot_screenShot create new log dir is succ");
            else
                DebugLog::getInstance()->writeLog("slot_screenShot create new log dir is fail");
        }

        /*  抓拍是一种频繁的操作 ，为了优化性能，
            在抓拍时往文件尾写数据，因为抓拍时间都是顺序后延的
            同时往数据模型前添加数据
        */
        QFile imgInfofile(warnLogAbsoluteFileName);
        if(imgInfofile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            QString tempStr = QString::number(warnTemp,'f',2);
            QString imgInfoStr = imgAbsolutePath+" "+tempStr+" "+curDatetimeStr+".jpeg";
            QTextStream out(&imgInfofile);
            out <<imgInfoStr << "\n";
            imgInfofile.close();

            QString date = curDateTime.toString("yyyy-MM-dd");
            QString time = curDateTime.toString("hh:mm:ss");

            //是当前日期才加入列表日志
            if(curDate.compare(datestr)==0){
                beginInsertRows(QModelIndex(),0,0);
                //m_listWarn.insert(0,new WarnModelData(curSelect,date+" "+time,tempStr,curDatetimeStr,imgAbsolutePath));
                endInsertRows();
            }

        }else {
            DebugLog::getInstance()->writeLog("slot_screenShot open log file is fail");
            return ;
        }
    }

}

bool WarnModel::funScreenShoot(QString path,QQuickWindow *quic,int capx,int capy,int capw,int caph,float warnTemp)
{

    QString datestr = QDate::currentDate().toString("yyyyMMdd");
    qDebug()<<"*********:"<<path<<" "<<datestr<<"   "<<curDate;

    QImage img= quic->grabWindow();

    if(path == ""){
        DebugLog::getInstance()->writeLog("scrennshot file path is null");
        return false;
    }



    QImage img1 = img.copy( capx, capy, capw, caph);
    //QImage img2 = img1.scaled(QSize(960,600),Qt::IgnoreAspectRatio);

    QDateTime curDateTime =  QDateTime::currentDateTime();
    QString  curDatetimeStr = curDateTime.toString("yyyyMMdd_hhmmss");

    QString desFileDir = path+"/image";

    QString imgAbsolutePath = path+"/image/"+curDatetimeStr+".jpeg";

    QDir dir;
    if (!dir.exists(desFileDir)){
        bool res = dir.mkpath(desFileDir);
        if(res)
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is succ");
        else
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is fail");
    }

    //创建相对路径
    if(!QDir::setCurrent(desFileDir)){
        DebugLog::getInstance()->writeLog("slot_screenShot set relative dir is false");
        return false;
    }


    if(!img1.save(curDatetimeStr+".jpeg","JPEG")){
        DebugLog::getInstance()->writeLog("scrennshot save fail");
        return false;
    }else{
        //存报警图片信息
        /*  告警名录下 建立一个日志文件夹 一个图片文件夹，
                    日志文件夹下放30个日志文件，一个日志文件代表一天，
                    图片文件夹下放入告警抓拍图片
                */
        QString warnLogAbsolutePath = path + "/log";
        QString warnLogAbsoluteFileName = warnLogAbsolutePath + "/"+curDateTime.date().toString("yyyyMMdd")+".log";
        if(!dir.exists(warnLogAbsolutePath)){
            bool res = dir.mkpath(warnLogAbsolutePath);
            if(res)
                DebugLog::getInstance()->writeLog("slot_screenShot create new log dir is succ");
            else
                DebugLog::getInstance()->writeLog("slot_screenShot create new log dir is fail");
        }

        /*  抓拍是一种频繁的操作 ，为了优化性能，
            在抓拍时往文件尾写数据，因为抓拍时间都是顺序后延的
            同时往数据模型前添加数据
        */
        QFile imgInfofile(warnLogAbsoluteFileName);
        if(imgInfofile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            QString tempStr = QString::number(warnTemp,'f',2);
            QString imgInfoStr = imgAbsolutePath+" "+tempStr+" "+curDatetimeStr+".jpeg";
            QTextStream out(&imgInfofile);
            out <<imgInfoStr << "\n";
            imgInfofile.close();

            QString date = curDateTime.toString("yyyy-MM-dd");
            QString time = curDateTime.toString("hh:mm:ss");

            //是当前日期才加入列表日志
            if(curDate.compare(datestr)==0){
                beginInsertRows(QModelIndex(),0,0);
               // m_listWarn.insert(0,new WarnModelData(curSelect,date+" "+time,tempStr,curDatetimeStr,imgAbsolutePath));
                endInsertRows();
            }

        }else {
            DebugLog::getInstance()->writeLog("slot_screenShot open log file is fail");
            return false;
        }
    }
    return true;
}

bool WarnModel::funScreenShoot1(QString path,QQuickWindow *quic,int capx,int capy,int capw,int caph,float warnTemp,int tempType)
{

    QString datestr = QDate::currentDate().toString("yyyyMMdd");
    qDebug()<<"*********:"<<path<<" "<<datestr<<"   "<<curDate;

    QImage img= quic->grabWindow();

    if(path == ""){
        DebugLog::getInstance()->writeLog("scrennshot file path is null");
        return false;
    }



    QImage img1 = img.copy( capx, capy, capw, caph);
    //QImage img2 = img1.scaled(QSize(960,600),Qt::IgnoreAspectRatio);

    QDateTime curDateTime =  QDateTime::currentDateTime();
    QString  curDatetimeStr = curDateTime.toString("yyyyMMdd_hhmmss");


    QString typeStr = "";

    if(tempType == 80)
        typeStr = "Alarm";
    else if(tempType == 81)
        typeStr = "Normal";
    else if(tempType == 82)
        typeStr = "NoMask";

    QString desFileDir = path+"/image/"+typeStr;

    QString imgAbsolutePath = desFileDir+"/"+curDatetimeStr+".jpeg";

    QDir dir;
    if (!dir.exists(desFileDir)){
        bool res = dir.mkpath(desFileDir);
        if(res)
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is succ");
        else
            DebugLog::getInstance()->writeLog("slot_screenShot create new dir is fail");
    }

    //创建相对路径
    if(!QDir::setCurrent(desFileDir)){
        DebugLog::getInstance()->writeLog("slot_screenShot set relative dir is false");
        return false;
    }


    if(!img1.save(curDatetimeStr+".jpeg","JPEG")){
        DebugLog::getInstance()->writeLog("scrennshot save fail");
        return false;
    }else{
        //存报警图片信息
        /*  告警名录下 建立一个日志文件夹 一个图片文件夹，
                    日志文件夹下放30个日志文件，一个日志文件代表一天，
                    图片文件夹下放入告警抓拍图片
                */
        QString warnLogAbsolutePath = path + "/log";
        QString warnLogAbsoluteFileName = warnLogAbsolutePath + "/"+curDateTime.date().toString("yyyyMMdd")+".log";
        if(!dir.exists(warnLogAbsolutePath)){
            bool res = dir.mkpath(warnLogAbsolutePath);
            if(res)
                DebugLog::getInstance()->writeLog("slot_screenShot create new log dir is succ");
            else
                DebugLog::getInstance()->writeLog("slot_screenShot create new log dir is fail");
        }

        /*  抓拍是一种频繁的操作 ，为了优化性能，
            在抓拍时往文件尾写数据，因为抓拍时间都是顺序后延的
            同时往数据模型前添加数据
        */
        QFile imgInfofile(warnLogAbsoluteFileName);
        if(imgInfofile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            QString tempStr = QString::number(warnTemp,'f',2);
            QString imgInfoStr = imgAbsolutePath+" "+tempStr+" "+curDatetimeStr+".jpeg";
            QTextStream out(&imgInfofile);
            out <<imgInfoStr << "\n";
            imgInfofile.close();

            QString date = curDateTime.toString("yyyy-MM-dd");
            QString time = curDateTime.toString("hh:mm:ss");

            //是当前日期才加入列表日志
            if(curDate.compare(datestr)==0){
                beginInsertRows(QModelIndex(),0,0);
               // m_listWarn.insert(0,new WarnModelData(curSelect,date+" "+time,tempStr,curDatetimeStr,imgAbsolutePath));
                endInsertRows();
            }

        }else {
            DebugLog::getInstance()->writeLog("slot_screenShot open log file is fail");
            return false;
        }
    }
    return true;
}
