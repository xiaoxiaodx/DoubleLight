#include "replaytimeline.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QString>
ReplayTimeline::ReplayTimeline()
{

    replayCurrentTime.setHMS(0,0,0);

    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    rectFIndicator.setX(30);
    rectFIndicator.setY(34);
    rectFIndicator.setWidth(4);
    rectFIndicator.setHeight(20);
    init();
}
ReplayTimeline::~ReplayTimeline()
{

}

void ReplayTimeline::init()
{
    initTimeLineInterval();
}


void ReplayTimeline::slot_24hSelect()
{
    currentScaleType = TIMELINE24H;
    this->update();
}
void ReplayTimeline::slot_2hSelect()
{
    currentScaleType = TIMELINE2H;
    this->update();
}
void ReplayTimeline::slot_1hSelect()
{
    currentScaleType = TIMELINE1H;
    this->update();
}
void ReplayTimeline::slot_30mSelect()
{
    currentScaleType = TIMELINE30M;
    this->update();
}

void ReplayTimeline::initTimeLineInterval()
{
    {
        TimeInterval *timeInterval24h = new TimeInterval;
        timeInterval24h->type = TIMELINE24H;
        timeInterval24h->startTime = QTime(0,0,0);
        timeInterval24h->endTime = QTime(23,59,59,59);
        listTimeInterval24H.append(timeInterval24h);
    }
    for (int i=0;i<24;i+=2) {
        TimeInterval *timeInterval2h = new TimeInterval();
        timeInterval2h->type = TIMELINE2H;
        timeInterval2h->startTime = QTime(i,0,0);
        timeInterval2h->endTime = QTime(i+2,0,0);
        listTimeInterval2H.append(timeInterval2h);
    }

    for (int i=0;i<24;i++) {
        TimeInterval *timeInterval1h = new TimeInterval();
        timeInterval1h->type = TIMELINE1H;
        timeInterval1h->startTime = QTime(i,0,0);
        timeInterval1h->endTime = QTime(i+1,0,0);
        listTimeInterval1H.append(timeInterval1h);
    }

    //一个小时中有2个区间
    for (int i=0;i<24;i++) {
        TimeInterval *timeInterval30m1 = new TimeInterval();
        timeInterval30m1->type = TIMELINE30M;
        timeInterval30m1->startTime = QTime(i,0,0);
        timeInterval30m1->endTime = QTime(i,30,0);
        listTimeInterval30M.append(timeInterval30m1);

        TimeInterval *timeInterval30m2 = new TimeInterval;
        timeInterval30m2->type = TIMELINE30M;
        timeInterval30m2->startTime = QTime(i,30,0);
        timeInterval30m2->endTime = QTime(i+1,0,0);
        listTimeInterval30M.append(timeInterval30m2);
    }
}

TimeInterval* ReplayTimeline::findInterval(QList<TimeInterval*> &list,QTime &time)
{
    // qDebug()<<"当前值："<<time.toString("hh:mm:ss");
    for (int i=0;i<list.size();i++) {
        TimeInterval *timeI = list.at(i);
        QTime tmpStart = timeI->startTime;
        QTime tmpEnd = timeI->endTime;
        //qDebug()<<"区间值:"<<tmpStart.toString("hh:mm:ss")<<" "<<tmpEnd.toString("hh:mm:ss");
        if(time>= tmpStart && time<=tmpEnd)
            return timeI;
    }

    return  NULL;
}

void ReplayTimeline::paint(QPainter *painter)
{

    painter->setRenderHint(QPainter::Antialiasing);   //线条抗锯齿
    painter->setRenderHint(QPainter::TextAntialiasing);


    drawBg(painter);

}

void ReplayTimeline::drawBg(QPainter *painter){
    //时间轴所在的矩形
    QRectF rectTimeLine(30,34,this->width()-60,this->height()-54);
    //绘制大背景
    painter->setPen(QPen(QColor("#252525"),1));
    painter->setBrush(QBrush(QColor("#252525")));
    painter->drawRect(0,41,1920,113);

    //绘制时间轴背景
    painter->save();
    painter->setBrush(QBrush(QColor("#FFFFFF")));
    painter->drawRect(rectTimeLine);
    painter->restore();

    //绘制刻度
    drawScale(painter,currentScaleType);

}

void ReplayTimeline::setSizeType(int type)
{
    if(type == 0){
        currentScaleType = TIMELINE24H;
    }else if(type == 1)
        currentScaleType = TIMELINE2H;
    else if(type == 2)
        currentScaleType= TIMELINE1H;
    else if(type == 3)
        currentScaleType = TIMELINE30M;
    update();
}

void ReplayTimeline::drawScale(QPainter *painter,IntervalType type)
{
    //时间轴所在的矩形
    QRectF rectTimeLine(30,34,this->width()-60,this->height()-54);

    int splictlineCount ;
    qreal contentWidth ;
    //字体
    QFont newFont;
    newFont.setPixelSize(14);
    newFont.setFamily("Microsoft Yahei");
    QFontMetrics fontMetrics(newFont);
    painter->setFont(newFont);
    splictlineCount = 24-1;
    contentWidth = (rectTimeLine.width()-splictlineCount) / 24;

    TimeInterval *timeInterval = NULL;
    qreal valuePerScale;
    int markInterval;

    //在这里获取：回放时间点所在的区间，区间的起始位置，每刻度的长度（单位分钟）,每隔几个刻度显示一次标注
    if(type == TIMELINE24H){
        timeInterval = findInterval(listTimeInterval24H,replayCurrentTime);
        valuePerScale = 60;
        markInterval = 1;
    }else if (type == TIMELINE2H) {
        timeInterval = findInterval(listTimeInterval2H,replayCurrentTime);
        valuePerScale = 5;
        markInterval = 1;
    }else if (type == TIMELINE1H) {
        timeInterval = findInterval(listTimeInterval1H,replayCurrentTime);
        valuePerScale = 2.5;
        markInterval = 2;
    }else if (type == TIMELINE30M) {
        timeInterval = findInterval(listTimeInterval30M,replayCurrentTime);
        valuePerScale = 1.25;
        markInterval = 4;
    }

    if(timeInterval == NULL){
        qDebug()<<"未找到指定区间";
        return;
    }


    //绘制背景刻度
    QTime startT = timeInterval->startTime;
    for (int num = 0;num<25;num++) {
        qreal linex = rectTimeLine.x() + (contentWidth+1)*num;
        painter->drawLine(QPointF(linex,rectTimeLine.y()),QPointF(linex,rectTimeLine.y()+rectTimeLine.height()));

        if(num % markInterval != 0)
            continue;
        painter->save();
        painter->setPen(QPen(QColor("#ffffff"),1));
        //每刻度1.25分钟

        int min = num*valuePerScale;
        QTime desTime = startT.addSecs(min*60);

        QString showtimeStr = desTime.toString("hh:mm");
        if(min == 24*60)
            showtimeStr = "24:00";
        QRect rect = fontMetrics.boundingRect(showtimeStr);
        painter->drawText(linex - rect.width()/2,rectTimeLine.y()-10,showtimeStr);
        painter->restore();
    }

    qreal secsPerPix = getsecsPerPix();

    //绘制平铺图
    QTime endT = timeInterval->endTime;
    painter->save();
    for(int i=0;i<drawListInterval.size();i++){

        TimeInterval *timeInterval = drawListInterval.at(i);
        QTime tileTimeStart = timeInterval->startTime;
        QTime tileEndStart = timeInterval->endTime;
        qDebug()<<" time ***:"<<tileTimeStart<<"    "<<  tileEndStart;
        if(tileTimeStart>=startT && tileEndStart<=endT){
            qreal leftX = startT.secsTo(tileTimeStart)/secsPerPix;
            qreal rightX = startT.secsTo(tileEndStart)/secsPerPix;
            QRectF tmpRectF;
            tmpRectF.setX(leftX+30);
            tmpRectF.setY(35);
            tmpRectF.setWidth(rightX - leftX);
            tmpRectF.setHeight(18);
            painter->fillRect(tmpRectF,QBrush(QColor(59,132,246,128)));
        }

    }
    painter->restore();
    //绘制指示器
    int timeFromStart = startT.secsTo(replayCurrentTime);
    int dpix = (qreal)timeFromStart/secsPerPix;
    //  qDebug()<<"replayCurrentTime:"<<replayCurrentTime.fromMSecsSinceStartOfDay(<<"    startT:"<<startT;
    qDebug()<<"replayCurrentTime:"<<replayCurrentTime<<"    startT:"<<startT;
    qDebug()<<"timeFromStart:"<<timeFromStart<<"    secsPerPix:"<<secsPerPix;

    qreal resX = dpix + 30;
    qreal resY = rectFIndicator.y();
    qreal resW = rectFIndicator.width();
    qreal resH = rectFIndicator.height();
    rectFIndicator.setRect(resX,resY,resW,resH);
    painter->fillRect(rectFIndicator,QBrush(QColor(255,0,0)));

    //drawValue(painter);
}


void ReplayTimeline::drawValue(QPainter *painter)
{

    //painter->setPen(QPen(QBrush(QColor(255,0,0)),1));
    ;

}

int ReplayTimeline::getIndicatorTime()
{

    qDebug()<<"cureent time :"<<replayCurrentTime;

}
void ReplayTimeline::setDate(QDate date)
{

}

void ReplayTimeline::mousePressEvent(QMouseEvent* event){


    if(rectFIndicator.contains(event->pos())){
        //qDebug()<<  "mousePressEvent "<<rectFIndicator;
        isIndicatorPress = true;
        pressPt = event->pos();
        update();
    }

}
void ReplayTimeline::mouseMoveEvent(QMouseEvent *event){


}
qreal ReplayTimeline::getsecsPerPix()
{
    qreal totalInterval = this->width()-60;//时间总像素宽度
    qreal secsPerPix; //每个像素是多少秒
    if(currentScaleType == TIMELINE24H){
        secsPerPix = 24*60*60/totalInterval;
    }else if(currentScaleType == TIMELINE2H){
        secsPerPix = 2*60*60/totalInterval;
    }else if(currentScaleType == TIMELINE1H){
        secsPerPix = 60*60/totalInterval;
    }else if(currentScaleType == TIMELINE30M){
        secsPerPix = 30*60/totalInterval;
    }
    return secsPerPix;
}
void ReplayTimeline::mouseReleaseEvent(QMouseEvent *event){
    if(isIndicatorPress){
        // qDebug()<<  "mouseReleaseEvent "<<rectFIndicator;
        qreal dx = event->x() - pressPt.x();
        isIndicatorPress = false;
        qreal secsPerPix = getsecsPerPix();
        int dSecs = (int)(secsPerPix * dx);

        qDebug()<< "dx  "<<dx<<"    secs:"<<dSecs;

        qDebug()<<"mouseReleaseEvent replayCurrentTime:"<<replayCurrentTime;
        replayCurrentTime = replayCurrentTime.addSecs(dSecs);
        qDebug()<<"mouseReleaseEvent replayCurrentTime1:"<<replayCurrentTime;
        //getIndicatorTime();
        emit indicatorTimeChange(replayCurrentTime);
        update();
    }
}

void ReplayTimeline::updateDate(QString relativePath,QString date)
{
    qDebug()<<"updateDate:"<<relativePath<<"    "<<date;

     QDir dir(relativePath+"/"+date);

     if(!dir.exists()){

         qDebug()<<"文件路径不存在";
     }
     //设置文件过滤器
     QStringList nameFilters;
     //设置文件过滤格式
     nameFilters << "*.yuv";

     QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

     removeTime();
     for (int i=0;i<files.size();i++) {
         QString tmpName = files.at(i);
         QString fileName = tmpName.remove(".yuv");
         qDebug()<<"files name:"<<fileName;

         QStringList fileNames = fileName.split("_");
         if(fileNames.size() != 2)
             continue;

         QString timeStart = fileNames[0];
         QString longt = fileNames[1];
         appendTime(timeStart,longt);
     }
     update();
}
void ReplayTimeline::appendTime(QString startT,QString longt){

    int h = startT.mid(0,2).toInt();
    int m = startT.mid(2,2).toInt();
    int s = startT.mid(4,2).toInt();
    int t = longt.toInt();


    QTime time(h,m,s);
    TimeInterval *timeInterval = new TimeInterval;
    timeInterval->startTime = time;
    timeInterval->endTime = time.addSecs(t);
    timeInterval->sec = t;

    drawListInterval.append(timeInterval);
}

void ReplayTimeline::removeTime(){

    while(drawListInterval.size() >0){

        delete drawListInterval.takeFirst();
    }
}
