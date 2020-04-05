#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "XVideo.h"
#include "screenvideo.h"
#include "replaytimeline.h"
#include "xvideoreplay.h"
#include "xvideotemp.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QFont font("Microsoft Yahei");
    app.setFont(font);

    app.setOrganizationName("GaoZhi"); //1
    app.setOrganizationDomain("gaozhi.com"); //2
    app.setApplicationName("DoubleLight"); //3

    QQmlApplicationEngine engine;
    qmlRegisterType<XVideo>("XVideo", 1, 0, "XVideo");
    qmlRegisterType<XVideoTemp>("XVideoTemp", 1, 0, "XVideoTemp");
    qmlRegisterType<ScreenVideo>("ScreenVideo", 1, 0, "ScreenVideo");
    qmlRegisterType<ReplayTimeline>("TimeLine", 1, 0, "TimeLine");
    qmlRegisterType<XVideoReplay>("XVideoReplay", 1, 0, "XVideoReplay");

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));



    if (engine.rootObjects().isEmpty())
        return -1;





    //    app.setOrganizationName("Gaozhi1"); //1
    //    app.setOrganizationDomain("gaozhi.com"); //2
    //    app.setApplicationName("VMS_V1.2"); //3
    //    QFont font("Microsoft Yahei");
    //    app.setFont(font);

    // XVideo 为QPaint显示视频(光栅绘图)

    //qmlRegisterType<DeviceManagerment>("DeviceManagerment", 1, 0, "DeviceManagerment");



    return app.exec();
}
