#ifndef TOOLUTIL_H
#define TOOLUTIL_H

#include <QObject>
#include <QProcess>
class toolUtil : public QObject
{
    Q_OBJECT
    Q_INVOKABLE void funStartCmd(QString cmd);
    Q_INVOKABLE void readDidFile(QString str);
    QStringList myDidList;




public:
    explicit toolUtil(QObject *parent = nullptr);

signals:

    void signal_setDidInfo(QString did,QString p2pkey,QString pushKey);
public slots:

    void slot_consoleOutput();
private:
    QProcess  m_Process;
    void setWriteDidLabel();
};

#endif // TOOLUTIL_H
