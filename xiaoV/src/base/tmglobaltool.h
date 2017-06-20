#ifndef TMGLOBALTOOL_H
#define TMGLOBALTOOL_H
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QSerialPort>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QMap>
#include <QTimer>

#include <QTime>
#include <QDateTime>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QProcess>
#include <QSettings>
#include <QDataStream>
#include <QCryptographicHash>
#include <QDomDocument>
#include <QSettings>

#include <tmserial.h>
#include <tmmedia.h>
#include <tmrobotinterface.h>
#include <tmjsengine.h>
#include <tmcamerawork.h>
#include <tmremote.h>
#include <tmupdate.h>
#include <tmkinect.h>
#include <tmplugintools.h>


//#include <QMessageBox>
//#include <dlgmain.h>
//#include <dlgloading.h>
//#include <dlgpwd.h>
//#include <dlgstand.h>
//#include <dlgselfsay.h>
//#include <dlgyyy.h>
//#include <dlgziding.h>

//#include <dlgvideoshow.h>
//#include <dlgpicshow.h>
//#include <dlgshutdown.h>
//#include <dlgremote.h>
//#include <dlgconfig.h>
//#include <dlgtalk.h>


#include "robot.h"

// variable
extern QString g_strExeRoot;
extern QJsonObject g_configJson;
extern TmSerial* g_pTmSerial;
extern TmMedia* g_pTmMedia;
extern TmRobotInterface* g_pRobot;
extern TmJsEngineWork* g_pJsEngineMain;
extern TmJsEngineWork* g_pJsEngineModule;
extern TmJsEngine* g_pJsEngineMgr;
extern TmCameraWork* g_pCamera;
extern TmRemote* g_pRemote;
extern TmUpdate* g_pUpdate;
extern TmKinect* g_pKinect;
extern TmPluginTools* g_pPluginTools;


extern Robot* g_pDlgMain;

//extern DlgMain* g_pDlgMain;
//extern DlgLoading* g_pDlgLoading;
//extern DlgPwd* g_pDlgPwd;
//extern DlgConfig* g_pDlgConfig;

//extern DlgStand* g_pDlgStand;
//extern DlgYYY* g_pDlgYYY;
//extern DlgSelfSay* g_pDlgSelfSay;
//extern DlgZiding* g_pDlgZiding;
//extern DlgShutdown* g_pDlgShutdown;
//extern DlgRemote* g_pDlgRemote;
//extern DlgTalk* g_pDlgTalk;

//extern DlgVideoShow* g_pDlgVideoShow;
//extern DlgPicShow* g_pDlgPicShow;
// mothod
int g_nGetRandom(int nMin, int nMax);
QString g_strGetMD5(QString strTxt);
void g_log(QString strOut);
void g_clearDir(QString strDir);
//void g_objSetBackground(QWidget* pObj, QString strFilePath);

#endif // TMGLOBALTOOL_H
