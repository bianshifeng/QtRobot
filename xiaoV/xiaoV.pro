TEMPLATE = app
TARGET  = xiaoVBase5

QT += qml quick widgets core gui

QT += serialport network multimedia script multimediawidgets xml

include(./src/src.pri)
include (./bin/bin.pri)
include (./plugins/webServer/plugin_webServer.pri)
include (./src/base/navigation/plugin_navi.pri)

RESOURCES += \
    qmlV5.qrc

SOURCES += \
    main.cpp
