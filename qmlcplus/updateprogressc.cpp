#include "updateprogressc.h"
using namespace std;
#define UNIT_KB 1024            //KB
#define UNIT_MB 1024*1024       //MB
#define UNIT_GB 1024*1024*1024  //GB
#define TIME_INTERVAL 300       //0.3s


updateProgressC::updateProgressC(QObject *parent) : QObject(parent)
{
    upcontrol =new CUpControl;
    connect(upcontrol,SIGNAL(signalFinishied(int)),this,SLOT(on_finishied(int)));
    connect(upcontrol,SIGNAL(signalError()),this,SLOT(on_err()));
    connect(upcontrol,SIGNAL(signalProgressUpdate(qint64 ,qint64)),this,SLOT(on_progress(qint64,qint64)));
}

void updateProgressC::on_err()
{
    qDebug()<<"send error"<<endl;
}

QString updateProgressC::getupToDid()
{
    return m_did;
}

void updateProgressC::startupLoad(QString did,QString url,QString file)
{

    qDebug()<<"url  "<<url<<"   "<<file;


    QTimer::singleShot(7*1000,this, SLOT(on_start()));

    m_did=did;
    m_url=url;
    m_filename=file;
}


void updateProgressC::on_start()
{
    if(!m_url.isEmpty()&&!m_filename.isEmpty())
    {
        upcontrol->startsendFile(m_url,m_filename);
    }
}
void updateProgressC::on_finishied(int status)
{
    if(status==200)
    {
        qDebug()<<"ok"<<endl;
        emit signal_updateState(true);
    }
    else
    {
        qDebug()<<"Faild"<<endl;
        emit signal_updateState(false);
    }

}
void updateProgressC::on_progress(qint64 bytessend,qint64 bytesTotal)
{

    //ui->progressBar->setMaximum(bytesTotal);
    //ui->progressBar->setValue(bytessend);

    //qDebug()<<"on_progress  "<<bytessend<<" "<<bytesTotal;
    if(bytesTotal == 0)
        return;
    int progressv = bytessend*100/bytesTotal;
    emit signal_updateProgress(progressv);
}
// 转换单位;
QString updateProgressC::transformUnit(qint64 bytes , bool isSpeed)
{
    QString strUnit = " B";
    if (bytes <= 0)
    {
        bytes = 0;
    }
    else if (bytes < UNIT_KB)
    {
    }
    else if (bytes < UNIT_MB)
    {
        bytes /= UNIT_KB;
        strUnit = " KB";
    }
    else if (bytes < UNIT_GB)
    {
        bytes /= UNIT_MB;
        strUnit = " MB";
    }
    else if (bytes > UNIT_GB)
    {
        bytes /= UNIT_GB;
        strUnit = " GB";
    }

    if (isSpeed)
    {
        strUnit += "/S";
    }
    QString tmp= QString("%1%2").arg(bytes).arg(strUnit);

    return tmp;
}

// 转换时间;
QString updateProgressC::transformTime(qint64 seconds)
{
    QString strValue;
    QString strSpacing(" ");
    if (seconds <= 0)
    {
        strValue = QString("%1s").arg(0);
    }
    else if (seconds < 60)
    {
        strValue = QString("%1s").arg(seconds);
    }
    else if (seconds < 60 * 60)
    {
        int nMinute = seconds / 60;
        int nSecond = seconds - nMinute * 60;

        strValue = QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else if (seconds < 60 * 60 * 24)
    {
        int nHour = seconds / (60 * 60);
        int nMinute = (seconds - nHour * 60 * 60) / 60;
        int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else
    {
        int nDay = seconds / (60 * 60 * 24);
        int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
        int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
        int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1d").arg(nDay);

        if (nHour > 0)
            strValue += strSpacing + QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }

    return strValue;

}
