#include "webserver.h"
#include <QCoreApplication>
#include <QDir>
#include <QSettings>

#include "webrequestrouter.h"
#include "webglobaltool.h"

WebServer::WebServer(QObject *parent) : QObject(parent)
{

    QString m_configFile = WebServer::searchConfigFile();


    // Configure template loader and cache
    QSettings* m_templateSettings = new QSettings(m_configFile,QSettings::IniFormat,this);
    m_templateSettings->beginGroup("templates");
    g_templateCache = new TemplateCache(m_templateSettings,this);

    //Concigure session store
    QSettings* m_sessionSettings = new QSettings(m_configFile,QSettings::IniFormat,this);
    m_sessionSettings->beginGroup("sessions");
    g_sessionStore = new HttpSessionStore(m_sessionSettings,this);

    //Configure static file controller
    QSettings* m_fileSettings = new QSettings(m_configFile,QSettings::IniFormat,this);
    m_fileSettings->beginGroup("docroot");
    g_staticFileController = new StaticFileController(m_fileSettings,this);

    //Configure and start the tcp listener
    QSettings* m_listenerSettings = new QSettings(m_configFile,QSettings::IniFormat,this);
    m_listenerSettings->beginGroup("listener");

    WebRequestRouter* m_webRequestRouter = new WebRequestRouter(this);
//    connect(m_webRequestRouter,SIGNAL(sig_router_talk(QString)),this,SLOT(slot_webrequest_talk(QString)));


    g_httpServer = new HttpListener(m_listenerSettings,m_webRequestRouter,this);

}

QString WebServer::searchConfigFile(const QString configName)
{
    QString t_binDir = QCoreApplication::applicationDirPath();
    QString t_configName = QCoreApplication::applicationName();
    if(!configName.isNull())t_configName = configName;
    QString t_fileName(t_configName+".ini");


    QStringList t_searchList;
    t_searchList.append(t_binDir);
    t_searchList.append(t_binDir+"/etc");
    t_searchList.append(t_binDir+"/../etc");
    t_searchList.append(t_binDir+"/../../etc"); //for development without shadow build
    t_searchList.append(t_binDir+"/../"+t_configName+"/etc");
    t_searchList.append(t_binDir+"/../../"+t_configName+"/etc");
    t_searchList.append(t_binDir+"/../../../"+t_configName+"/etc");
    t_searchList.append(t_binDir+"/../../../../"+t_configName+"/etc");
    t_searchList.append(t_binDir+"/../../../../../"+t_configName+"/etc");
    t_searchList.append(QDir::rootPath()+"etc/opt");
    t_searchList.append(QDir::rootPath()+"etc");


    foreach(QString t_dir, t_searchList)
    {
        QFile t_file(t_dir+"/"+ t_fileName);
        if(t_file.exists())
        {
            //found
            t_fileName =QDir(t_file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(t_fileName));
            return t_fileName;
        }
    }

    // not found

    foreach (QString t_dir, t_searchList)
    {
        qWarning("%s/%s not found", qPrintable(t_dir),qPrintable(t_fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(t_fileName));
    return 0;
}

void WebServer::slot_webrequest_talk(QString name)
{
    if(g_pRobot){
        g_pRobot->audio_play_tts(name);
    }
}
