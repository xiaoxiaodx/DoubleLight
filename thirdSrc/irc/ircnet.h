#ifndef IRCNET_H
#define IRCNET_H

#include <QObject>
#include "windows.h"
#include "IRNet.h"
class IRCNet : public QObject
{
    Q_OBJECT
public:
    explicit IRCNet(QObject *parent = nullptr);
    void ircInit();
signals:

public slots:

private:

//    void rawCallBackFunc(char * data, int width, int height, void * context);

    CHANNEL_CLIENTINFO clientInfo;
    IRNETHANDLE	m_handleClientStart2;

};

#endif // IRCNET_H
