#include "renderthread.h"
#include <QOpenGLFramebufferObject>
#include <QGuiApplication>
#include "render/nv12render.h"

#include <QDebug>
#include <QFile>
RenderThread::RenderThread(QSize &size,QList<yuvInfo> *buff,yuvInfo *info, QObject *parent):
    QThread(parent),
    m_size(size)
{


    qDebug()<<"RenderThread  currentThread:"<<QThread::currentThreadId();


    yuvData = info;
    listYuvInfo = buff;
    first = false;
    worker = nullptr;
}

RenderThread::~RenderThread()
{

}

void RenderThread::renderNext()
{
    context->makeCurrent(surface); //使上下文回到离屏的surface
    if (!m_renderFbo) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        m_renderFbo = new QOpenGLFramebufferObject(m_size, format); //创建和windown(显示屏)格式一样的FBO
        m_displayFbo = new QOpenGLFramebufferObject(m_size, format);
        m_videoRender = new Nv12Render(); //实例化解码器
        m_videoRender->initialize();   //初始化
    }

    m_renderFbo->bind(); //绑定渲染的FBO到当前上下文
    context->functions()->glViewport(0, 0, m_size.width(), m_size.height()); //重调视口

//    while(listYuvInfo->size() <= 1){
//        QThread::msleep(10);
//    }
//    yuvInfo info = listYuvInfo->at(0);
//    m_videoRender->renderFrame(info.data,info.resolutionW,info.resolutionH); //渲染一帧视频
//    delete info.data;
//    listYuvInfo->removeFirst();

    while(!yuvData->data){
        QThread::msleep(10);
    }

    m_videoRender->renderFrame(yuvData->data,yuvData->resolutionW,yuvData->resolutionH); //渲染一帧视频
    context->functions()->glFlush(); //刷新一下
    m_renderFbo->bindDefault();  //渲染的FBO绑定到默认的上下文(也就是与主屏的surface对应的那个上下文),测试了，不绑定也可以
    qSwap(m_renderFbo, m_displayFbo); //交换两个FBO的内容
    emit textureReady(m_displayFbo->texture(), m_size);
}


void RenderThread::shutDown()
{

    qDebug()<<" shutDown    ";
    context->makeCurrent(surface);
    delete m_renderFbo;
    delete m_displayFbo;
    delete m_videoRender;
    context->doneCurrent();
    delete context;

    surface->deleteLater();

    exit();
    moveToThread(QGuiApplication::instance()->thread());
}
