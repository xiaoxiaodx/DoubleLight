#include "xvideoreplay.h"

XVideoReplay::XVideoReplay()
{
    setFlag(QQuickItem::ItemHasContents);
    QSize size;
    size.setWidth(640);
    size.setHeight(360);
    m_renderThread = new RenderThread(size,&listYuv,&yuvData,nullptr);

    playStartT.setHMS(0,0,0);
    playTotalTime = 0;

    yuvData.data = new uchar[960*600 * 3 /2];
//    yuvfile = new QFile("F:/work/doubleLight/avi/shot/20200407/212226_43.yuv");
//    if(yuvfile->open(QIODevice::ReadOnly))
//        yuvArr = yuvfile->readAll();

    connect(&timer,&QTimer::timeout,[&]{
//        if(yuvfile->open(QIODevice::ReadOnly)){
//           //yuvfile->seek(timeoutIndex*960*600*3/2);

//           QByteArray arr = yuvfile->read(960*600*3/2);

//            memcpy(yuvData.data,arr.data(),960*600*3/2);

//            timeoutIndex++;
//            yuvfile->close();
//        }else{
//            qDebug()<<"yuv 文件打开失败";
//        }

        if(yuvArr.size() > 0){
            yuvData.resolutionH = 600;
            yuvData.resolutionW = 960;
            memcpy(yuvData.data,yuvArr.data(),960*600*3/2);

            yuvArr.remove(0,960*600*3/2);
        }

    });
    //timer.start(40);

}


void XVideoReplay::ready()
{
    qDebug()<<"function ready";

    m_renderThread->surface = new QOffscreenSurface(); //实例一个离屏的Surface，有点像不显示的Window，使得opengl的contex能够绑定到它上面
    m_renderThread->surface->setFormat(m_renderThread->context->format());
    m_renderThread->surface->create(); //根据文档QOffscreenSurface的create只能在GUI线程调用，所以在这里做了实例和初始化。
    m_renderThread->moveToThread(m_renderThread); //移动到子线程循环
    //当场景失效后，关闭子线程的资源
    connect(window(), &QQuickWindow::sceneGraphInvalidated, m_renderThread, &RenderThread::shutDown, Qt::QueuedConnection);
    //启动子线程
    m_renderThread->start();
    update(); //再update一次用于实例TextureNode，因为程序刚初始化时会调用一次，但在初始化子线程后，返回了，所以要再来一次实例TextureNode。
}

QSGNode* XVideoReplay::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    TextureNode *node = static_cast<TextureNode *>(oldNode);
    qDebug()<<"updatePaintNode  currentThread:"<<QThread::currentThreadId();

    if (!m_renderThread->context) {
        QOpenGLContext *current = window()->openglContext();
        current->doneCurrent(); //取消opengl在当前上下文中的绑定，因为下面要设置shareContext，即将sharedContext移动到子线程
        m_renderThread->context = new QOpenGLContext();
        m_renderThread->context->setFormat(current->format());
        m_renderThread->context->setShareContext(current);
        m_renderThread->context->create();
        m_renderThread->context->moveToThread(m_renderThread); //context有线程归属性，一个context只能被它关联的线程调用makeCurrent，不能被其它线程调用;也只能有一个对应的surface
        //一个线程在同一时刻也只能有一个context
        current->makeCurrent(window()); //恢复绑定
        qDebug()<<"invokeMethod ready";
        QMetaObject::invokeMethod(this, "ready"); //跨线程调用


        return 0;
    }

    if (!node) {
        node = new TextureNode(window()); //实例化自定义的纹理结点

        //当纹理在子线程渲染好后，将纹理id、大小设置到自定义的QSimpleTextureNode结构中
        connect(m_renderThread, &RenderThread::textureReady, node, &TextureNode::newTexture, Qt::DirectConnection);
        //update函数调用后，渲染线程会在适当的时候发出beforRendering信号
        connect(node, &TextureNode::pendingNewTexture, window(), &QQuickWindow::update, Qt::QueuedConnection);
        //在开始渲染之前，把子线程渲染好的纹理设置到QSimpletTextureNode中，以便渲染线程使用
        connect(window(), &QQuickWindow::beforeRendering, node, &TextureNode::prepareNode, Qt::DirectConnection);
        //渲染好的纹理被使用后，通知子线程渲染下一帧
        connect(node, &TextureNode::textureInUse, m_renderThread, &RenderThread::renderNext, Qt::QueuedConnection);

        // Get the production of FBO textures started..
        qDebug()<<"invokeMethod renderNext";
        QMetaObject::invokeMethod(m_renderThread, "renderNext", Qt::QueuedConnection);
    }

    qDebug()<<"*************1";
    node->setRect(boundingRect());//设置显示区域，为qml分配的整个区域
    return node;
}
#include <QDir>
void XVideoReplay::funPlayTimeChange(QString relativePath,QString date,QTime playTime){

    QString tmpFilePath = findPlayFile(relativePath,date,playTime);

    if(tmpFilePath == "")
        return;

    if(curFilePath.compare(tmpFilePath)==0){


    }else{


        if(yuvfile != nullptr)
            delete  yuvfile;

        curFilePath = tmpFilePath;
        qDebug()<<" curFilePath "<<curFilePath;
        yuvfile = new QFile(curFilePath+".yuv");


            if(yuvfile->open(QIODevice::ReadOnly))
                yuvArr = yuvfile->readAll();
        timer.start(40);
    }
}

QString XVideoReplay::findPlayFile(QString relativePath,QString date,QTime time){

    QDir dir(relativePath+"/"+date);

    if(!dir.exists()){
        qDebug()<<"文件路径不存在";
    }
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.yuv";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for (int i=0;i<files.size();i++) {
        QString tmpName = files.at(i);
        QString fileName = tmpName.remove(".yuv");
        qDebug()<<"files name:"<<fileName;

        QStringList fileNames = fileName.split("_");
        if(fileNames.size() != 2)
            continue;

        QString timeStart = fileNames[0];
        QString longt = fileNames[1];

        //        int h = timeStart.mid(0,2).toInt();
        //        int m = timeStart.mid(2,2).toInt();
        //        int s = timeStart.mid(4,2).toInt();
        int t = longt.toInt();

        QTime startT = QTime::fromString(timeStart,"hhmmss");
        QTime endT = startT.addSecs(t);

        int secTime = time.msecsSinceStartOfDay();
        int secStartT = startT.msecsSinceStartOfDay();
        int secEndT = endT.msecsSinceStartOfDay();
        if(secTime>= secStartT && secTime<=secEndT){
            return (relativePath+"/"+date+"/"+tmpName);
        }
    }
    return "";
}
