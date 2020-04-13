#ifndef IRCNET_H
#define IRCNET_H

#include <QObject>
#include "windows.h"
#include "IRNet.h"
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core_c.h"
//#include "opencv2/imgproc/imgproc_c.h"

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
    IRNETHANDLE	m_handleClientStart;
    IRNETHANDLE	m_handleClientStart2;
    //IplImage* YUV420_To_IplImage_Opencv(char* pYUV420, int width, int height);

};

#endif // IRCNET_H
