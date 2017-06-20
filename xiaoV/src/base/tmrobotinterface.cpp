#include "tmrobotinterface.h"
#include <tmglobaltool.h>


TmRobotInterface::TmRobotInterface(QObject *parent) : QObject(parent)
{
    connect(&pr_tmLookAt, SIGNAL(timeout()), this, SLOT(timeout_lookat()));
    connect(this,SIGNAL(sig_ui_jump_name(bool)),g_pDlgMain, SLOT(pwd_check_return(bool)));
}

void TmRobotInterface::log(QString strOut)
{
    g_log(strOut);
}

void TmRobotInterface::msleep(int nMinSec)
{
    QThread::msleep(nMinSec);
}

int TmRobotInterface::nrandom(int nMin, int nMax)
{
    return g_nGetRandom(nMin, nMax);
}

int TmRobotInterface::sensor_camera_persons()
{
    if( g_pCamera ){
        int nPersons = g_pCamera->getCameraPersonCounts();
        qDebug()<<"Camera:"<<nPersons;
        return nPersons;
    }
    return 0;
}

int TmRobotInterface::sensor_shengbo_distance(int nID)
{
    if( g_pTmSerial ){
        int nDistance =  g_pTmSerial->sensor_shengbo_distance(nID);
//        qDebug()<<"Shengbo: "<<nID<< nDistance;
        return nDistance;
    }
    return 0;
}

bool TmRobotInterface::sensor_remin_isMoving()
{
    if( g_pTmSerial ){
        bool bMoving =  g_pTmSerial->sensor_remin_isMoving();
        qDebug()<<"Hongwai: "<<bMoving;
        return bMoving;
    }
    return false;
}
int TmRobotInterface::sensor_hongwai_distance(int nID)
{
    if( g_pTmSerial ){
        int nDistance = g_pTmSerial->sensor_hongwai_distance(nID);
        //qDebug()<<"Shengbo: "<<nID<< nDistance;
        return nDistance;
    }
    return 0;
}

void TmRobotInterface::behavior_lookat_open(bool bIsOpen)
{
    if( bIsOpen ){
        timeout_lookat();
        pr_tmLookAt.start(2000);
    }else{
        pr_tmLookAt.stop();
    }
}

void TmRobotInterface::behavior_auto_action(bool bStart)
{
    if( g_pTmSerial ){
        g_pTmSerial->behavior_auto_action(bStart);
    }
}

void TmRobotInterface::behavior_auto_talk_open(bool bStart)
{
    //qDebug()<<"[auto talk]:"<<bStart;
    if( g_pTmMedia ){
        if( bStart ){
            g_pTmMedia->isr_start();
        }else{
            g_pTmMedia->isr_stop();
        }
    }
}

void TmRobotInterface::behavior_auto_talk_pause(bool bPause)
{
    //qDebug()<<"[talk state]:"<<bPause;
    if( g_pTmMedia ){
       g_pTmMedia->isr_pause(bPause);
    }
}

//void TmRobotInterface::behavior_cmd_talk_stop()
//{
//    if( g_pDlgTalk ){
//        qDebug()<<"From scripts";
//        emit g_pDlgTalk->sig_talk_stop();
//    }
//}

void TmRobotInterface::tool_ask_answer(QString strAsk)
{
    if( strAsk.length() > 0 ){
        g_pTmMedia->tuling_answer(strAsk);
    }
}

QList<int> TmRobotInterface::tool_kinect_get_data()
{
    QList<int> retVal;
    if( g_pKinect ){
        retVal = g_pKinect->getBodyInfo();
    }
    return retVal;
}

void TmRobotInterface::tool_kinect_start()
{
    if( g_pKinect ){
        g_pKinect->startRun();
    }
}

bool TmRobotInterface::tool_kinect_is_opened()
{
    if( g_pKinect ){
        return g_pKinect->getIsOpened();
    }
    return false;
}

void TmRobotInterface::tool_kinect_stop()
{
    if( g_pKinect ){
        g_pKinect->stopRun();
    }
}

void TmRobotInterface::motor_head_enable(bool bEnable)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_head_enable(bEnable);
    }
}

void TmRobotInterface::motor_head_zero()
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_head_zero();
    }
}

void TmRobotInterface::motor_head_LR(int nAngle, int nSpd)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_head_LR(nAngle, nSpd);
    }
}

void TmRobotInterface::motor_head_UD(int nAngle, int nSpd)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_head_UD(nAngle, nSpd);
    }
}

void TmRobotInterface::motor_hand_left_enable(bool bEnable)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_hand_left_enable(bEnable);
    }
}

void TmRobotInterface::motor_hand_left_zero()
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_hand_left_zero();
    }
}

void TmRobotInterface::motor_hand_left_Expand(int nAngle, int nSpd)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_hand_left_Expand(nAngle, nSpd);
    }
}

void TmRobotInterface::motor_hand_left_Swing(int nAngle, int nSpd)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_hand_left_Swing(nAngle, nSpd);
    }
}

void TmRobotInterface::motor_hand_right_enable(bool bEnable)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_hand_right_enable(bEnable);
    }
}

void TmRobotInterface::motor_hand_right_zero()
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_hand_right_zero();
    }
}

void TmRobotInterface::motor_hand_right_Expand(int nAngle, int nSpd)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_hand_right_Expand(nAngle, nSpd);
    }
}

void TmRobotInterface::motor_hand_right_Swing(int nAngle, int nSpd)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_hand_right_Swing(nAngle, nSpd);
    }
}

void TmRobotInterface::motor_wheel_enable(bool bEnable)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_wheel_enable (bEnable);
    }
}

void TmRobotInterface::motor_wheel_stop(bool bSlow)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_wheel_stop (bSlow);
    }
}

void TmRobotInterface::motor_wheel_move(bool bUp, int nDistance, int nSpd)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_wheel_move (bUp, nDistance, nSpd);
    }
}

void TmRobotInterface::motor_wheel_rotate(bool bRight, int nAngle, int nSpd)
{
    if( g_pTmSerial ){
        g_pTmSerial->motor_wheel_rotate (bRight, nAngle, nSpd);
    }
}

void TmRobotInterface::motor_wheel_diff_run(bool bForward, int nLeftSpeed, int nRightSpeed)
{
    if( g_pTmSerial ){
        if( nLeftSpeed < 0 ){
            nLeftSpeed = 0;
        }
        if( nLeftSpeed > 100 ){
            nLeftSpeed = 100;
        }
        if( nRightSpeed < 0 ){
            nRightSpeed = 0;
        }
        if( nRightSpeed > 100 ){
            nRightSpeed = 100;
        }
        g_pTmSerial->motor_wheel_diff_run ((bForward?0:1), nLeftSpeed, (bForward?1:0), nRightSpeed);
    }
}

void TmRobotInterface::led_face_set_up(int nFaceID)
{
    if( g_pTmSerial ){
        g_pTmSerial->led_face_set_up(nFaceID);
    }
}

void TmRobotInterface::led_face_set_by_name(QString strName)
{
    if( g_pTmSerial ){
        g_pTmSerial->led_face_set_by_name(strName);
    }
}

void TmRobotInterface::led_face_set_by_id(int nIndex)
{
    if( g_pTmSerial ){
        g_pTmSerial->led_face_set_by_id(nIndex);
    }
}

void TmRobotInterface::led_face_set(int nIndex, int nTmMid, int nTmOver, int nTmFrame, int nNumRepeat)
{
    if( g_pTmSerial ){
        g_pTmSerial->led_face_set(nIndex, nTmMid, nTmOver, nTmFrame, nNumRepeat);
    }
}

void TmRobotInterface::led_body_set(int nR, int nG, int nB, int nDelay)
{
    if( g_pTmSerial ){
        g_pTmSerial->led_body_set(nR, nG, nB, nDelay);
    }
}

void TmRobotInterface::led_head_set(int nR, int nG, int nB, int nDelay)
{
    if( g_pTmSerial ){
        g_pTmSerial->led_head_set(nR, nG, nB, nDelay);
    }
}

void TmRobotInterface::led_touch_set(int nMode, int nColor, int nFreq)
{
    if( g_pTmSerial ){
        g_pTmSerial->led_touch_set(nMode, nColor, nFreq);
    }
}

void TmRobotInterface::led_touch_get()
{
    if( g_pTmSerial ){
        g_pTmSerial->led_touch_get();
    }
}

bool TmRobotInterface::json_load_by_name(QString strName)
{
    if( !g_pTmSerial ){
        return false;
    }
    return g_pTmSerial->json_load_by_name(strName);
}

bool TmRobotInterface::json_load_isRunning()
{
    if( !g_pTmSerial ){
        return false;
    }
    return g_pTmSerial->json_load_isRunning();
}

void TmRobotInterface::json_load_stop()
{
    if( !g_pTmSerial ){
        return;
    }
    g_pTmSerial->json_load_stop();
}

bool TmRobotInterface::js_main_load_by_name(QString strName)
{
    if( !g_pJsEngineMgr ){
        return false;
    }
    return g_pJsEngineMgr->load_js_main_by_name(strName);
}

bool TmRobotInterface::js_main_isRunning()
{
    if( !g_pJsEngineMgr ){
        return false;
    }
    return g_pJsEngineMgr->load_js_main_isRunning();
}

void TmRobotInterface::js_main_stop()
{
    if( g_pJsEngineMgr ){
        g_pJsEngineMgr->load_js_main_stop();
    }
}

bool TmRobotInterface::js_sub_load_by_name(QString strName)
{
    if( !g_pJsEngineMgr ){
        return false;
    }
    g_log(".......................");
    g_log(strName);
    return g_pJsEngineMgr->load_js_module_by_name(strName);
}

bool TmRobotInterface::js_sub_load_content(QString strJsContent)
{
    if( !g_pJsEngineMgr ){
        return false;
    }
    return g_pJsEngineMgr->load_js_module_content(strJsContent);
}

bool TmRobotInterface::js_sub_isRunning()
{
    if( !g_pJsEngineMgr ){
        return false;
    }
    return g_pJsEngineMgr->load_js_module_isRunning();
}

void TmRobotInterface::js_sub_stop()
{
    if( g_pJsEngineMgr ){
        g_pJsEngineMgr->load_js_module_stop();
    }
}

bool TmRobotInterface::audio_play_tts(QString strTxt)
{
    QString strPath = g_pTmMedia->getTTSFilePath(strTxt);
    if( strPath.length() > 1 ){
        g_pTmMedia->playMediaTTS(strPath);
        return true;
    }
    return false;
}

bool TmRobotInterface::audio_play_tts_params(QString strTxt, QString strSpeaker, int nSpeed, int nPitch, int nSpecial)
{
    if( g_pTmMedia ){
        if( strTxt.length() > 1 ){
            return g_pTmMedia->playMediaTTS_Params(strTxt, strSpeaker, nSpeed, nPitch, nSpecial);
        }
    }
    return false;
}

void TmRobotInterface::audio_play_system(QString strName)
{
    if( g_pTmMedia ){
        g_pTmMedia->playMediaAudio(strName);
    }
}

bool TmRobotInterface::audio_play_isPlaying()
{
    if( g_pTmMedia ){
        return g_pTmMedia->isPlaying();
    }
    return false;
}

void TmRobotInterface::audio_stop()
{
    if( g_pTmMedia ){
        g_pTmMedia->stop();
    }
}

void TmRobotInterface::audio_play_system_bg(QString strName)
{
    if( g_pTmMedia ){
        g_pTmMedia->playMediaAudio(strName, 1);
    }
}

bool TmRobotInterface::audio_play_isPlaying_bg()
{
    if( g_pTmMedia ){
        return g_pTmMedia->isPlaying(1);
    }
    return false;
}

void TmRobotInterface::audio_stop_bg()
{
    if( g_pTmMedia ){
        g_pTmMedia->stop(1);
    }
}

void TmRobotInterface::ui_background_set(QString strModeName, bool bShow, QString strFileName)
{

    emit this->sig_ui_background_set(strModeName,bShow,strFileName);
}


void TmRobotInterface::ui_video_show(QString strFilePath)
{
//    if( g_pDlgMain ){
//        emit g_pDlgMain->sig_set_video_show(strFilePath);
//    }
    emit this->sig_ui_video_show(strFilePath);
}

bool TmRobotInterface::ui_video_isPlaying()
{
    if( g_pDlgMain ){
        return g_pDlgMain->m_bIsPlaying;
    }
    return false;
}

void TmRobotInterface::ui_video_stopped()
{
    if( g_pDlgMain ){
        emit g_pDlgMain->sig_set_video_stopped();
    }
}

void TmRobotInterface::ui_pics_btn_show(QString strModeName, bool bShow)
{
    qDebug()<<strModeName<<bShow;
    if( strModeName == "stand" ){
        emit g_pDlgMain->pic_btn_show(bShow);
    }
    if( strModeName == "yyy" ){
        emit g_pDlgMain->pic_btn_show_yyy(bShow);
    }
}

void TmRobotInterface::ui_pics_btn_name(QString strModeName, QString strName)
{
    if( strModeName == "stand" ){
        emit g_pDlgMain->pic_btn_name(strName);
    }
    if( strModeName == "yyy" ){
        emit g_pDlgMain->pic_btn_name_yyy(strName);
    }
}

void TmRobotInterface::ui_pics_show(QString strFilePath)
{
    if( g_pDlgMain ){
        emit g_pDlgMain->sig_pics_show(strFilePath);
    }
}

void TmRobotInterface::ui_pics_hide()
{
    if( g_pDlgMain ){
        emit g_pDlgMain->sig_pics_hide();
    }
}

void TmRobotInterface::ui_jump_name(QString strName)
{
    if( g_pDlgMain ){
        g_pDlgMain->m_strModuleName = strName;
        emit sig_ui_jump_name(true);
    }
}

QString TmRobotInterface::config_file_get(QString fileName, QString strRootKey)
{
    //QString strFile = QString("./user/scripts/%1.ini").arg(fileName);
    QSettings setIni(fileName, QSettings::IniFormat);
    setIni.setIniCodec("UTF-8");
    return setIni.value(strRootKey).toString();
}

void TmRobotInterface::config_file_set(QString fileName, QString strRootKey, QString val)
{
    //QString strFile = QString("./user/scripts/%1.ini").arg(fileName);
    QSettings setIni(fileName, QSettings::IniFormat);
    setIni.setIniCodec("UTF-8");
    setIni.setValue(strRootKey, val);
}

void TmRobotInterface::system_set_volume(int nVolume)
{
    if( g_pTmMedia ){
        g_pTmMedia->setVolume(nVolume, 0);
    }
    if( g_pDlgMain ){
        emit g_pDlgMain->sig_set_video_volume(nVolume);
    }
    QString strIniPath = g_strExeRoot+"/settings/robot.ini";
    QSettings settings(strIniPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("Settings/volume", nVolume);
}

void TmRobotInterface::exscreen_pic_dir(QString strDirName)
{
    if( g_pPluginTools ){
        emit g_pPluginTools->sig_exscreen_pic_dir(strDirName);
    }
}

void TmRobotInterface::exscreen_pic_file(QString strFileFullName)
{
    if( g_pPluginTools ){
        emit g_pPluginTools->sig_exscreen_pic_file(strFileFullName);
    }
}

void TmRobotInterface::exscreen_video_dir(QString strDirName)
{
    if( g_pPluginTools ){
        emit g_pPluginTools->sig_exscreen_video_dir(strDirName);
    }
}

void TmRobotInterface::exscreen_video_file(QString strFileFullName)
{
    if( g_pPluginTools ){
        emit g_pPluginTools->sig_exscreen_video_file(strFileFullName);
    }
}

void TmRobotInterface::timeout_lookat()
{
    if( g_pCamera ){
        if( g_pCamera->getCameraPersonCounts()>0 ){
            int pos[2] = {0};
            int nMaxX = 30;
            int nMaxY = 20;
            g_pCamera->getCameraPersonCenter(pos);
            motor_head_LR(pos[0]*nMaxX/100, 2);
            motor_head_UD(pos[1]*nMaxY/100, 2);
        }
    }
}

void TmRobotInterface::initConfig()
{

}

