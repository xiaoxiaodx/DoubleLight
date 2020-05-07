#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "XVideo.h"
#include "screenvideo.h"
#include "replaytimeline.h"
#include "xvideoreplay.h"
#include "xvideotemp.h"
#include <qqmlcontext.h>
#include "warnmodel.h"
#include "application.h"
#include "updateprogressc.h"

#include "toolutil.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    Application app(argc, argv);
    if(!app.lock())
         return -42;
    QFont font("Microsoft Yahei");
    app.setFont(font);
    app.setOrganizationName("GaoZhi"); //1
    app.setOrganizationDomain("gaozhi.com"); //2
    app.setApplicationName("DoubleLight"); //3

    QQmlApplicationEngine engine;
//    QmlLanguage qmlLanguage(app, engine);
//    engine.rootContext()->setContextProperty("qmlLanguage", &qmlLanguage);


//    QTranslator translator;
//    translator.load("en_US.qm");
//    app.installTranslator(&translator);



    qmlRegisterType<XVideo>("XVideo", 1, 0, "XVideo");
    qmlRegisterType<XVideoTemp>("XVideoTemp", 1, 0, "XVideoTemp");
    qmlRegisterType<ScreenVideo>("ScreenVideo", 1, 0, "ScreenVideo");
    qmlRegisterType<ReplayTimeline>("TimeLine", 1, 0, "TimeLine");
    qmlRegisterType<XVideoReplay>("XVideoReplay", 1, 0, "XVideoReplay");
    qmlRegisterType<WarnModel>("WarnModel", 1, 0, "WarnModel");
    qmlRegisterType<updateProgressC>("UpdateProgressC", 1, 0, "UpdateProgressC");
    qmlRegisterType<toolUtil>("ToolUtil", 1, 0, "ToolUtil");

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
