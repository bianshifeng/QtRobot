#ifndef DUMPCONTROLLER_H
#define DUMPCONTROLLER_H
#include "httpconnectionhandler.h"

class DumpController : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(DumpController)
public:
    DumpController();

    // HttpRequestHandler interface
public:
    void service(HttpRequest &request, HttpResponse &response);
};

#endif // DUMPCONTROLLER_H
