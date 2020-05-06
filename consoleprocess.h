#ifndef CONSOLEPROCESS_H
#define CONSOLEPROCESS_H

#include <QObject>
#include <QProcess>
#include <QFile>
class ConsoleProcess : public QObject
{
    Q_OBJECT

    Q_INVOKABLE void funStartCmd(QString cmd);

public:
    explicit ConsoleProcess(QObject *parent = nullptr);


signals:

public slots:
    void slot_output();

private:
    QProcess  m_Process;

};

#endif // CONSOLEPROCESS_H
