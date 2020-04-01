#include "replaytimeline.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>

ReplayTimeline::ReplayTimeline()
{

    replayCurrentTime.setHMS(14,10,15);

//    setAcceptHoverEvents(true);
//    setAcceptedMouseButtons(Qt::AllButtons);
//    setFlag(ItemAcceptsInputMethod, true);

    rectFIndicator.setX(30);
    rectFIndicator.setY(34);
    rectFIndicator.setWidth(4);
    rectFIndicator.setHeight(20);
    init();
}
ReplayTimeline::~ReplayTimeline()
{
    delete ui;
}

void ReplayTimeline::init()
{
    initTimeLineInterval();
}


void ReplayTimeline::slot_24hSelect()
{
    scaleType = TIMELINE24H;
    this->update();
}
void ReplayTimeline::slot_2hSelect()
{
    scaleType = TIMELINE2H;
    this->update();
}
void ReplayTimeline::slot_1hSelect()
{
    scaleType = TIMELINE1H;
    this->update();
}
void ReplayTimeline::slot_30mSelect()
{
    scaleType = TIMELINE30M;
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
    drawScale(painter,scaleType);

}

void ReplayTimeline::setSizeType(int type)
{
    if(type == 0){
        scaleType = TIMELINE24H;
    }else if(type == 1)
        scaleType = TIMELINE2H;
    else if(type == 2)
        scaleType= TIMELINE1H;
    else if(type == 3)
        scaleType = TIMELINE30M;
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
        QTime time;
        if(min<60){
            time.setHMS(startT.hour(),min,0);
        }else {
            int divisorH = min/60;  //除数
            int remainderH = min%60;//余数
            time.setHMS(startT.hour()+divisorH,remainderH,0);
        }

        QString showtimeStr = time.toString("hh:mm");
        if(min == 24*60)
            showtimeStr = "24:00";
        QRect rect = fontMetrics.boundingRect(showtimeStr);
        painter->drawText(linex - rect.width()/2,rectTimeLine.y()-10,showtimeStr);
        painter->restore();
    }

    drawValue(painter);
}


void ReplayTimeline::drawValue(QPainter *painter)
{



}
void ReplayTimeline::setDate(QDate date)
{

}

//void ReplayTimeline::mousePressEvent(QMouseEvent* event){

//}
//void ReplayTimeline::mouseMoveEvent(QMouseEvent *event){

//    qDebug()<<" mouseMoveEvent  ";

//}

//void ReplayTimeline::hoverMoveEvent(QMouseEvent *event){
//    qDebug()<<" hoverMoveEvent  ";
//    if(rectFIndicator.contains(event->pos())){

//        qDebug()<<" mouseMoveEvent **** ";
//        rectFIndicator.setSize(QSize(2,2));
//        update();
//    }
//}
//void ReplayTimeline::mouseReleaseEvent(QMouseEvent *event){

//}

