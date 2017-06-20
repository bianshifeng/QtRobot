HEADERS +=\
    $$PWD/robot.h \
    $$PWD/qmlrobot.h \
    $$PWD/robot/camera/qmlcamera.h \
    $$PWD/robot/head/qmlrobothead.h \
    $$PWD/robot/servers/voiceserver/voicerequest.h \
    $$PWD/robot/servers/voiceserver/voicerequesthandler.h \
    $$PWD/robot/servers/voiceserver/voiceconnectionhandler.h \
    $$PWD/robot/brain/tmcompassserial.h \
    $$PWD/robot/feet/tmrobotwheel.h


SOURCES += \
    $$PWD/robot.cpp \
    $$PWD/qmlrobot.cpp \
    $$PWD/robot/camera/qmlcamera.cpp \
    $$PWD/robot/head/qmlrobothead.cpp \
    $$PWD/robot/servers/voiceserver/voicerequest.cpp \
    $$PWD/robot/servers/voiceserver/voicerequesthandler.cpp \
    $$PWD/robot/servers/voiceserver/voiceconnectionhandler.cpp \
    $$PWD/robot/brain/tmcompassserial.cpp \
    $$PWD/robot/feet/tmrobotwheel.cpp

#base
HEADERS += \
    $$PWD/base/qtmpackage.h \
    $$PWD/base/qtmtcpcontrol.h \
    $$PWD/base/qtmtcpvoiceserver.h \
    $$PWD/base/qtmudprecvvoice.h \
    $$PWD/base/qtmudpsendvoice.h \
    $$PWD/base/qtmvoiceiodevice.h \
    $$PWD/base/qtmvoiceplay.h \
    $$PWD/base/qtmvoicerecord.h \
    $$PWD/base/tmcamerawork.h \
    $$PWD/base/tmglobaltool.h \
    $$PWD/base/tmjsengine.h \
    $$PWD/base/tmjsenginework.h \
    $$PWD/base/tmkinect.h \
    $$PWD/base/tmmedia.h \
    $$PWD/base/tmmediawork.h \
    $$PWD/base/tmplugintools.h \
    $$PWD/base/tmremote.h \
    $$PWD/base/tmrobotinterface.h \
    $$PWD/base/tmserial.h \
    $$PWD/base/tmserialwork.h \
    $$PWD/base/tmupdate.h


SOURCES += \
    $$PWD/base/qtmpackage.cpp \
    $$PWD/base/qtmtcpcontrol.cpp \
    $$PWD/base/qtmtcpvoiceserver.cpp \
    $$PWD/base/qtmudprecvvoice.cpp \
    $$PWD/base/qtmudpsendvoice.cpp \
    $$PWD/base/qtmvoiceiodevice.cpp \
    $$PWD/base/qtmvoiceplay.cpp \
    $$PWD/base/qtmvoicerecord.cpp \
    $$PWD/base/tmcamerawork.cpp \
    $$PWD/base/tmglobaltool.cpp \
    $$PWD/base/tmjsengine.cpp \
    $$PWD/base/tmjsenginework.cpp \
    $$PWD/base/tmkinect.cpp \
    $$PWD/base/tmmedia.cpp \
    $$PWD/base/tmmediawork.cpp \
    $$PWD/base/tmplugintools.cpp \
    $$PWD/base/tmremote.cpp \
    $$PWD/base/tmrobotinterface.cpp \
    $$PWD/base/tmserial.cpp \
    $$PWD/base/tmserialwork.cpp \
    $$PWD/base/tmupdate.cpp



INCLUDEPATH += \
    $$PWD \
    $$PWD/base \



# 3rd lib-------------------------------------------------------

# kdxf lib
INCLUDEPATH += \
    $$PWD/modules

LIBS += \
    $$PWD/modules/libsharedlib_kdxf.a


# opencv lib
INCLUDEPATH +=C:\OpenCV\2.4.9\build\include

LIBS+= \
    c:\opencv\2.4.9\qt\lib\libopencv_calib3d249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_contrib249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_core249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_features2d249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_flann249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_gpu249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_highgui249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_imgproc249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_legacy249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_ml249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_objdetect249.dll.a\
    c:\opencv\2.4.9\qt\lib\libopencv_video249.dll.a


