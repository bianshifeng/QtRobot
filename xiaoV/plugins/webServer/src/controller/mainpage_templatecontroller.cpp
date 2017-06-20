#include "mainpage_templatecontroller.h"
#include "webglobaltool.h"
MainPage_TemplateController::MainPage_TemplateController()
{

}

void MainPage_TemplateController::service(HttpRequest &request, HttpResponse &response)
{
    response.setHeader("Content-Type","text/html; charset=UTF-8");
//    response.setHeader("Content-Type", "image/jpeg");

    Template t= g_templateCache->getTemplate("mainTemplate",request.getHeader("Accept-Language"));
    t.enableWarnings();
    t.setVariable("path",request.getPath());

    QMap<QByteArray,QByteArray> headers = request.getHeaderMap();
    QMapIterator<QByteArray,QByteArray> iterator(headers);
    t.loop("header",headers.size());
    int i = 0;
    while (iterator.hasNext())
    {
        iterator.next();
        t.setVariable(QString("header%1.name").arg(i),QString(iterator.key()));
        t.setVariable(QString("header%1.value").arg(i),QString(iterator.value()));
        ++i;
    }

//    <head>
//        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
//        <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0" />
//        <meta content="yes" name="apple-mobile-web-app-capable" />
//        <meta content="black" name="apple-mobile-web-app-status-bar-style" />
//        <meta content="telephone=no" name="format-detectio" />
//        <link rel="stylesheet" href="css/bootstrap.min.css">
//        <link rel="stylesheet" href="css/index.css">
//        <title>Pair</title>
//    </head>

    QByteArray body;
    body.append("<html>");
    body.append("<head>");
    body.append("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
    body.append("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0\" />");
    body.append("<meta content=\"yes\" name=\"apple-mobile-web-app-capable\" />");
    body.append("<meta content=\"black\" name=\"apple-mobile-web-app-status-bar-style\" />");
    body.append("<meta content=\"telephone=no\" name=\"format-detectio\" />");
    body.append("<link rel=\"stylesheet\" href=\"css/bootstrap.min.css\">");
    body.append("<link rel=\"stylesheet\" href=\"css/index.css\">");
    body.append("<title>Pair</title>");
    body.append("</head>");
    body.append("<body>");
    body.append("<div><img src=\"./images/qfeel_text.png\" style=\"width: 70px;\"></div>");
    body.append(t.toLatin1());
    body.append("<script src=\"js/jquery.min.js\"></script>");
    body.append("</body></html>");
    response.write(body,true);


}

