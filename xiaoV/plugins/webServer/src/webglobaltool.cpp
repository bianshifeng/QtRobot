#include "webglobaltool.h"

/** Cache for template files*/
TemplateCache* g_templateCache;

/** Storage for session cookies*/
HttpSessionStore* g_sessionStore;

/** Controller for static files*/
StaticFileController* g_staticFileController;

/** Redirects  log messages to a file */
FileLogger* g_logger;

HttpListener* g_httpServer;



