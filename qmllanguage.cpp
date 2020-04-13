#include "qmlLanguage.h"
#include <QDebug>
QmlLanguage::QmlLanguage(QGuiApplication &app, QQmlApplicationEngine& engine)
{
    m_app = &app;
    m_engine = &engine;
}

void QmlLanguage::setLanguage(int nLanguage)
{
    emit signal_languageChange(nLanguage);
//    QTranslator translator;
//    if (nLanguage == 0)
//    {
//        if(translator.load("en_US.qm"))
//            qDebug()<<"加载英文成功";
//    }else{
//        if(translator.load("zh_CN.qm"))
//            qDebug()<<"加载中文文成功";

//    }
//    if(m_app->installTranslator(&translator))
//        qDebug()<<"安装成功";
//    m_engine->retranslate();
}
