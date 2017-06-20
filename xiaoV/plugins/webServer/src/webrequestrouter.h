#ifndef WEBREQUESTROUTER_H
#define WEBREQUESTROUTER_H

#include <QObject>
#include "httprequesthandler.h"

class WebRequestRouter : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(WebRequestRouter)


public:
    WebRequestRouter(QObject* parent=0);
    ~WebRequestRouter();

    // HttpRequestHandler interface

signals:
    void sig_router_talk(QString);
public:
    void service(HttpRequest &request, HttpResponse &response);
};

#endif // WEBREQUESTROUTER_H
