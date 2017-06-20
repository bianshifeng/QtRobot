#include "tmupdate.h"
#include <tmglobaltool.h>
#include <QNetworkInterface>

TmUpdateWork::TmUpdateWork(TmUpdate *pWork, QObject *parent) : QObject(parent)
{
    pr_parent = pWork;
    pr_bCheckPass = false;
    pr_pNet = NULL;
}

void TmUpdateWork::initConfig()
{
    QJsonObject objInfo = g_configJson.value("Info").toObject();
    pr_strName = objInfo.value("name").toString();
    pr_strVersion = objInfo.value("version").toString();
    pr_strSN = objInfo.value("sn").toString();
    pr_strDescription = objInfo.value("description").toString();

    objInfo = g_configJson.value("Server").toObject();
    pr_strServerIP = objInfo.value("ip").toString();
    pr_nServerPort = objInfo.value("port").toInt();

    pr_pNet = new QNetworkAccessManager(this);
}

void TmUpdateWork::timeout_login()
{
    if( update_ip_mac() ){
        send_update_info();
    }
}

bool TmUpdateWork::update_ip_mac()
{
    QString strHardID;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface onelist, list)
    {
        QNetworkInterface::InterfaceFlags flags = onelist.flags();
        if( (flags & QNetworkInterface::IsRunning) && (flags & QNetworkInterface::IsLoopBack)==0 ){
            strHardID = onelist.hardwareAddress();
            QList<QNetworkAddressEntry> entryList = onelist.addressEntries();
            //获取IP地址条目列表，每个条目中包含一个IP地址，一个子网掩码和一个广播地址
            foreach(QNetworkAddressEntry entry,entryList)
            {
                QHostAddress host = entry.ip();
                if(host.protocol() == QAbstractSocket::IPv4Protocol){
                    pr_strCPUID = strHardID;
                    pr_strLocalIP = host.toString();
                    //qDebug()<<pr_strCPUID<<pr_strLocalIP;
                    return true;
                }
            }
        }
    }
    return false;
}

bool TmUpdateWork::send_update_info()
{
    // check
    if( pr_bCheckPass == false ){
        // D:/tami/check.dat
        QFile checkFile("D:/tami/check.dat");
        if( checkFile.open(QIODevice::ReadOnly) ){
            //QByteArray bArray = checkFile.readAll();
            QString strHead; // = "Tami Robot Check File.";
            QByteArray bArray;
            QDataStream save(&checkFile);
            save>>strHead;
            save>>bArray;

            pr_bCheckPass = true;
            qDebug()<<"校验完成，通过";
            checkFile.close();
        }else{
            pr_bCheckPass = false;
            qDebug()<<"验证文件不存在，校验不通过";
        }
    }

    if( pr_pNet == NULL ){
        qDebug()<<"Update error! no init http!";
        return false;
    }
    /*
     POST /manage
        {
        "cmd": "dev_heartbeat",
        "dev_type": "HOME_ROBOT",
        "cur_soft_version": "0.1.2",
        "cur_hw_version": "0.3.4",
        "sys_time": "2015-11-21 18:27:37",
        "sys_status": "running",
        "sys_power": "90",
        "dev_location": [123,456],
        "dev_id": "123456789"
        }
     */
    // send update_info
    QJsonDocument docObj;
    QJsonObject msgObj;
    msgObj.insert("cmd","dev_heartbeat");
    msgObj.insert("dev_type","HOME_ROBOT");
    msgObj.insert("cur_soft_version", pr_strVersion);
    msgObj.insert("cur_hw_version", "0.0.0");
    QDateTime dtm = QDateTime::currentDateTime();
    msgObj.insert("sys_time", dtm.toString());
    msgObj.insert("sys_status", g_pDlgMain->m_strCurMode);
    msgObj.insert("sys_power", 100);
    QJsonArray geoArray;
    geoArray.append(0);
    geoArray.append(0);
    msgObj.insert("dev_location", geoArray);
    msgObj.insert("dev_id", pr_strSN);
    msgObj.insert("ip", pr_strLocalIP);

    docObj.setObject(msgObj);
    QByteArray datajson = docObj.toJson();
    //qDebug()<<datajson;

    QString strUrl = QString("http://%1:%2/").arg(pr_strServerIP).arg(pr_nServerPort);
    QNetworkRequest req;
    req.setUrl(QUrl(strUrl));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::ContentLengthHeader,datajson.length());
    pr_pNet->post(req,datajson);
    //qDebug()<<"Update info:"<<strUrl<<datajson.length()<<dtm.toString();
    g_log("Update to "+strUrl);
    return true;
}

// --------------------------------//

TmUpdate::TmUpdate(QObject *parent)
{
    Q_UNUSED(parent);
    pr_work = new TmUpdateWork(this);
    pr_thread = new QThread();
    pr_work->moveToThread(pr_thread);
    pr_tmLogin = new QTimer();
    connect(pr_work, SIGNAL(sig_tmstop()), pr_tmLogin, SLOT(stop()));
    connect(pr_tmLogin, SIGNAL(timeout()), pr_work, SLOT(timeout_login()));
    connect(this, SIGNAL(sig_initConfig()), pr_work, SLOT(initConfig()));
    connect(this, SIGNAL(sig_update_info()), pr_work, SLOT(send_update_info()));
    pr_thread->start();
}

QString TmUpdate::getInfo_SN()
{
    return pr_work->pr_strSN;
}

QString TmUpdate::getInfo_Name()
{
    return pr_work->pr_strName;
}

void TmUpdate::initConfig()
{
    emit sig_initConfig();
    pr_tmLogin->start(25000); // 25s try onece
}


