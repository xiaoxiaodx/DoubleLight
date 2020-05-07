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
    QStringList myDidList;
signals:

    void signal_setDidInfo(QString did,QString p2pkey,QString pushKey);
    void signal_sendLisence(QString lisence);
public slots:

    void slot_consoleOutput();
private:
    QProcess  m_Process;

};

#endif // TOOLUTIL_H
