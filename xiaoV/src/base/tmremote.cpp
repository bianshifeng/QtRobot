#include "tmremote.h"
#include <tmglobaltool.h>

TmRemote::TmRemote(QObject *parent) : QObject(parent)
{

}

void TmRemote::initConfig()
{
    pr_pTmCamera = new QTimer();
    pr_pSockCamera = new QTcpSocket();
    connect(pr_pTmCamera, SIGNAL(timeout()), this, SLOT(msg_camera_send()));

    pr_pTcpControl = new QTmTcpControl();
    pr_pAudioServer = new QTmTCPVoiceServer();
    pr_objRoot = g_configJson.value("Remote").toObject();
    pr_pTcpControl->startServer(pr_objRoot.value("control_port").toInt());
    connect(pr_pTcpControl, SIGNAL(signal_msg_cmd(QString,QJsonValue)), this, SLOT(msg_control_cmd(QString,QJsonValue)));
    connect(pr_pTcpControl, SIGNAL(signal_cur_disconnect()), this, SLOT(msg_control_discon()));

    pr_pAudioServer->initConfig();
}

bool TmRemote::enableSendCamera(bool bEnable)
{
    if( !bEnable ){
        pr_pTmCamera->stop();
        if( pr_pSockCamera->state() == QAbstractSocket::ConnectedState ){
            pr_pSockCamera->close();
        }
        return true;
    }else{
        QAbstractSocket::SocketState curstate = pr_pSockCamera->state();
        if( curstate == QAbstractSocket::UnconnectedState ){
            int nTryCount = 10;
            while( nTryCount-- ){
                curstate = pr_pSockCamera->state();
                qDebug()<<curstate;
                if( curstate == QAbstractSocket::UnconnectedState ){
                    pr_pSockCamera->connectToHost(QHostAddress(pr_pTcpControl->getRemoteAddr()), pr_objRoot.value("image_port").toInt());
                }
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                QThread::msleep(600);
                if( curstate == QAbstractSocket::ConnectedState ){
                    break;
                }
            }
            // error
            if( nTryCount<=0 || curstate == QAbstractSocket::UnconnectedState ){
                return false;
            }
        }
        if( curstate == QAbstractSocket::ConnectedState ){
            pr_pTmCamera->start(200);
            return true;
        }
    }
    return false;
}

void TmRemote::enableVoiceSend(bool bEnable)
{
    if( pr_pAudioServer ){
        emit pr_pAudioServer->sig_enableAudioSend(bEnable);
    }
}

void TmRemote::enableVoicePlayer(bool bEnable)
{
    if( pr_pAudioServer ){
        emit pr_pAudioServer->sig_enableAudioReceive(bEnable);
    }
}

void TmRemote::msg_control_cmd(QString strCmd, QJsonValue val)
{
    qDebug()<<strCmd;
    if( strCmd == TM_CMD_Video ){
        if( val.toBool() ){
            enableSendCamera(true);
        }else{
            enableSendCamera(false);
        }
    }
    if( strCmd == TM_CMD_Audio ){ // 只打开音频发送
        if( val.toBool() ){
            enableVoiceSend(true);
        }else{
            enableVoiceSend(false);
        }
    }
    if( strCmd == TM_CMD_Say ){ // 只打开音频接收, 相当于开启远程模式
        if( val.toBool() ){
            enableVoicePlayer(true);
            g_pRobot->ui_jump_name("remote");
        }else{
            enableVoicePlayer(false);
            g_pRobot->ui_jump_name("main");
        }
    }
    if( strCmd == TM_CMD_Mode_Get ){ // 获取模式
        if( val.toBool() ){
            pr_pTcpControl->write_ack(m_package.make_json_ack(true,TM_CMD_Mode_Get,g_pDlgMain->m_strCurMode));
        }
    }
    if( strCmd == TM_CMD_Mode_Set ){ // 设置模式
        QString strMode = val.toString();
        if( strMode == "mode1" ){
//            g_pDlgMode->m_strScriptName = "mode1";
            g_pRobot->ui_jump_name("modeshow");
        }else
        if( strMode == "mode2" ){
//            g_pDlgMode->m_strScriptName = "mode2";
            g_pRobot->ui_jump_name("modeshow");
        }else{
            g_pRobot->ui_jump_name(strMode);
        }


    }
    if( strCmd == TM_CMD_Wheel ){ // 设置模式
        if( !val.isArray() ){
            g_log("motor_cmd_wheel params is not array!");
            return;
        }
        QJsonArray jsArry = val.toArray();
        //qDebug()<<"cmd"<<jsArry.at(0).toString()<<jsArry.at(1).toBool();
        QString strAction = jsArry.at(0).toString();
        if( strAction == "enable" ){
            bool bEnable = jsArry.at(1).toBool();
            g_pRobot->motor_wheel_enable (bEnable);
        }else if( strAction == "stop" ){
            bool bEnable = jsArry.at(1).toBool();
            g_pRobot->motor_wheel_stop (bEnable);
        }else if( strAction == "move" ){
            bool bUp = jsArry.at(1).toBool();
            int nDistane = jsArry.at(2).toInt ();
            int nSpd = jsArry.at(3).toInt ();
            g_pRobot->motor_wheel_move (bUp, nDistane, nSpd);
        }else if( strAction == "rotate" ){
            bool bRight = jsArry.at(1).toBool();
            int nDistane = jsArry.at(2).toInt ();
            int nSpd = jsArry.at(3).toInt ();
            g_pRobot->motor_wheel_rotate (bRight, nDistane, nSpd);
        }
    }
    if( strCmd == TM_CMD_Action ){ // 执行动作脚本
        QString strAction;
        if( val.isArray() ){
            QJsonArray jsArry = val.toArray();
            strAction = jsArry.at(0).toString();
            int nSleep = jsArry.at(1).toInt();
            if( nSleep>0 ){
                QThread::msleep(nSleep);
            }
        }else{
            strAction = val.toString();
        }
        qDebug()<<"Call action:"<<strAction;
        if( strAction == "random" ){
            g_pRobot->json_load_by_name("kaixin");
        }else{
            g_pRobot->js_sub_load_by_name(strAction);
        }
    }
    if( strCmd == TM_CMD_TTS ){ // 执行动作脚本
        QString strTTS = val.toString();
        if( strTTS.length() > 0 ){
            qDebug()<<"Call TTS:"<<strTTS;
            g_pRobot->audio_play_tts(strTTS);
            g_pRobot->behavior_auto_action(true);
        }
    }
    if( strCmd == TM_CMD_Script ){ // 执行主脚本
        QString strScript = val.toString();
        if( strScript.length() > 0 ){
            qDebug()<<"Call Script:"<<strScript;
            g_pRobot->js_main_load_by_name(strScript);
        }
    }
    if( strCmd == TM_CMD_Face ){ // 执行动作脚本
        if( !val.isArray() ){
            g_log("motor_cmd_face params is not array!");
            return;
        }
        QJsonArray jsArry = val.toArray();
        int nID = jsArry.at(0).toInt();
        int nIndex = jsArry.at(1).toInt();
        g_pRobot->led_face_set_up(nID);
        g_pRobot->led_face_set_by_id(nIndex);
    }
    if( strCmd == TM_CMD_Head ){ //
        QJsonArray jsArry = val.toArray();
        if( jsArry.size() >= 3 ){
            int nIndex = jsArry.at(0).toInt();
            int nAngle = jsArry.at(1).toInt();
            int nSpeed = jsArry.at(2).toInt();
            if( nIndex == -1 ){
                g_pRobot->motor_head_LR(nAngle, nSpeed);
                g_pRobot->motor_head_UD(nAngle, nSpeed);
            }
            if( nIndex == 0 ){
                g_pRobot->motor_head_UD(nAngle, nSpeed);
            }
            if( nIndex == 1 ){
                g_pRobot->motor_head_LR(nAngle, nSpeed);
            }
        }
    }
}

void TmRemote::msg_control_discon()
{
    enableSendCamera(false);
    enableVoicePlayer(false);
    enableVoiceSend(false);
    if( g_pDlgMain->m_strCurMode == "remote" ){
        g_pRobot->ui_jump_name("main");
    }
}

void TmRemote::msg_camera_send()
{
    QByteArray bJpeg;
    if(g_pCamera->getCameraJpeg(bJpeg)){
        pr_pSockCamera->write(m_package.make_full_package_video(bJpeg));
    }
}
