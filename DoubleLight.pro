QT += quick
CONFIG += c++11
 
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# 版本信息
VERSION = 1.0
# 图标
RC_ICONS = myico.ico
TARGET=GAOZHI_TCS
# 公司名称
QMAKE_TARGET_COMPANY = "高至科技"

# 产品名称
QMAKE_TARGET_PRODUCT = "测温平台"

# 文件说明
QMAKE_TARGET_DESCRIPTION = "Qt Creator based on Qt 5.12.2 (MINGW 2020, 64 bit)"

# 版权信息
QMAKE_TARGET_COPYRIGHT = "Copyright 2020-2016 The Qt Company Ltd. All rights reserved."

# 中文（简体）
RC_LANG = 0x0004

SOURCES += \
        main.cpp \
    mysearch1.cpp \
    tcpworker.cpp \
    XVideo.cpp \
    youseeparse.cpp \
    render/renderthread.cpp \
    render/texturenode.cpp \
    ffmpegcodec.cpp \
    debuglog.cpp \
    render/nv12render.cpp \
    avi/avformat.cpp \
    avi/avformat_input.cpp \
    avi/avi_adapt.cpp \
    avi/avienc_adapt.cpp \
    avi/hi_avi.cpp \
    avirecord.cpp \
    screenvideo.cpp \
    ffmpegconvert.cpp \
    chttpapidevice.cpp \
    replaytimeline.cpp \
    xvideoreplay.cpp \
    ffmpegreplay.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    mysearch1.h \
    tcpworker.h \
    XVideo.h \
    youseeparse.h \
    render/renderthread.h \
    render/texturenode.h \
    ffmpegcodec.h \
    debuglog.h \
    render/nv12render.h \
    avi/adapt.h \
    avi/avformat.h \
    avi/avi_adapt.h \
    avi/avienc_adapt.h \
    avi/common.h \
    avi/debug.h \
    avi/defs.h \
    avi/hi_avi.h \
    avi/hi_type.h \
    avirecord.h \
    common.h \
    screenvideo.h \
    ffmpegconvert.h \
    chttpapidevice.h \
    replaytimeline.h \
    xvideoreplay.h \
    ffmpegreplay.h


INCLUDEPATH += $$PWD/avi

INCLUDEPATH += $$PWD/third/ffmpeg64/include
LIBS += $$PWD/third/ffmpeg64/lib/avcodec.lib \
        $$PWD/third/ffmpeg64/lib/avdevice.lib \
        $$PWD/third/ffmpeg64/lib/avfilter.lib \
        $$PWD/third/ffmpeg64/lib/avformat.lib \
        $$PWD/third/ffmpeg64/lib/avutil.lib \
        $$PWD/third/ffmpeg64/lib/postproc.lib \
        $$PWD/third/ffmpeg64/lib/swresample.lib \
        $$PWD/third/ffmpeg64/lib/swscale.lib

INCLUDEPATH += $$PWD/third/yousee/include
LIBS += $$PWD/third/yousee/YoseenFfmpeg.lib
LIBS += $$PWD/third/yousee/YoseenSDK.lib

INCLUDEPATH += $$PWD/OpenCV4.1.1x64/include
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_calib3d411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_dnn411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_flann411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_gapi411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_imgcodecs411.dll
LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_imgproc411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_ml411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_features2d411.dll

LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_core411.dll
LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_highgui411.dll

#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_objdetect411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_video411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_photo411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_stitching411.dll
#LIBS += $$PWD/OpenCV4.1.1x64/x64/mingw/bin/libopencv_videoio411.dll

