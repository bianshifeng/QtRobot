#include "tmplugintools.h"
#include <tmglobaltool.h>

TmPluginTools::TmPluginTools(QObject *parent) : QObject(parent)
{
    pr_pSocketExScreen = NULL;
    pr_nUdpPort = 0;
}

void TmPluginTools::initConfig()
{
    QJsonObject remoteObj = g_configJson.value("Remote").toObject();
    pr_nUdpPort = remoteObj.value("plugin_screen").toInt(7755);

    // signal
    connect(this, SIGNAL(sig_init()), this, SLOT(slot_init()));
    connect(this, SIGNAL(sig_exscreen_pic_dir(QString)), this, SLOT(slot_exscreen_pic_dir(QString)));
    connect(this, SIGNAL(sig_exscreen_pic_file(QString)), this, SLOT(slot_exscreen_pic_file(QString)));
    connect(this, SIGNAL(sig_exscreen_video_dir(QString)), this, SLOT(slot_exscreen_video_dir(QString)));
    connect(this, SIGNAL(sig_exscreen_video_file(QString)), this, SLOT(slot_exscreen_video_file(QString)));

    // start exe
    QProcess* pExScreen = new QProcess();
    pExScreen->start("MultiScreenTool.exe");

    // init
    emit sig_init();
}

void TmPluginTools::slot_init()
{
    pr_pSocketExScreen = new QUdpSocket();
}

void TmPluginTools::slot_exscreen_pic_dir(QString strDirName)
{
    if( !pr_pSocketExScreen ){
        return;
    }
    QString strCmd = QString("pic dir %1").arg(strDirName);
    pr_pSocketExScreen->writeDatagram(strCmd.toUtf8(),QHostAddress::LocalHost, pr_nUdpPort);
    //pr_pSocketExScreen->waitForBytesWritten(3000);
    pr_pSocketExScreen->waitForReadyRead(3000);
}

void TmPluginTools::slot_exscreen_pic_file(QString strFileFullName)
{
    if( !pr_pSocketExScreen ){
        return;
    }
    QString strCmd = QString("pic fil %1").arg(strFileFullName);
    pr_pSocketExScreen->writeDatagram(strCmd.toUtf8(),QHostAddress::LocalHost, pr_nUdpPort);
    //pr_pSocketExScreen->waitForBytesWritten(3000);
    pr_pSocketExScreen->waitForReadyRead(3000);
}

void TmPluginTools::slot_exscreen_video_dir(QString strDirName)
{
    if( !pr_pSocketExScreen ){
        return;
    }
    QString strCmd = QString("vid dir %1").arg(strDirName);
    pr_pSocketExScreen->writeDatagram(strCmd.toUtf8(),QHostAddress::LocalHost, pr_nUdpPort);
    //pr_pSocketExScreen->waitForBytesWritten(3000);
    pr_pSocketExScreen->waitForReadyRead(3000);
}

void TmPluginTools::slot_exscreen_video_file(QString strFileFullName)
{
    if( !pr_pSocketExScreen ){
        return;
    }
    QString strCmd = QString("vid fil %1").arg(strFileFullName);
    pr_pSocketExScreen->writeDatagram(strCmd.toUtf8(),QHostAddress::LocalHost, pr_nUdpPort);
    //pr_pSocketExScreen->waitForBytesWritten(3000);
    pr_pSocketExScreen->waitForReadyRead(3000);
}

