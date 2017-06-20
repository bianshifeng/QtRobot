#include "robot.h"
#include "tmglobaltool.h"
#include <QByteArray>
#include <QSettings>

Robot::Robot(QObject *parent)
    : QObject(parent),
      m_ptrPluginThread(0)
{
    qDebug() << "Robot";
    m_strCurMode = "loading";
    initLogic();
}

Robot::~Robot()
{
    qDebug() << "~Robot";
    if(m_ptrPluginThread != NULL)
    {
        delete m_ptrPluginThread;
        m_ptrPluginThread = NULL;
    }
    exitapp();

}


void Robot::listening()
{
    if( g_pTmMedia){
        emit g_pTmMedia->sig_cmd_start();
    }

}

void Robot::thinking()
{
    if( g_pTmMedia ){
        emit g_pTmMedia->sig_cmd_stop();
    }
}

void Robot::answer()
{

}

void Robot::compass_test()
{
    g_log("1.---------------void Robot::compass_test()");
//    if(m_ptrNaviCompass)
//        m_ptrNaviCompass


}

void Robot::slot_answer_result(QString result)
{
    if(result.isEmpty())
    {
        g_pRobot->audio_play_tts("我没有听懂啊？");
    }
    else
    {
        if( g_pJsEngineMgr ){
            emit g_pJsEngineMgr->sig_js_event("isr", result);
        }


    }

}

void Robot::slot_load_pageEngine(QString pageName)
{
    loadPageJsEngine(pageName);
}

void Robot::initLogic()
{

    g_pDlgMain =  this;

    g_strExeRoot = qApp->applicationDirPath();
    QDir::setCurrent(g_strExeRoot);
    g_log("ExeRootDir: " + g_strExeRoot);

    //load json config
    QString configPath = g_strExeRoot + "/settings/robot.json";
    QFile configFile(configPath);
    if(!configFile.exists())
    {
        g_log("config file not exist!");
        return;
    }
    if(configFile.open(QIODevice::ReadOnly))
    {
        QByteArray t_strJson = configFile.readAll();
        QJsonDocument t_jsDoc;
        QJsonParseError t_jsErr;

        t_jsDoc = QJsonDocument::fromJson(t_strJson,&t_jsErr);
        if(t_jsErr.error == QJsonParseError::NoError)
        {
            g_configJson = t_jsDoc.object();
        }else{
            g_log("robot.json error!" + t_jsErr.errorString());
            return;
        }
    }

    // clear tts files
    g_clearDir(g_strExeRoot + "/data/tts");

    // init utils object

    //执行所有的机器人命令的全局变量
    g_pRobot = new TmRobotInterface(this);
    g_pRobot->initConfig();
    connect(g_pRobot,SIGNAL(sig_ui_jump_name(bool)),this,SIGNAL(sig_ui_jump_name(bool)));

    //标准功能：进行所有串口通讯，完成机器人机械指令的全局变量
    g_pTmSerial = new TmSerial(this);
    if(g_pTmSerial->initConfig())
    {
        connect(g_pTmSerial,SIGNAL(sig_read_wheel_code_info(int,int,int)),this,SIGNAL(sig_robot_read_wheel_code_info(int,int,int)));

        connect(g_pTmSerial,SIGNAL(sig_tmserial_compass_info(double,double,double)),SIGNAL(sig_compass_info(double,double,double)));
        connect(g_pTmSerial,SIGNAL(sig_tmserial_wheel_stop_normal_status(bool)),
                SIGNAL(sig_robot_wheel_stop_normal_status(bool)));
    }

    //标准功能：进行语音播放，完成机器人语言指令的全局变量
    g_pTmMedia = new TmMedia(this);
    g_pTmMedia->initConfig();
    connect(g_pTmMedia,SIGNAL(sig_isr_result(QString)),
            this,SLOT(slot_answer_result(QString)));


    //标准功能：解析js脚本的全局变量，完成机器人复杂脚本控制的全局变量
    g_pJsEngineMgr = new TmJsEngine(this);
    g_pJsEngineMgr->initConfig();

    g_pRemote = new TmRemote(this);
    g_pRemote->initConfig();


    g_pUpdate = new TmUpdate(this);\
    g_pUpdate->initConfig();
}

void Robot::exitapp()
{
    if(g_pJsEngineMgr!=NULL)
    { g_pJsEngineMgr->exit(); }

    if(g_pCamera!=NULL)
    { g_pCamera->exit(); }

    if(g_pTmMedia!=NULL)
    { g_pTmMedia->exit(); }

    if(g_pTmMedia!=NULL)
    { g_pTmMedia->exit(); }

    if(g_pTmSerial!=NULL)
    { g_pTmSerial->exit(); }

    QThread::msleep(300);
}


void Robot::slot_angle_info(double fYoll, double fPitch, double fYaw)
{

    static double history_Yaw = 0;
    static double diff_Yaw =0;
    static double navi_fYoll,navi_fPitch,navi_fYaw =0;
    navi_fYoll = fYoll;
    navi_fPitch = fPitch;
    navi_fYaw = -fYaw;
    diff_Yaw = navi_fYaw - history_Yaw;
    history_Yaw = navi_fYaw;
    qDebug()<<"RobotAngle:"<<navi_fYaw;
    qDebug()<<"RobotAngle diff:"<<diff_Yaw;

}




void Robot::initLogicOld()
{
    // init global
    g_pDlgMain =  this;

    g_strExeRoot = qApp->applicationDirPath();
    QDir::setCurrent(g_strExeRoot);
    g_log("ExeRootDir: " + g_strExeRoot);

    //load json config
    QString configPath = g_strExeRoot + "/settings/robot.json";
    QFile configFile(configPath);
    if(!configFile.exists())
    {
        g_log("config file not exist!");
        return;
    }
    if(configFile.open(QIODevice::ReadOnly))
    {
        QByteArray t_strJson = configFile.readAll();
        QJsonDocument t_jsDoc;
        QJsonParseError t_jsErr;

        t_jsDoc = QJsonDocument::fromJson(t_strJson,&t_jsErr);
        if(t_jsErr.error == QJsonParseError::NoError)
        {
            g_configJson = t_jsDoc.object();
        }else{
            g_log("robot.json error!" + t_jsErr.errorString());
            return;
        }
    }

    // clear tts files
    g_clearDir(g_strExeRoot + "/data/tts");




    // init utils object

    // init tts
    //所有有tts进行处理的语音对话，都传回来给jsengine
    // TODO: 将这个功能分包进机器人对象里更为合理，作为机器人的说觉反馈
    g_pTmMedia = new TmMedia();
    g_pTmMedia->initConfig();
    connect(g_pTmMedia, SIGNAL(sig_answer_result(QString)),this,SLOT(slot_tuling_result(QString)));
    connect(g_pTmMedia,SIGNAL(sig_isr_result(QString)),this,SLOT(slot_audio_result(QString)));




    // init serial
    //所有有tts进行处理的语音对话，都传回来给jsengine
    // TODO: 将这个功能分包进机器人对象里更为合理，作为机器人的肢体通讯反馈
    g_pTmSerial = new TmSerial();
    g_pTmSerial->initConfig();

    //init camera
    //完成视频的处理
    // TODO: 将这个功能分包进机器人对象里更为合理，作为机器人的视觉
    g_pCamera = new TmCameraWork();
    g_pCamera->initConfig();

    g_pJsEngineMgr = new TmJsEngine();
    g_pJsEngineMgr->initConfig();

    connect(g_pJsEngineMgr,SIGNAL(sig_js_event(QString,QString)),g_pJsEngineMain,SLOT(js_event_call(QString,QString)));
    connect(g_pJsEngineMgr, SIGNAL(sig_js_event(QString,QString)),g_pJsEngineModule,SLOT(js_event_call(QString,QString)));

    g_pRemote = new TmRemote();
    g_pRemote->initConfig();

    g_pUpdate = new TmUpdate();
    g_pUpdate->initConfig();

    // kinect
//    g_pKinect = new TmKinect();
//    g_pKinect->initConfig();

    // plugins
    g_pPluginTools = new TmPluginTools();
    m_ptrPluginThread = new QThread();
    g_pPluginTools->moveToThread(m_ptrPluginThread);
    m_ptrPluginThread->start();
    g_pPluginTools->initConfig();

    // emit init ok
    emit sig_init_success();

}

void Robot::loadPageJsEngine(const QString pageName)
{
    if(!pageName.isEmpty())
    {
        g_pJsEngineMgr->load_js_main_by_name(pageName);
    }

}

void Robot::pwd_check_return(bool bIsOk)
{
    if(bIsOk)
    {
        if(this->m_strModuleName == "stand")
        {
            g_log("Jump to stand module");
            g_pJsEngineMain->run_js_script_by_name("stand");
        }
        if(this->m_strModuleName == "selfsay")
        {
            g_log("Jump to selfsay module!");
            g_pJsEngineMain->run_js_script_by_name("selfsay");
        }

    }

}


void Robot::loading_over()
{

    g_log("Loading Over!");

    QString t_strIniPath = g_strExeRoot + "/settings/root.ini";
    QSettings t_settings(t_strIniPath,QSettings::IniFormat);
    t_settings.setIniCodec("UTF-8");

    int t_nVol = t_settings.value("Settings/volume",100).toInt();
    emit this->sig_set_video_volume(t_nVol);

    g_pTmSerial->start_sensor_chaosheng(true);
    g_pRobot->ui_jump_name("main");
}

void Robot::slot_tuling_result(QString result)
{
    if(g_pJsEngineMgr)
    {
        emit g_pJsEngineMgr->sig_js_event("tuling",result);
    }

}

void Robot::slot_touch_result(QString result)
{
    if( g_pJsEngineMgr)
    {
        emit g_pJsEngineMgr->sig_js_event("touch",result);

    }
}

void Robot::slot_audio_result(QString result)
{
    if( g_pJsEngineMgr )
    {
        //qDebug() << "audio result" << result.toStdString().data();
        emit g_pJsEngineMgr->sig_js_event("isr",result);
    }

}
