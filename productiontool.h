#ifndef PRODUCTIONTOOL_H
#define PRODUCTIONTOOL_H

#include <QObject>
#include <QProcess>
class ProductionTool : public QObject
{
    Q_OBJECT
    Q_INVOKABLE void funStartCmd(QString cmd);
    Q_INVOKABLE void readDidFile(QString str);
public:
    explicit ProductionTool(QObject *parent = nullptr);

    QStringList myDidList;
signals:
public slots:

    void slot_consoleOutput();

private:
    QProcess  m_Process;

};

#endif // PRODUCTIONTOOL_H
