#include "tmglobaltool.h"

QString g_strExeRoot = ".";
QJsonObject g_configJson;

TmSerial* g_pTmSerial = NULL;
TmMedia* g_pTmMedia = NULL;
TmRobotInterface* g_pRobot = NULL;
TmJsEngineWork* g_pJsEngineMain = NULL;
TmJsEngineWork* g_pJsEngineModule = NULL;
TmJsEngine* g_pJsEngineMgr = NULL;
TmCameraWork* g_pCamera = NULL;
TmRemote* g_pRemote = NULL;
TmUpdate* g_pUpdate = NULL;
TmKinect* g_pKinect = NULL;
TmPluginTools* g_pPluginTools = NULL;

Robot* g_pDlgMain = NULL;

//DlgMain* g_pDlgMain = NULL;
//DlgLoading* g_pDlgLoading = NULL;
//DlgPwd* g_pDlgPwd = NULL;
//DlgStand* g_pDlgStand = NULL;
//DlgYYY* g_pDlgYYY = NULL;
//DlgSelfSay* g_pDlgSelfSay = NULL;
//DlgZiding* g_pDlgZiding = NULL;
//DlgShutdown* g_pDlgShutdown = NULL;
//DlgRemote* g_pDlgRemote = NULL;
//DlgConfig* g_pDlgConfig = NULL;

//DlgVideoShow* g_pDlgVideoShow = NULL;
//DlgPicShow* g_pDlgPicShow = NULL;
//DlgTalk* g_pDlgTalk = NULL;

//-------------------------------------//
int g_nGetRandom(int nMin, int nMax)
{
    if( (nMax-nMin) == 0 ){
        return nMin;
    }
    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    //int n = (qrand()%((nMax-nMin+1)*100)) / 100 + nMin;
    int n = qrand()%(nMax-nMin+1) + nMin;
    return n;
}

QString g_strGetMD5(QString strTxt)
{
    QString strMD5;
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(strTxt.toUtf8());
    QByteArray bArry = md5.result();
    QString strChar;
    for( int i=0;i<bArry.size();i++ ){
        char cNum = bArry.at(i);
        strChar.sprintf("%02X", (uchar)cNum);
        strMD5 += strChar;
    }
    return strMD5;
}

void g_log(QString strOut){
    QTime tm = QTime::currentTime();
    QString logTxt = tm.toString("[HH:mm:ss.zzz] ") + strOut;
    qDebug()<<logTxt.toStdString().data();
}

void g_clearDir(QString strDir)
{
    QDir dirRoot(strDir);
    if( dirRoot.exists() ){
        dirRoot.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QFileInfoList fileList = dirRoot.entryInfoList();
        foreach (QFileInfo fi, fileList)
        {
            dirRoot.remove(fi.fileName());
        }
    }
}


//void g_objSetBackground(QWidget *pObj, QString strFilePath)
//{
//    QPixmap png;
//    png.load(strFilePath);
//    if( png.isNull() ){
//        return;
//    }
//    pObj->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Background, QBrush(png));
//    pObj->setPalette(palette);
//}
