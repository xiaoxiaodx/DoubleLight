#ifndef TOOLUTIL_H
#define TOOLUTIL_H

#include <QObject>
#include <QProcess>
class ToolUtil : public QObject
{
    Q_OBJECT
public:



    explicit ToolUtil(QObject *parent = nullptr);
    Q_INVOKABLE void setWriteDidLabel();
    Q_INVOKABLE void funStartCmd(QString cmd);
    Q_INVOKABLE void readDidFile(QString str);
    Q_INVOKABLE void setDidLisence(QString did,QString lisence);
    Q_INVOKABLE void deleteFirstDid();


    QStringList myDidList;
signals:
    void signal_setDidInfo(QString did,QString p2pkey,QString pushKey);
    void signal_sendLisence(QString lisence);
    void signal_tip(bool isSucc,QString str);
public slots:

    void slot_consoleOutput();
private:
    QProcess  m_Process;
    QString didFilePath;

    bool isWriteLisence = false;
};

#endif // TOOLUTIL_H
