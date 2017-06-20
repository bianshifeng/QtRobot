#ifndef MAINPAGE_TEMPLATECONTROLLER_H
#define MAINPAGE_TEMPLATECONTROLLER_H
#include "httprequesthandler.h"

class MainPage_TemplateController : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(MainPage_TemplateController)
public:
    MainPage_TemplateController();

    // HttpRequestHandler interface
public:
    void service(HttpRequest &request, HttpResponse &response);
};

#endif // MAINPAGE_TEMPLATECONTROLLER_H
