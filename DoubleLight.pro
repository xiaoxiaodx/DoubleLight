QT += quick axcontainer
QT += multimedia
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

VERSION = 2.0.2.6

RC_ICONS = myicon11.ico
TARGET=VMukti_VMS

QMAKE_TARGET_COMPANY = "VMukti"
QMAKE_TARGET_PRODUCT = "VMukti VMS"
QMAKE_TARGET_DESCRIPTION = "VMukti VMS"
QMAKE_TARGET_COPYRIGHT = "Copyright The VMukti Company. All rights reserved."


RC_LANG = 0x0004

SOURCES += \
        main.cpp \
    playVideo/XVideo.cpp \
    recordVideo/captureScreen/screenvideo.cpp \
    util/debuglog.cpp \
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
    thirdSrc/avi/avformat.cpp \
    thirdSrc/avi/avformat_input.cpp \
    thirdSrc/avi/avi_adapt.cpp \
    thirdSrc/avi/avienc_adapt.cpp \
    thirdSrc/avi/hi_avi.cpp \
    recordVideo/record/avirecord.cpp \
    thirdSrc/J07/j07device.cpp \
    qmlcplus/updateprogressc.cpp \
    qmlcplus/upcontrol.cpp \
    protocol/warntcpserver.cpp \
    consoleprocess.cpp

HEADERS += \
    playVideo/XVideo.h \
    recordVideo/captureScreen/screenvideo.h \
    util/debuglog.h \
    thirdSrc/ffmpeg/ffmpegcodec.h \
    thirdSrc/ffmpeg/ffmpegconvert.h \
    thirdSrc/ffmpeg/ffmpegreplay.h \
    thirdSrc/ffmpeg/ffmpegwriteavi.h \
    protocol/chttpapidevice.h \
    protocol/mysearch1.h \
    protocol/tcpworker.h \
    playVideo/xvideotemp.h \
    recordVideo/captureScreen/writeh264.h \
    qmlcplus/warnmodeldata.h \
    qmlcplus/warnmodel.h \
    application.h \
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
    thirdSrc/J07/j07device.h \
    qmlcplus/updateprogressc.h \
    qmlcplus/upcontrol.h \
    protocol/warntcpserver.h \
    consoleprocess.h

#RESOURCES += qml.qrc

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
INCLUDEPATH += $$PWD/thirdSrc
INCLUDEPATH += $$PWD/thirdSrc/ffmpeg
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





#十敢需要使用windows socket库支持
LIBS += -lWs2_32


RESOURCES += \
    qml.qrc

