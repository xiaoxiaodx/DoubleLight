QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# �汾��Ϣ
VERSION = 1.1.0.8
# ͼ
RC_ICONS = myicon6.ico
TARGET=TTM-S
# ��˾����
QMAKE_TARGET_COMPANY = "�����Ƽ�"

# ��Ʒ����
QMAKE_TARGET_PRODUCT = "����ƽ̨"

# �ļ�˵��
QMAKE_TARGET_DESCRIPTION = "Qt Creator based on Qt 5.12.2 (MINGW 2020, 64 bit)"

# ��Ȩ��Ϣ
QMAKE_TARGET_COPYRIGHT = "Copyright 2020-2016 The Qt Company Ltd. All rights reserved."

# ���ģ����壩
RC_LANG = 0x0004

SOURCES += \
        main.cpp \
    playVideo/XVideo.cpp \
    playVideo/xvideoreplay.cpp \
    recordVideo/captureScreen/screenvideo.cpp \
    render/nv12render.cpp \
    render/renderthread.cpp \
    render/texturenode.cpp \
    thirdSrc/yousee/youseeparse.cpp \
    util/debuglog.cpp \
    qmlcplus/replaytimeline.cpp \
    thirdSrc/ffmpeg/ffmpegcodec.cpp \
    thirdSrc/ffmpeg/ffmpegconvert.cpp \
    thirdSrc/ffmpeg/ffmpegreplay.cpp \
    thirdSrc/ffmpeg/ffmpegwriteavi.cpp \
    protocol/chttpapidevice.cpp \
    protocol/mysearch1.cpp \
    protocol/tcpworker.cpp \
    playVideo/xvideotemp.cpp \
    recordVideo/captureScreen/writeh264.cpp \
    qmlcplus/warnmodeldata.cpp \
    qmlcplus/warnmodel.cpp \
    application.cpp \
    qmlcplus/replaydatesearch.cpp \
    thirdSrc/avi/avformat.cpp \
    thirdSrc/avi/avformat_input.cpp \
    thirdSrc/avi/avi_adapt.cpp \
    thirdSrc/avi/avienc_adapt.cpp \
    thirdSrc/avi/hi_avi.cpp \
    recordVideo/record/avirecord.cpp \
    protocol/shigan.cpp \
    protocol/shiganobject.cpp \
    thirdSrc/irc/ircnet.cpp \
    thirdSrc/J07/j07device.cpp \
    qmlcplus/updateprogressc.cpp \
    qmlcplus/upcontrol.cpp \
    protocol/warntcpserver.cpp \
    consoleprocess.cpp

HEADERS += \
    playVideo/XVideo.h \
    playVideo/xvideoreplay.h \
    recordVideo/captureScreen/screenvideo.h \
    render/nv12render.h \
    render/renderthread.h \
    render/texturenode.h \
    thirdSrc/yousee/youseeparse.h \
    util/debuglog.h \
    qmlcplus/replaytimeline.h \
    thirdSrc/ffmpeg/ffmpegcodec.h \
    thirdSrc/ffmpeg/ffmpegconvert.h \
    thirdSrc/ffmpeg/ffmpegreplay.h \
    thirdSrc/ffmpeg/ffmpegwriteavi.h \
    protocol/chttpapidevice.h \
    protocol/mysearch1.h \
    protocol/tcpworker.h \
    util/common.h \
    playVideo/xvideotemp.h \
    recordVideo/captureScreen/writeh264.h \
    qmlcplus/warnmodeldata.h \
    qmlcplus/warnmodel.h \
    application.h \
    qmlcplus/replaydatesearch.h \
    thirdSrc/avi/adapt.h \
    thirdSrc/avi/avformat.h \
    thirdSrc/avi/avi_adapt.h \
    thirdSrc/avi/avienc_adapt.h \
    thirdSrc/avi/common.h \
    thirdSrc/avi/debug.h \
    thirdSrc/avi/defs.h \
    thirdSrc/avi/hi_avi.h \
    thirdSrc/avi/hi_type.h \
    recordVideo/record/avirecord.h \
    protocol/shigan.h \
    protocol/shiganobject.h \
    thirdSrc/irc/ircnet.h \
    thirdSrc/J07/j07device.h \
    qmlcplus/updateprogressc.h \
    qmlcplus/upcontrol.h \
    protocol/warntcpserver.h \
    consoleprocess.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/avi
INCLUDEPATH += $$PWD/protocol
INCLUDEPATH += $$PWD/playVideo
INCLUDEPATH += $$PWD/render
INCLUDEPATH += $$PWD/thirdSrc
INCLUDEPATH += $$PWD/thirdSrc/ffmpeg
INCLUDEPATH += $$PWD/thirdSrc/yousee
INCLUDEPATH += $$PWD/thirdSrc/avi
INCLUDEPATH += $$PWD/recordVideo/captureScreen
INCLUDEPATH += $$PWD/util
INCLUDEPATH += $$PWD/qmlcplus


INCLUDEPATH += $$PWD/thirdLib/ffmpeg64/include
LIBS += $$PWD/thirdLib/ffmpeg64/lib/avcodec.lib \
        $$PWD/thirdLib/ffmpeg64/lib/avdevice.lib \
        $$PWD/thirdLib/ffmpeg64/lib/avfilter.lib \
        $$PWD/thirdLib/ffmpeg64/lib/avformat.lib \
        $$PWD/thirdLib/ffmpeg64/lib/avutil.lib \
        $$PWD/thirdLib/ffmpeg64/lib/postproc.lib \
        $$PWD/thirdLib/ffmpeg64/lib/swresample.lib \
        $$PWD/thirdLib/ffmpeg64/lib/swscale.lib

INCLUDEPATH += $$PWD/thirdLib/yousee/include
LIBS += $$PWD/thirdLib/yousee/YoseenFfmpeg.lib
LIBS += $$PWD/thirdLib/yousee/YoseenSDK.lib

INCLUDEPATH += $$PWD/thirdLib/OpenCV4.1.1x64/include
LIBS += $$PWD/thirdLib/OpenCV4.1.1x64/x64/mingw/bin/libopencv_imgproc411.dll
LIBS += $$PWD/thirdLib/OpenCV4.1.1x64/x64/mingw/bin/libopencv_core411.dll
LIBS += $$PWD/thirdLib/OpenCV4.1.1x64/x64/mingw/bin/libopencv_highgui411.dll


#十敢需要使用windows socket库支持
LIBS += -lWs2_32
#IRC
INCLUDEPATH += $$PWD/thirdLib/irc/include
LIBS += $$PWD/thirdLib/irc/IRNetSDK64.lib
