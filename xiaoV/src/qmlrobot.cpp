#include "qmlrobot.h"
#include "tmglobaltool.h"
#include "robot.h"

WebServer* g_webServer = 0;

QmlRobot::QmlRobot(QObject *parent) :
    QObject(parent),
    m_ptrWebServer(0),
    m_ptrRobot(0),
    m_pageJsName(""),
    m_voiceHandler(0)
{
    m_ptrWebServer = new WebServer(this);
    m_ptrRobot = new Robot(this);


    connect(g_pTmSerial,SIGNAL(sig_tmserial_cruise_info(int,int,double,double,double)),
            this,SIGNAL(sig_qmlrobot_cruise_info(int,int,double,double,double)));

    connect(g_pRobot,SIGNAL(sig_ui_background_set(QString,bool,QString)),SIGNAL(sig_qmlrobot_ui_background_set(QString,bool,QString)));

    connect(g_pRobot,SIGNAL(sig_ui_video_show(QString)),SIGNAL(sig_qmlrobot_ui_video_show(QString)));





    connect(this,SIGNAL(pageJsNameChanged(QString)),m_ptrRobot,SLOT(slot_load_pageEngine(QString)));
    connect(m_ptrRobot,SIGNAL(sig_ui_jump_name(bool)),this,SLOT(slot_pageName_changed(bool)));
    connect(m_ptrRobot,SIGNAL(sig_compass_info(double,double,double)),this,SIGNAL(sig_compass_info(double,double,double)));
    connect(m_ptrRobot,SIGNAL(sig_robot_read_wheel_code_info(int,int,int)),SLOT(slot_qmlrobot_wheelcode_info(int,int,int)));

    connect(m_ptrRobot,SIGNAL(sig_robot_wheel_stop_normal_status(bool)),this,SIGNAL(sig_qmlrobot_wheel_stop_normal_status(bool)));

    QString m_configFile = WebServer::searchConfigFile();
    // Configure template loader and cache
    QSettings* m_templateSettings = new QSettings(m_configFile,QSettings::IniFormat,this);
    m_templateSettings->beginGroup("templates");
    VoiceRequestHandler* xx = new VoiceRequestHandler();
    m_voiceHandler = new VoiceConnectionHandler();
    connect(this,SIGNAL(sig_change_page_by_name(QString)),m_voiceHandler,SLOT(slot_startTimer(QString)));


}

QmlRobot::~QmlRobot()
{
    if(m_ptrWebServer!= Q_NULLPTR)
    {
        m_ptrWebServer->deleteLater();
    }
    if(m_ptrRobot != Q_NULLPTR)
    {
        m_ptrRobot->deleteLater();
    }

}

void QmlRobot::startListen()
{
    emit sig_change_page_by_name("sdfsdfsdfd");


}

void QmlRobot::startSay(const QString Jsname)
{
    g_pRobot->audio_play_tts(Jsname);
}

void QmlRobot::asking(bool isAsking)
{
    if(isAsking){
        m_ptrRobot->listening();
    }else{
        m_ptrRobot->thinking();
    }
}

void QmlRobot::talking(bool isTalking)
{
    if(isTalking){
        m_ptrRobot->listening();
    }else{
        m_ptrRobot->thinking();
    }

}

void QmlRobot::moveBehavior(bool isUp, int distance, int speed)
{
    g_pRobot->motor_wheel_move(isUp,distance,speed);
}

void QmlRobot::rotateBehavior(bool isRight, int angle, int speed)
{
    g_pRobot->motor_wheel_rotate(isRight,angle,speed);

}

void QmlRobot::moveDiffBehavior(bool bForward, int leftSpeed, int rightSpeed)
{
    g_pRobot->motor_wheel_diff_run(bForward,leftSpeed,rightSpeed);
}

void QmlRobot::moveDiffBehaviorAdvance( int leftSpeed, int rightSpeed)
{
    bool t_leftForward = true;
    bool t_rightForward = true;

    int t_leftSpeed = qAbs(leftSpeed);
    int t_rightSpeed = qAbs(rightSpeed);

    if(leftSpeed < 0){
        t_leftForward = false;
    }
    if(rightSpeed < 0){
        t_rightForward = false;
    }

    g_pTmSerial->motor_wheel_diff_run((t_leftForward?0:1),t_leftSpeed,(t_rightForward?1:0),t_rightSpeed);
}

void QmlRobot::moveStart()
{
    g_pTmSerial->motor_wheel_enable (true);

}

void QmlRobot::moveStop()
{

    g_pTmSerial->motor_wheel_stop(false);
}

void QmlRobot::moveEnd()
{
    g_pTmSerial->motor_wheel_stop(true);
}

void QmlRobot::resetWheelCode()
{
    if(g_pTmSerial){
        g_pTmSerial->walk_cmd_wheel_code_reset();
    }

}

void QmlRobot::queryWheelCodeStart()
{
    g_pTmSerial->motor_wheel_code_query_start();

}

void QmlRobot::queryWheelCodeStop()
{
    g_pTmSerial->motor_wheel_code_query_stop();

}

void QmlRobot::queryWheelCode(bool isRight)
{
    if(g_pTmSerial){
        g_pTmSerial->walk_query_wheel_code_info(isRight);
    }

}

void QmlRobot::changePageName(QString pageName)
{
    g_pRobot->ui_jump_name(pageName);

}

void QmlRobot::runJsPageModel(QString jsModelName)
{
    if(g_pJsEngineMain){
        g_pJsEngineMain->run_js_script_by_name(jsModelName);
    }

}

void QmlRobot::pushBehaviorCommand(QByteArray data)
{
//    QQueue<QObject> workLineManager;

//    workLineManager.enqueue(QObject());



}

void QmlRobot::setCompassConfig()
{

    m_ptrRobot->compass_test();

}

QString QmlRobot::pageJsName() const
{
    return m_pageJsName;
}

void QmlRobot::slot_qmlrobot_wheelcode_info(int wheelCode, int leftWheelCode, int rightWheelCode)
{
    emit queryWheelCodeFeedBack(wheelCode,leftWheelCode,rightWheelCode);
}


void QmlRobot::setPageJsName(QString pageJsName)
{

    g_log("current page is" + pageJsName);
    if (m_pageJsName == pageJsName)
        return;

    m_pageJsName = pageJsName;
    emit pageJsNameChanged(pageJsName);
}

void QmlRobot::slot_pageName_changed(bool value)
{
    if(value)
    {
        emit sig_change_page_by_name(m_ptrRobot->m_strModuleName);
    }

}

