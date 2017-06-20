#ifndef WEBGLOBALTOOL_H
#define WEBGLOBALTOOL_H
#include "templatecache.h"
#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"
#include "httplistener.h"
//在一个大型项目中，首先看看那些类用于全局使用，比如公用的一些，资源类，队列类，
//将这些类直接放入全局变量文件中，后期使用直接调运即可
//第一次声明的全局变量，表示其他地方使用的时候，必须在这里已经被第一过

/** Cache for template files*/
extern TemplateCache* g_templateCache;

/** Storage for session cookies*/
extern HttpSessionStore* g_sessionStore;

/** Controller for static files*/
extern StaticFileController* g_staticFileController;

/** Redirects  log messages to a file */
extern FileLogger* g_logger;

extern HttpListener* g_httpServer;


#endif // WEBGLOBALTOOL_H
