#ifndef UPDATEPROGRESSC_H
#define UPDATEPROGRESSC_H

#include <QObject>
#include "upcontrol.h"
class updateProgressC : public QObject
{
    Q_OBJECT
public:
    explicit updateProgressC(QObject *parent = nullptr);

    Q_INVOKABLE void startupLoad(QString did,QString url,QString file);

    QString getupToDid();


signals:
    void signal_updateState(bool isSucc);
    void signal_updateProgress(int progressvalue);
private slots:


private:
    CUpControl*upcontrol;
    QString m_did;
    QString m_url;
    QString m_filename;
   // QString m_url;
    QString transformUnit(qint64 bytes ,bool isSpeed);
    QString transformTime(qint64 seconds);
private slots:
    void on_err();
    void on_finishied(int);
    void on_progress(qint64,qint64);
    void on_start();
};

#endif // UPDATEPROGRESSC_H
