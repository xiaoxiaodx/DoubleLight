#ifndef YOUSEEPARSE_H
#define YOUSEEPARSE_H

#include <QObject>
#include "YoseenSDK.h"
#include <QByteArray>
#include <QImage>
#include <QDebug>
#include <QMutex>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"

#define CHECK_MAX_TEMP (50)
#define CHECK_MIN_TEMP (28)
#define TEMP_OFFSET 0
#define TEMP_LEVEL  5.0
#define TEMP_MIN_INTERVAL 5

#define FACECOORDINATES_HOST ("192.168.0.103")
#define FACECOORDINATES_PORT (6000)
#define FACECOORDINATES_TIMEOUT (1000 * 1)

typedef struct _FACECOORDINATES_T {//人脸坐标
    short x;
    short y;
    short width;
    short height;

    //平均温度：temp/tmpCnt
    float temp; //人脸坐标区域的温度
    //short tempCnt;//人脸坐标区域的 人体温度计数
}FACECOORDINATES_T;
typedef struct _FACEDATA_T {
    FACECOORDINATES_T * faceCoordinatesData;
    int faceCoordinatesCnt;//检测到的人脸数量
}FACEDATA_T;

class RectInfo{
public:
    QRect rect;
    float temp;
};
class ImageInfo{
public:
    QImage *pImg;
    quint64 time;
    bool isDrawLine = false;

    QList<QVariantMap> listRect;
    float areaMaxtemp = 0;
};

typedef struct _ShellView {
    s32 flags;
    wchar_t url[128];
    wchar_t fn[256];
    wchar_t echo[256];

    s32 userHandle;							//用户句柄
    s32 previewHandle;						//预览句柄
    YoseenLoginInfo loginInfo;				//登入信息
    CameraBasicInfo cameraBasicInfo;	//热像仪基本信息
    YoseenPreviewInfo previewInfo;		//预览信息
}ShellView;
class YouSeeParse : public QObject
{
    Q_OBJECT
public:
    explicit YouSeeParse(QObject *parent = nullptr);
    ~YouSeeParse();
    void forceFinish();

    static float temp_offset ;
    static float check_max_temp ;
    static float check_min_temp ;

    static float getTempAavl(s16* IrdaDataFloat, int height, int width, u16 slop ,s16 offset , CvRect * rec);
    static int HotnessResetData(s16* IrdaDataFloat, int height, int width,char *p,u16 slop,s16 offset);
    static CvSeq* cvSegmentFGMask( CvArr* _mask, bool poly1Hull0,float perimScale,
                                  CvMemStorage* storage, CvPoint offset);
    static IplImage* pFrame;
    static IplImage* pFrameSrc;
signals:
    void signal_senImg(QImage *);
public slots:
    void slot_init();
    void slot_discover();
    void slot_login();
    void slot_startPlay(s32 dataType);
    void slot_stopPlay();
    bool slot_setTemOffset(float tem);

    void slot_getInitPar();
    bool slot_parSet(QMap<QString,QVariant>);

private:

    void InitTempArray();
    void UninitTempArray();

    QMutex mMutex;
    bool isForceFinish = false;
    ShellView _shellView = { 0 };

};

#endif // YOUSEEPARSE_H
