
#webserver root
INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

HEADERS += \
    $$PWD/src/webserver.h \
    $$PWD/src/webrequestrouter.h \
    $$PWD/src/controller/dumpcontroller.h \
    $$PWD/src/webglobaltool.h \
    $$PWD/src/controller/mainpage_templatecontroller.h

SOURCES += \
    $$PWD/src/webserver.cpp \
    $$PWD/src/webrequestrouter.cpp \
    $$PWD/src/controller/dumpcontroller.cpp \
    $$PWD/src/webglobaltool.cpp \
    $$PWD/src/controller/mainpage_templatecontroller.cpp


#独立模块包容
#include($$PWD/QtWebApp/logging/logging.pri)
#include($$PWD/QtWebApp/httpserver/httpserver.pri)
#include($$PWD/QtWebApp/templateengine/templateengine.pri)



#web模块包容
#loggine moudel
INCLUDEPATH += $$PWD/QtWebApp/logging
DEPENDPATH += $$PWD/QtWebApp/logging

HEADERS += \
    $$PWD/QtWebApp/logging/logglobal.h \
    $$PWD/QtWebApp/logging/logmessage.h \
    $$PWD/QtWebApp/logging/logger.h \
    $$PWD/QtWebApp/logging/filelogger.h \
    $$PWD/QtWebApp/logging/dualfilelogger.h

SOURCES += \
    $$PWD/QtWebApp/logging/logmessage.cpp \
    $$PWD/QtWebApp/logging/logger.cpp \
    $$PWD/QtWebApp/logging/filelogger.cpp \
    $$PWD/QtWebApp/logging/dualfilelogger.cpp

#httpserver moudel
INCLUDEPATH += $$PWD/QtWebApp/httpserver
DEPENDPATH += $$PWD/QtWebApp/httpserver
QT += network

# Enable very detailed debug messages when compiling the debug version
CONFIG(debug, debug|release) {
    DEFINES += SUPERVERBOSE
}

HEADERS += $$PWD/QtWebApp/httpserver/httpglobal.h \
           $$PWD/QtWebApp/httpserver/httplistener.h \
           $$PWD/QtWebApp/httpserver/httpconnectionhandler.h \
           $$PWD/QtWebApp/httpserver/httpconnectionhandlerpool.h \
           $$PWD/QtWebApp/httpserver/httprequest.h \
           $$PWD/QtWebApp/httpserver/httpresponse.h \
           $$PWD/QtWebApp/httpserver/httpcookie.h \
           $$PWD/QtWebApp/httpserver/httprequesthandler.h \
           $$PWD/QtWebApp/httpserver/httpsession.h \
           $$PWD/QtWebApp/httpserver/httpsessionstore.h \
           $$PWD/QtWebApp/httpserver/staticfilecontroller.h

SOURCES += $$PWD/QtWebApp/httpserver/httpglobal.cpp \
           $$PWD/QtWebApp/httpserver/httplistener.cpp \
           $$PWD/QtWebApp/httpserver/httpconnectionhandler.cpp \
           $$PWD/QtWebApp/httpserver/httpconnectionhandlerpool.cpp \
           $$PWD/QtWebApp/httpserver/httprequest.cpp \
           $$PWD/QtWebApp/httpserver/httpresponse.cpp \
           $$PWD/QtWebApp/httpserver/httpcookie.cpp \
           $$PWD/QtWebApp/httpserver/httprequesthandler.cpp \
           $$PWD/QtWebApp/httpserver/httpsession.cpp \
           $$PWD/QtWebApp/httpserver/httpsessionstore.cpp \
           $$PWD/QtWebApp/httpserver/staticfilecontroller.cpp


#templateengine moudel
INCLUDEPATH += $$PWD/QtWebApp/templateengine
DEPENDPATH += $$PWD/QtWebApp/templateengine

HEADERS += \
    $$PWD/QtWebApp/templateengine/templateglobal.h \
    $$PWD/QtWebApp/templateengine/template.h \
    $$PWD/QtWebApp/templateengine/templateloader.h \
    $$PWD/QtWebApp/templateengine/templatecache.h


SOURCES += \
    $$PWD/QtWebApp/templateengine/template.cpp \
    $$PWD/QtWebApp/templateengine/templateloader.cpp \
    $$PWD/QtWebApp/templateengine/templatecache.cpp



OTHER_FILES += \
    $$PWD/etc/* \
    $$PWD/etc/docroot/* \
    $$PWD/etc/templates/* \
    $$PWD/etc/ssl/* \
    $$PWD/logs/*



