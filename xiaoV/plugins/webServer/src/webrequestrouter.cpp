#include "webrequestrouter.h"

#include "controller/dumpcontroller.h"
#include "controller/mainpage_templatecontroller.h"

#include "filelogger.h"
#include "staticfilecontroller.h"
#include "webglobaltool.h"

WebRequestRouter::WebRequestRouter(QObject *parent):
    HttpRequestHandler(parent)
{
    qDebug("RequestRouter: created");

}

WebRequestRouter::~WebRequestRouter()
{

    qDebug("RequestRouter: deleted");

}

void WebRequestRouter::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray t_path = request.getPath();
    qDebug("WebRequestRouter: path=%s",t_path.data());
    QByteArray t_main_tip = "---*main+++page*---";
    QByteArray t_path_main = t_path.append(t_main_tip);

    if(t_path.startsWith("/")&&t_path_main.startsWith("/"+t_main_tip))
    {
        MainPage_TemplateController().service(request,response);
        emit sig_router_talk("你进入main页面");
//        g_staticFileController->service(request, response);

    }
    else if(t_path.startsWith("/list"))
    {
        MainPage_TemplateController().service(request,response);
    }
    else if(t_path.startsWith("/dump"))
    {
        DumpController().service(request,response);
        emit sig_router_talk("你进入dump页面");
    }
    else
    {
        g_staticFileController->service(request, response);
    }

    if(g_logger)
    {
        g_logger->clear();
    }




}
