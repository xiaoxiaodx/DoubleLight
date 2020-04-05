#include "youseeparse.h"
#include "debuglog.h"
#include "YoseenAlg.h"
#include "xvideotemp.h"
float YouSeeParse::temp_offset = TEMP_OFFSET;
float YouSeeParse::check_max_temp = CHECK_MAX_TEMP;
float YouSeeParse::check_min_temp = CHECK_MIN_TEMP;

FACEDATA_T faceCoordinates;
YouSeeParse::YouSeeParse(QObject *parent) : QObject(parent)
{


}

void YouSeeParse::slot_discover()
{
    //发现在线设备, 返回响应链表
    DiscoverCameraResp2* dcrList = Yoseen_DiscoverCameras(xxxdiscoverflags_broadcast);

    //迭代响应链表
    if (dcrList == NULL) {
        swprintf(_shellView.echo, 16, L"discover no cameras");
        DebugLog::getInstance()->writeLog("discover no cameras");
        return;
    }else {
        s32 i = 0;
        wchar_t* w = _shellView.echo;
        swprintf(w, 32, L"discover cameras, ");
        DebugLog::getInstance()->writeLog("discover cameras");

        w += wcslen(w);
        DiscoverCameraResp2* dcr = dcrList;
        while (dcr != NULL) {
            u32 ip = dcr->CameraIp;
            u8* p = (u8*)& ip;
            swprintf(w, 16, L"%d.%d.%d.%d, ", *(p + 3), *(p + 2), *(p + 1), *p);
            QString degstr = QString::number(*(p + 3))+"."+QString::number(*(p + 2))+"."+ QString::number(*(p + 1))+"."+ QString::number(*p);
            DebugLog::getInstance()->writeLog("红外地址:"+degstr);
            w += wcslen(w);
            dcr = dcr->pNext;
        }
    }

    if (NULL != dcrList) {
        YoseenLoginInfo& loginInfo = _shellView.loginInfo;
        u8* p = (u8*) &dcrList->CameraIp;
        snprintf(loginInfo.CameraAddr, 32, "%d.%d.%d.%d", *(p + 3), *(p + 2), *(p + 1), *p);
        slot_login();
    }


    //释放发现响应链表
    Yoseen_DiscoverCamerasFree(&dcrList);
}

void YouSeeParse::slot_login() {

    s32 userHandle = _shellView.userHandle;
    if (userHandle >= 0) {
        s32 ret = Yoseen_Logout(userHandle);
        _shellView.userHandle = -1;
        qDebug()<<"logout "<<ret;
    }
    else {
        DebugLog::getInstance()->writeLog("start login");
        YoseenLoginInfo& loginInfo = _shellView.loginInfo;
        CameraBasicInfo& cameraBasicInfo = _shellView.cameraBasicInfo;
        //wcstombs(loginInfo.CameraAddr, _shellView.url, 128);
        //        strcpy(loginInfo.Username, "admin");
        //        strcpy(loginInfo.Password, "admin");
        s32 userHandle = Yoseen_Login(&loginInfo, &cameraBasicInfo);
        if (userHandle >= 0) {
            _shellView.userHandle = userHandle;
            DebugLog::getInstance()->writeLog("Login Succ");
            slot_startPlay(xxxdatatype_temp);
        }else {
            DebugLog::getInstance()->writeLog("login fail "+userHandle);
        }
    }
}
/* * * * * * * * * * * * * * * * * * * *
 * 功能：  计算温度数据data中对应faceCoordinates.faceCoordinatesData区域（起始位置，宽，高）的温度值
 *
 * 参数说明：data： 温度数据
 *      width：   温度数据表 宽
 *      height：  温度数据表 高
 *      slope：   温度误差系数
 *      offset：  温度误差系数
 *
 * 注：温度浮点值：温度整数值/slope+offset，人脸像素坐标必须是连续的.
* * * * * * * * * * * * * * * * * * * */
#include <iostream>

/*
预览回调, 此回调触发在工作线程
*/
static int HotnessResetData(s16* IrdaDataFloat, int height, int width,char *p,u16 slop,s16 offset)
{
    int x = 0, y = 0, pI = 0;
    for (y = 0; y < height; y++) {
        for(x = 0 ; x < width; x++) {
            float value = IrdaDataFloat[y * width + x]/slop + offset;
            if ((value > YouSeeParse::check_min_temp) && (value < YouSeeParse::check_max_temp)) {
                p[pI] = 1;
            } else {
                p[pI] = 0;
            }
            pI++;
        }
    }
    return 0;
}

static CvSeq*cvSegmentFGMask( CvArr* _mask, bool poly1Hull0,float perimScale,
                              CvMemStorage* storage, CvPoint offset )
{
    CvMat mstub, *mask = cvGetMat( _mask, &mstub ,NULL,0);
    CvMemStorage* tempStorage = (storage ) ? storage: cvCreateMemStorage(0);
    CvSeq *contours, *c;
    int nContours = 0;
    CvContourScanner scanner;

    // clean up raw mask
    cvMorphologyEx( mask, mask, 0, 0, CV_MOP_OPEN, 1 );
    cvMorphologyEx( mask, mask, 0, 0, CV_MOP_CLOSE, 1 );
    // find contours around only bigger regions
    scanner = cvStartFindContours( mask, tempStorage,
                                   sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, offset );

    while( (c = cvFindNextContour( scanner )) != 0 )
    {
        double len = cvContourPerimeter( c );
        double q = (mask->rows + mask->cols)/perimScale; // calculate perimeter len threshold
        if( len < q ) //Get rid of blob if it's perimeter is too small
            cvSubstituteContour( scanner, 0 );
        else //Smooth it's edges if it's large enough
        {
            CvSeq* newC;

            if( poly1Hull0 ) //Polygonal approximation of the segmentation
                newC = cvApproxPoly( c, sizeof(CvContour), tempStorage, CV_POLY_APPROX_DP, 2, 0 );
            else //Convex Hull of the segmentation
                newC = cvConvexHull2( c, tempStorage, CV_CLOCKWISE, 1 );
            cvSubstituteContour( scanner, newC );
            nContours++;
        }
    }
    contours = cvEndFindContours( &scanner );

    cvZero( mask );
    //    for( c=contours; c != 0; c = c->h_next ){

    //        qDebug()<<"****11";
    //    }
    if( tempStorage != storage )
    {
        cvReleaseMemStorage( &tempStorage );
        contours = 0;
    }
    return contours;
}

static float getTempAavl(s16* IrdaDataFloat, int height, int width, u16 slop ,s16 offset , CvRect * rec) {
    float temp = 0.0;
    float value =0.0;
    int tempCnt = 0;
    float avl = 0.0;
    float max = 0.0,min = 41.0;
    static float *tempData = (float *)malloc(height*width * sizeof (float));
    int i = rec->x,j = rec->y;
    int x = rec->x, y = rec->y, w = rec->width, h = rec->height;
    signed short *innerData = IrdaDataFloat;

    for(j = y;j < (y + h);j ++)
    {
        for(i = x;i < (x+w);i++)
        {
            value = (((float)innerData[j * width + i])/slop + offset);
            if(value >= YouSeeParse::check_min_temp && value <= YouSeeParse::check_max_temp) {
                if(value > max)
                {
                    max = value;
                }

                if(value < min)
                {
                    min = value;
                }
                tempData[tempCnt] = value;
                temp += value;
                tempCnt++;
            }
        }
    }
    avl = temp/tempCnt;
    temp = 0;
    value =0;
    float tempCnt2 = 0;

    i = 0;
    for(i = 0;i < tempCnt;i++)
    {
        if(tempData[i] >= max || tempData[i] <= min)
        {
            continue;
        }
        temp += tempData[i];
        tempCnt2++;
    }

    avl = temp/tempCnt2;
    float coe = 0.2;
    if(YouSeeParse::check_min_temp >= 20 && YouSeeParse::check_min_temp < 25 )
    {
        coe = 0.2;
    }
    else if(YouSeeParse::check_min_temp >= 25 && YouSeeParse::check_min_temp < 35 )
    {
        coe = 0.3;
    }

    // qDebug()<<"1111avl: "<<avl;
    if(avl < (YouSeeParse::check_min_temp + TEMP_LEVEL) && avl >= YouSeeParse::check_min_temp)
    {
        avl = (avl - YouSeeParse::check_min_temp)*coe + 35.5;
    }
    else if(avl >= (YouSeeParse::check_min_temp + TEMP_LEVEL) && avl < (YouSeeParse::check_min_temp + TEMP_LEVEL + 1))
    {
        avl = (avl - (YouSeeParse::check_min_temp + TEMP_LEVEL)) + 36.5 + YouSeeParse::temp_offset;
    }
    else if(avl >= (YouSeeParse::check_min_temp + TEMP_LEVEL + 1))
    {
        avl = avl + TEMP_LEVEL + YouSeeParse::temp_offset;
    }
    else if(avl < YouSeeParse::check_min_temp)
    {
        qDebug()<<"avl ERROR: "<<avl;
    }
    //qDebug()<<"2222avl: "<<avl;
    return (avl);// + YouSeeParse::temp_offset
}



/*
预览回调, 此回调触发在工作线程
*/

IplImage* pFrame = NULL;
IplImage* pFrameSrc = NULL;
static void __stdcall _previewCallback(s32 errorCode, DataFrame* frame, void* customData) {
    if (YET_None == errorCode) {
        DataFrameHeader* tempHead = (DataFrameHeader*)frame->Head;
        s16* tempData = (s16*) frame->Temp;//得到温度 坐标 表（一个像素点表示一个温度值）
        if(!tempHead){
            qDebug()<<"解析失败";
            return;
        }

        static char *p = (char *)malloc(tempHead->Height*tempHead->Width);
        CvMemStorage*stor = NULL;
        CvSeq* cont = NULL;
        CvRect mdrects;
        HotnessResetData(tempData, tempHead->Width,tempHead->Height, p,tempHead->Slope,tempHead->Offset);

        if(pFrame == NULL)
            pFrame=cvCreateImageHeader(cvSize(tempHead->Width,tempHead->Height),IPL_DEPTH_8U,1);
        cvSetData(pFrame,p,tempHead->Width);

        stor = cvCreateMemStorage(0);
        cont=cvSegmentFGMask(pFrame,true,10.0,stor,cvPoint(0,0));

        if(pFrameSrc == NULL)
            pFrameSrc=cvCreateImageHeader(cvSize(tempHead->Width,tempHead->Height),IPL_DEPTH_32F,1);

        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX ,0.5, 0.5);
        cvSetData(pFrameSrc,(uchar*)frame->Bmp,tempHead->Width*4);

        ImageInfo *info = new ImageInfo;
        float maxAvgT = -1;
        for(int i=0;cont!=0;cont=cont->h_next,i++)
        {
            mdrects=((CvContour*)cont)->rect;
            float avgT = getTempAavl(tempData, tempHead->Height, tempHead->Width, tempHead->Slope, tempHead->Offset, &mdrects) ;
            if(mdrects.width < TEMP_MIN_INTERVAL && mdrects.height < TEMP_MIN_INTERVAL)
            {
                continue;
            }

            QMap<QString,QVariant> map;
            QRect rect(mdrects.x,mdrects.y,mdrects.width,mdrects.height);
            map.insert("rect",rect);
            map.insert("temp",avgT);
            info->listRect.append(map);
            if(maxAvgT == -1){
                maxAvgT = avgT;
            }else{
                if(avgT > maxAvgT){
                    maxAvgT = avgT;
                }
            }
        }



        try {
            info->pImg =  new QImage((uchar*)pFrameSrc->imageData, tempHead->Width, tempHead->Height, QImage::Format_RGB32);
            // 其它代码
        } catch ( const std::bad_alloc& e ) {
            DebugLog::getInstance()->writeLog("Yousee 图片分配内存失败");
            info->pImg = nullptr;
        }
        info->areaMaxtemp = maxAvgT;
        info->isDrawLine = true;

        QMutexLocker locker(&XVideoTemp::buffMutex);
        if(XVideoTemp::pBufferImginfo == nullptr)
            XVideoTemp::pBufferImginfo = info;
        else{
            if(info->pImg != nullptr)
                delete info->pImg;
            delete info;
        }
        cvReleaseMemStorage(&stor);
    }else {
        //接收数据失败, 预览内置自动恢复, YET_PreviewRecoverBegin-YET_PreviewRecoverEnd
        DebugLog::getInstance()->writeLog("recevie fail :PreviewCallback,errorCode "+errorCode);
    }
}

#include <QDateTime>
void YouSeeParse::slot_getInitPar()
{
    QMap<QString,QVariant> map;
    map.insert("cmd","getosdparam");
    slot_parSet(map);
    map.insert("cmd","setDate");
    slot_parSet(map);
}

bool YouSeeParse::slot_parSet(QMap<QString,QVariant> map)
{
    QString strcmd = map.value("cmd").toString();

    if(_shellView.userHandle < 0){
        DebugLog::getInstance()->writeLog(tr("设置参数时，句柄为空"));

        return false;
    }

    s32 ret =1;
    if(strcmd.compare("temOffset") == 0){
        float temp = map.value("value").toFloat();
        FixInfo fixInfo;
        Yoseen_GetFixInfo(_shellView.userHandle, &fixInfo);
        qDebug()<<"相对湿度:"<<fixInfo.RelativeHumidity<<"降雨强度:"<<fixInfo.RainfallIntensity<<", 降雪强度:"<<fixInfo.RainfallIntensity<<", 温度漂移:"<<fixInfo.TempOffset<<", TargetDistance:"<<fixInfo.TargetDistance<<", 大气压使能:"<<fixInfo.EnableAtmFix;
        fixInfo.TempOffset = temp;//温漂
        ret = Yoseen_SetFixInfo(_shellView.userHandle, &fixInfo);//previewInfo.Hwnd = NULL;
    }else if(strcmd.compare("getTemOffset")==0){
        FixInfo fixInfo;
        Yoseen_GetFixInfo(_shellView.userHandle, &fixInfo);
        qDebug()<<"相对湿度:"<<fixInfo.RelativeHumidity<<"降雨强度:"<<fixInfo.RainfallIntensity<<", 降雪强度:"<<fixInfo.RainfallIntensity<<", 温度漂移:"<<fixInfo.TempOffset<<", TargetDistance:"<<fixInfo.TargetDistance<<", 大气压使能:"<<fixInfo.EnableAtmFix;



    }else if(strcmd.compare("setosdparam")==0){
        bool enable = map.value("enable").toBool();


    }else if(strcmd.compare("getosdparam")==0){
        CameraOSDInfo osdinfo;
        ret = Yoseen_GetCameraOSDInfo(_shellView.userHandle, &osdinfo);

    }else if(strcmd.compare("setDate")==0){

        CtlX ctlx = {};

        QDateTime dateT = QDateTime::currentDateTime();
        QDate date = dateT.date();
        QTime time = dateT.time();
        ctlx.Type = CtlXType_SetTime;
        ctlx.Data.Time.year = 2004;//date.day()
        ctlx.Data.Time.mon = date.month();
        ctlx.Data.Time.day = date.day();
        ctlx.Data.Time.hour = time.hour();
        ctlx.Data.Time.min = time.minute();
        ctlx.Data.Time.sec = time.second();
        ret = Yoseen_SendControlX(_shellView.userHandle, &ctlx);
        ctlx.Type = CtlXType_GetTime;
        ret = Yoseen_SendControlX(_shellView.userHandle, &ctlx);
        qDebug()<<"========> year:"<<ctlx.Data.Time.year;
    }

    if(ret ==0 )
        return true;
    else{
        qDebug()<< "    yousee:"<<strcmd<<" "<<ret;
        return false;
    }
}
bool YouSeeParse::slot_setTemOffset(float temp){


    if(_shellView.userHandle >= 0){
        QMap<QString,QVariant> map;
        map.insert("cmd","temOffset");
        map.insert("value",temp);
        return slot_parSet(map);
    }else{
        DebugLog::getInstance()->writeLog(tr("温漂设置失败"));
        return false;
    }

}

void YouSeeParse::slot_startPlay(s32 dataType) {
    slot_stopPlay();


    YoseenPreviewInfo& previewInfo = _shellView.previewInfo;
    previewInfo.DataType = dataType;
    previewInfo.CustomData = &_shellView;
    previewInfo.CustomCallback = _previewCallback;
    previewInfo.Hwnd = NULL;
    s32 previewHandle = Yoseen_StartPreview(_shellView.userHandle, &previewInfo);

    qDebug()<<L"Yoseen_StartPreview, ret  "<<previewHandle;
    if (previewHandle < 0) {
        return;
    }
    _shellView.previewHandle = previewHandle;

    //
    //    RECT rect;
    //    GetClientRect(_shellView.hwndShell, &rect);
    //    ShellView_Layout(rect);
}

/*
预览开始和停止
*/
void YouSeeParse::slot_stopPlay() {
    s32 previewHandle = _shellView.previewHandle;
    if (previewHandle >= 0) {
        s32 ret = Yoseen_StopPreview(previewHandle);
        _shellView.previewHandle = -1;
        qDebug()<<L"Yoseen_StopPreview, ret "<<ret;
    }
}

void YouSeeParse::forceFinish(){

    QMutexLocker locker(&mMutex);
    isForceFinish = true;

    Yoseen_StopPreview(_shellView.previewHandle);

    Yoseen_Logout(_shellView.userHandle);

    Yoseen_FreeSDK();
}


void YouSeeParse::slot_init()
{
    int ret = Yoseen_InitSDK();
    if(ret==0){
        DebugLog::getInstance()->writeLog("init succ");
        _shellView.userHandle = -1;
        _shellView.previewHandle = -1;
        slot_discover();
    }else {
        DebugLog::getInstance()->writeLog("init fail");
    }
}

YouSeeParse::~YouSeeParse()
{
    qDebug()<<"YouSeeParse 析构";

    qDebug()<<"YouSeeParse 析构完成";
}


