#include "tmserial.h"
#include <tmglobaltool.h>
#include <QtAlgorithms>
#include <QtGlobal>

static double Compass_Yaw_Start_Angle = 0;
static double Compass_Yaw_Current_Angle = 0;

static double Compass_Current_Roll = 0;
static double Compass_Current_Pitch = 0;
static double Compass_Current_Yaw = 0;
static bool   Robot_Move_status = true;
static int Robot_Left_WheelCode_Num = 0;
static int Robot_Right_WheelCode_Num = 0;


static int S_DOUBLE_VALUE_COUNT = 0;


TmSerial::TmSerial(QObject *parent)
    : QObject(parent)

{
    pr_pSerWork = NULL;
    pr_nFaceID = 0;
    m_ptrNaviCompass = 0;
}

bool TmSerial::initConfig()
{
    if(!this->initMainSerial()){
        return false;
    }

    if(!this->initCompassSerial()){
        return false;
    }

    if(!this->initEnginJson()){
        return false;
    }

    return true;
}

bool TmSerial::initMainSerial()
{
    // init work in thread
    pr_pSerWork = new TmSerialWork();
    connect(this, SIGNAL(writeSerialData(QByteArray)), pr_pSerWork, SLOT(writeSerialData(QByteArray)));
    connect(pr_pSerWork, SIGNAL(sig_event_touch(QString)), g_pDlgMain, SLOT(slot_touch_result(QString)));
    connect(pr_pSerWork, SIGNAL(sig_serial_data(QByteArray)),this,SIGNAL(sig_serial_data(QByteArray)));
    connect(pr_pSerWork, SIGNAL(sig_serial_data_for_wheelcode_query(QByteArray)),this,SLOT(slot_wheelcode_feedback_info(QByteArray)));

    connect(pr_pSerWork,SIGNAL(sig_serial_data_for_wheel_stop(QByteArray)),this,SLOT(slot_wheel_feedback_stop(QByteArray)));
    QJsonObject root = g_configJson.value("Motor").toObject();
    pr_pSerWork->initSerial(root.value("port").toString());


    m_timerWheelCodeQuery = new QTimer();
    connect(m_timerWheelCodeQuery,SIGNAL(timeout()),SLOT(slot_wheelcode_query_info()));
    return true;
}

bool TmSerial::initCompassSerial()
{
    m_ptrNaviCompass = new TMNaviAngle(this);
    connect(m_ptrNaviCompass,SIGNAL(sig_angle(double,double,double)),SLOT(slot_compass_feedback_info(double,double,double)));

    if(!m_ptrNaviCompass->initConfig()) return false;
    m_ptrNaviCompass->slot_start();

    return true;


}

bool TmSerial::initEnginJson()
{
    // load motor params
    QJsonObject root = g_configJson.value("Motor").toObject();
    QJsonArray motorArr = root.value("head_ud").toArray();
    _head_ud[0] = motorArr.at(0).toInt();
    _head_ud[1] = motorArr.at(1).toInt();
    _head_ud[2] = motorArr.at(2).toInt();
    motorArr = root.value("head_lr").toArray();
    _head_lr[0] = motorArr.at(0).toInt();
    _head_lr[1] = motorArr.at(1).toInt();
    _head_lr[2] = motorArr.at(2).toInt();
    motorArr = root.value("hand_left_expand").toArray();
    _hand_left_e[0] = motorArr.at(0).toInt();
    _hand_left_e[1] = motorArr.at(1).toInt();
    _hand_left_e[2] = motorArr.at(2).toInt();
    motorArr = root.value("hand_left_swing").toArray();
    _hand_left_s[0] = motorArr.at(0).toInt();
    _hand_left_s[1] = motorArr.at(1).toInt();
    _hand_left_s[2] = motorArr.at(2).toInt();
    motorArr = root.value("hand_right_expand").toArray();
    _hand_right_e[0] = motorArr.at(0).toInt();
    _hand_right_e[1] = motorArr.at(1).toInt();
    _hand_right_e[2] = motorArr.at(2).toInt();
    motorArr = root.value("hand_right_swing").toArray();
    _hand_right_s[0] = motorArr.at(0).toInt();
    _hand_right_s[1] = motorArr.at(1).toInt();
    _hand_right_s[2] = motorArr.at(2).toInt();

    connect(this, SIGNAL(sig_json_engine(bool)), this, SLOT(slot_json_engine(bool)));
    connect(&pr_tmSayAutoAction, SIGNAL(timeout()), this, SLOT(slot_say_auto_action()));
    connect(this, SIGNAL(sig_say_auto_control(bool)), this, SLOT(slot_say_control(bool)));
    // load json script
    pr_jsonMap.clear();
    QFile file("./data/base/stand_actions.json");
    QByteArray bArrJson;
    if( file.open(QIODevice::ReadOnly) ){
        bArrJson = file.readAll();
        file.close();
    }
    QJsonDocument jsDoc;
    QJsonParseError jsError;
    jsDoc = QJsonDocument::fromJson(bArrJson, &jsError);
    if( jsError.error != QJsonParseError::NoError ){
        g_log("Parser stand_actions.json error!"+jsError.errorString());
        return false;
    }
    QJsonObject obj = jsDoc.object();
    QStringList strList = obj.keys();
//    for( int i=0; i<strList.size(); i++ ){
//        QString strName = strList.at(i);
//        QJsonObject valObj = obj.value(strName).toObject();
//        pr_jsonMap.insert(strName, valObj.value("data").toArray());
//        g_log("Load stand action: "+strName+" "+valObj.value("name").toString());
//    }
    for( int i=0; i<strList.size(); i++ ){
        QString strName = strList.at(i);
        QString strNameClass = strName.left(strName.lastIndexOf('_'));
        QMap<QString, QJsonArray>::const_iterator arryObj = pr_jsonMap.find(strNameClass);
        QJsonArray jsArry;
        if( arryObj == pr_jsonMap.end() ){
            jsArry = QJsonArray();
//            qDebug()<<"Create new"<<strNameClass;
        }else{
            jsArry = arryObj.value();
        }
        jsArry.append(obj.value(strName));
        pr_jsonMap.insert(strNameClass, jsArry);
//        qDebug()<<"Update map:"<<strNameClass<<strName;
    }

    connect(&pr_enginJson, SIGNAL(timeout()), this, SLOT(slot_engineJson_timeout()));
    connect(&pr_sensor_chaosheng, SIGNAL(timeout()), this, SLOT(slot_sensor_chaosheng()));
    return true;
}



void TmSerial::exit()
{
    pr_enginJson.stop();
    pr_pSerWork->exit();
    m_ptrNaviCompass->deleteLater();
}

void TmSerial::start_sensor_chaosheng(bool bOpen)
{
    if( bOpen ){
        pr_sensor_chaosheng.start(1000);
    }else{
        pr_sensor_chaosheng.stop();
    }
}

int TmSerial::sensor_shengbo_distance(int nId)
{
    if( nId > -1 && nId<3 && pr_pSerWork){
        //qDebug()<<"Shengbo:"<<pr_pSerWork->m_nPersonDistance[0]<<pr_pSerWork->m_nPersonDistance[1];
        int nVal = pr_pSerWork->m_nPersonDistance[nId];
        if( nVal > 32767 ){
            nVal = 0;
        }
        return nVal;
    }
    return 0;
}

int TmSerial::sensor_hongwai_distance(int nId)
{
    if( nId > -1 && nId<5 && pr_pSerWork){
        int nVal = pr_pSerWork->m_nHongWaiDistance[nId];
        if( nVal > 100 ){
            nVal = 0;
        }
        return nVal*10;
    }
    return 0;
}

bool TmSerial::sensor_remin_isMoving()
{
    if( pr_pSerWork ){
        //qDebug()<<"Remin:"<<pr_pSerWork->m_bIsPersonMoving;
        return pr_pSerWork->m_bIsPersonMoving;
    }
    return false;
}

void TmSerial::motor_head_enable(bool bEnable)
{
    char buff[] = { 0x24, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA };
    if( bEnable ){
        buff[3] = 0x01;
        buff[4] = 0x01;
    }
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_head_zero()
{
    char buff[] = { 0x24, 0x5d, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0xAA };
    emit writeSerialData(QByteArray(buff, 9));
    buff[2] = 0x02;
    buff[4] = 0x00;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_head_LR(int nAngle, int nSpd)
{
    char buff[] = { 0x24, 0x5a, 0x02, nSpd, 0x00, 0x00, 0x00, 0x00, 0xAA };
    switch (nSpd) {
    case 1:
        buff[3] = 0x10;
        break;
    case 2:
        buff[3] = 0x15;
        break;
    case 3:
        buff[3] = 0x20;
        break;
    default:
        buff[3] = 0x15;
        break;
    }
    int nReal = _head_lr[1] + nAngle;
    if( nReal < _head_lr[0] ){
        nReal = _head_lr[0];
    }else if( nReal > _head_lr[2] ){
        nReal = _head_lr[2];
    }
    buff[4] = (uchar)nReal;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_head_UD(int nAngle, int nSpd)
{
    char buff[] = { 0x24, 0x5a, 0x01, nSpd, 0x00, 0x00, 0x00, 0x00, 0xAA };
    switch (nSpd) {
    case 1:
        buff[3] = 0x05;
        break;
    case 2:
        buff[3] = 0x08;
        break;
    case 3:
        buff[3] = 0x10;
        break;
    default:
        buff[3] = 0x08;
        break;
    }
    int nReal = _head_ud[1] - nAngle;
    if( nReal < _head_ud[0] ){
        nReal = _head_ud[0];
    }else if( nReal > _head_ud[2] ){
        nReal = _head_ud[2];
    }
    buff[4] = (uchar)nReal;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_hand_left_enable(bool bEnable)
{
    char buff[] = { 0x22, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA };
    if( bEnable ){
        buff[3] = 0x01;
        buff[4] = 0x01;
    }
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_hand_left_zero()
{
    char buff[] = { 0x22, 0x5d, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0xAA };
    emit writeSerialData(QByteArray(buff, 9));
    buff[2] = 0x02;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_hand_left_Expand(int nAngle, int nSpd)
{
    char buff[] = { 0x22, 0x5a, 0x02, nSpd, 0x00, 0x00, 0x00, 0x00, 0xAA };
    switch (nSpd) {
    case 1:
        buff[3] = 0x10;
        break;
    case 2:
        buff[3] = 0x20;
        break;
    case 3:
        buff[3] = 0x30;
        break;
    default:
        buff[3] = 0x20;
        break;
    }
    int nReal = _hand_left_e[1] - (nAngle - 90);
    if( nReal < _hand_left_e[0] ){
        nReal = _hand_left_e[0];
    }else if( nReal > _hand_left_e[2] ){
        nReal = _hand_left_e[2];
    }
    buff[4] = (uchar)nReal;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_hand_left_Swing(int nAngle, int nSpd)
{
    char buff[] = { 0x22, 0x5a, 0x01, nSpd, 0x00, 0x00, 0x00, 0x00, 0xAA };
    switch (nSpd) {
    case 1:
        buff[3] = 0x10;
        break;
    case 2:
        buff[3] = 0x20;
        break;
    case 3:
        buff[3] = 0x30;
        break;
    default:
        buff[3] = 0x20;
        break;
    }
    int nReal = _hand_left_s[1] + nAngle;
    if( nReal < _hand_left_s[0] ){
        nReal = _hand_left_s[0];
    }else if( nReal > _hand_left_s[2] ){
        nReal = _hand_left_s[2];
    }
    buff[4] = (uchar)nReal;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_hand_right_enable(bool bEnable)
{
    char buff[] = { 0x23, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA };
    if( bEnable ){
        buff[3] = 0x01;
        buff[4] = 0x01;
    }
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_hand_right_zero()
{
    char buff[] = { 0x23, 0x5d, 0x01, 0x05, 0x01, 0x00, 0x00, 0x00, 0xAA };
    emit writeSerialData(QByteArray(buff, 9));
    buff[2] = 0x02;
    buff[4] = 0x00;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_hand_right_Expand(int nAngle, int nSpd)
{
    char buff[] = { 0x23, 0x5a, 0x02, nSpd, 0x00, 0x00, 0x00, 0x00, 0xAA };
    switch (nSpd) {
    case 1:
        buff[3] = 0x10;
        break;
    case 2:
        buff[3] = 0x20;
        break;
    case 3:
        buff[3] = 0x30;
        break;
    default:
        buff[3] = 0x20;
        break;
    }
    int nReal = _hand_right_e[1] - (nAngle - 90);
    if( nReal < _hand_right_e[0] ){
        nReal = _hand_right_e[0];
    }else if( nReal > _hand_right_e[2] ){
        nReal = _hand_right_e[2];
    }
    buff[4] = (uchar)nReal;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_hand_right_Swing(int nAngle, int nSpd)
{
    char buff[] = { 0x23, 0x5a, 0x01, nSpd, 0x00, 0x00, 0x00, 0x00, 0xAA };
    switch (nSpd) {
    case 1:
        buff[3] = 0x10;
        break;
    case 2:
        buff[3] = 0x20;
        break;
    case 3:
        buff[3] = 0x30;
        break;
    default:
        buff[3] = 0x20;
        break;
    }
    int nReal = _hand_right_s[1] + nAngle;
    if( nReal < _hand_right_s[0] ){
        nReal = _hand_right_s[0];
    }else if( nReal > _hand_right_s[2] ){
        nReal = _hand_right_s[2];
    }
    buff[4] = (uchar)nReal;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_wheel_enable(bool bEnable)
{
     g_log("--------------motor_wheel_enable--------------------");
    char buff[] = { 0x22, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA };
    if( bEnable ){
        buff[3] = 0x01;
        buff[4] = 0x01;
    }
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_wheel_stop(bool bSlow)
{
    char buff[] = { 0x22, 0x55, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0xAA };
    if( bSlow ){
        buff[1] = 0x54;
    }
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_wheel_move(bool bUp, int nDistance, int nSpd)
{
//    g_log("--------------motor_wheel_move--------------------");
    char buff[] = { 0x22, 0x50, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xAA };
    if( bUp ){
        buff[3] = 0x00;
    }
    switch (nSpd) {
    case 1:
        nSpd = 0x21;
        break;
    case 2:
        nSpd = 0x42;
        break;
    case 3:
        nSpd = 0x50;
        break;
    default:
        nSpd = 0x21;
        break;
    }
    buff[4] = (uchar)nSpd;
    buff[5] = (uchar)((nDistance&0xff00)>>8);
    buff[6] = (uchar)(nDistance&0xff);
//    qDebug()<<buff;
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_wheel_rotate(bool bRight, int nAngle, int nSpd)
{
    char buff[] = { 0x22, 0x51, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xAA };
    if( bRight ){
        buff[3] = 0x00;
    }
    // --fix motor bug--
    nAngle *= 4;
    // ----
    switch (nSpd) {
    case 1:
        nSpd = 0x21;
        break;
    case 2:
        nSpd = 0x42;
        break;
    case 3:
        nSpd = 0x50;
        break;
    default:
        nSpd = 0x21;
        break;
    }
    buff[4] = (uchar)nSpd;
    buff[5] = (uchar)((nAngle&0xff00)>>8);
    buff[6] = (uchar)(nAngle&0xff);
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_wheel_diff_run(int nLeftForward, int nLeftSpeed, int nRightForward, int nRightSpeed)
{
    char buff[] = { 0x22, 0x53, 0x00, (uchar)nLeftForward, (uchar)nLeftSpeed,(uchar)nRightForward, (uchar)nRightSpeed, 0x00, 0xAA };
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::walk_cmd_wheel_code_reset()
{
    //当码盘进行清零重新基数时，码盘记录变量同时归零
    Robot_Left_WheelCode_Num = 0;
    Robot_Right_WheelCode_Num = 0;
    Compass_Yaw_Start_Angle = Compass_Yaw_Current_Angle = 1000;

    char buff[] = {0x22, 0x57, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA};
    emit writeSerialData(QByteArray(buff, 9));

}

void TmSerial::walk_query_wheel_code_info(bool bRight)
{
    char buff[] = {0x22, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA};
    if( bRight ){
        buff[2] = 0x01;
    }
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::motor_wheel_code_query_start()
{
    m_timerWheelCodeQuery->start(100);
}

void TmSerial::motor_wheel_code_query_stop()
{
    m_timerWheelCodeQuery->stop();
}

void TmSerial::motor_wheel_code_reset()
{
    walk_cmd_wheel_code_reset();
}


void TmSerial::led_face_set_up(int nFaceID)
{
    pr_nFaceID = nFaceID;
}


void TmSerial::led_face_set(int nIndex, int nTmMid, int nTmOver, int nTmFrame, int nNumRepeat)
{
    char buff[] = { 0x86, 0x01, (uchar)nIndex, (uchar)nTmMid, (uchar)nTmOver, (uchar)nTmFrame, (uchar)nNumRepeat, 0x00, 0xAA };
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::led_body_set(int nR, int nG, int nB, int nDelay)
{
    char buff[] = { 0x02, 0x01, 0x15, (uchar)nDelay, (uchar)nR, (uchar)nG, (uchar)nB, 0x00, 0xAA };
    switch (nDelay) {
    case 1:
        buff[3] = 0x30;
        break;
    case 2:
        buff[3] = 0x20;
        break;
    case 3:
        buff[3] = 0x10;
        break;
    default:
        buff[3] = 0x00;
        break;
    }
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::led_head_set(int nR, int nG, int nB, int nDelay)
{
    char buff[] = { 0x02, 0x01, 0x15, (uchar)nDelay, (uchar)nR, (uchar)nG, (uchar)nB, 0x00, 0xAA };

    //emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::led_touch_set(int nMode, int nColor, int nFreq)
{

}

void TmSerial::led_touch_get()
{

}

bool TmSerial::json_load_by_name(QString strName)
{
    pr_enginJson.stop();
    pr_nScriptCount = 0;
    pr_cmdJsonMap.clear();
    QMap<QString, QJsonArray>::const_iterator arryObj = pr_jsonMap.find(strName);
    if( arryObj == pr_jsonMap.end() ){
        return false;
    }
    QJsonArray allArray = arryObj.value();
    int nIndex = g_nGetRandom(0, allArray.size()-1);
    QJsonObject objOne = allArray.at(nIndex).toObject();
//    qDebug()<<"Json Val:"<<strName<<nIndex<<allArray.size();
//    qDebug()<<"Json Action:"<<objOne.value("name").toString()<<objOne.value("description").toString();
    QJsonArray bArry = objOne.value("data").toArray();
    for(int i=0; i<bArry.size();i++){
        QJsonObject obj = bArry.at(i).toObject();
        int nTime = obj.value("time").toInt(-1);
        if( nTime != -1 ) {
            pr_cmdJsonMap.insert(nTime, obj);
        }
    }
    // run engine
    if( pr_cmdJsonMap.size() > 0 ){
//        pr_nScriptCount = 0;
//        pr_enginJson.start(100);
        emit sig_json_engine(true);
        return true;
    }
    return false;
}

bool TmSerial::json_load_isRunning()
{
    return pr_enginJson.isActive();
}

void TmSerial::json_load_stop()
{
//    pr_enginJson.stop();
//    pr_cmdJsonMap.clear();
//    pr_nScriptCount = 0;
    emit sig_json_engine(false);
}

void TmSerial::behavior_auto_action(bool bStart)
{
    emit sig_say_auto_control(bStart);
}

void TmSerial::slot_engineJson_timeout()
{
    if( pr_cmdJsonMap.empty() ){
        pr_enginJson.stop();
        pr_nScriptCount = 0;
        qDebug()<<"Json Frame stopped!";
        return;
    }
    if( pr_nScriptCount == 0 ){
        qDebug()<<"Json Frame start!";
    }
    // run
    QJsonObject obj = pr_cmdJsonMap.take(pr_nScriptCount);
    /*{
        "time":10,
        "face":[1,0,0,0,0],
        "led_head":[255,255,255,0],
        "led_body":[255,255,255,0],
        "head_lr":[40,-1],
        "head_up":[10,-1],
        "hand_left_expand":[60,-1],
        "hand_left_swing":[-20,-1],
        "hand_right_expand":[10,-1],
        "hand_right_swing":[60,-1]
    },*/
    if( !obj.empty() ){
        // 解析每一个的参数
        QJsonArray objParam = obj.value("face").toArray();
        if( !objParam.empty() ){
            //led_face_set(objParam.at(0).toInt(),objParam.at(1).toInt(),objParam.at(2).toInt(),objParam.at(3).toInt(),objParam.at(4).toInt());
            led_face_set_by_id(objParam.at(0).toInt());
        }
        objParam = obj.value("led_head").toArray();
        if( !objParam.empty() ){
            led_head_set(objParam.at(0).toInt(),objParam.at(1).toInt(),objParam.at(2).toInt(),objParam.at(3).toInt());
        }
        objParam = obj.value("led_body").toArray();
        if( !objParam.empty() ){
            led_body_set(objParam.at(0).toInt(),objParam.at(1).toInt(),objParam.at(2).toInt(),objParam.at(3).toInt());
        }
        objParam = obj.value("head_lr").toArray();
        if( !objParam.empty() ){
            motor_head_LR(objParam.at(0).toInt(),objParam.at(1).toInt());
        }
        objParam = obj.value("head_ud").toArray();
        if( !objParam.empty() ){
            motor_head_UD(objParam.at(0).toInt(),objParam.at(1).toInt());
        }
        objParam = obj.value("hand_left_expand").toArray();
        if( !objParam.empty() ){
            motor_hand_left_Expand(objParam.at(0).toInt(),objParam.at(1).toInt());
        }
        objParam = obj.value("hand_left_swing").toArray();
        if( !objParam.empty() ){
            motor_hand_left_Swing(objParam.at(0).toInt(),objParam.at(1).toInt());
        }
        objParam = obj.value("hand_right_expand").toArray();
        if( !objParam.empty() ){
            motor_hand_right_Expand(objParam.at(0).toInt(),objParam.at(1).toInt());
        }
        objParam = obj.value("hand_right_swing").toArray();
        if( !objParam.empty() ){
            motor_hand_right_Swing(objParam.at(0).toInt(),objParam.at(1).toInt());
        }
    }
    pr_nScriptCount += 1;
    //qDebug()<<"Json Frame:"<<pr_nScriptCount;
}

void TmSerial::slot_sensor_chaosheng()
{
    char buff[] = { 0x02, 0x01, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA };
    emit writeSerialData(QByteArray(buff, 9));
}

void TmSerial::slot_json_engine(bool bStart)
{
    qDebug()<<"Json engine thread"<<QThread::currentThreadId();
    if( bStart ){
        pr_nScriptCount = 0;
        pr_enginJson.start(100);
    }else{
        qDebug()<<"json engine timmer stopped!";
        pr_enginJson.stop();
        pr_cmdJsonMap.clear();
        pr_nScriptCount = 0;
    }
}

void TmSerial::slot_say_auto_action()
{
    if( g_pTmMedia->isPlaying() ){
        if( !g_pRobot->json_load_isRunning() ){
            g_pRobot->json_load_by_name("auto_action");
        }
    }else{
        emit sig_say_auto_control(false);
    }
}

void TmSerial::slot_say_control(bool bStart)
{
    if( bStart ){
        pr_tmSayAutoAction.start(800);
        g_pRobot->json_load_by_name("auto_action");
    }else{
        pr_tmSayAutoAction.stop();
        g_pRobot->json_load_stop();
        // reset
        motor_hand_left_Expand(0,2);
        motor_hand_left_Swing(0,2);
        motor_hand_right_Expand(0,2);
        motor_hand_right_Swing(0,2);
        motor_head_LR(0,2);
        motor_head_UD(0,2);
        led_body_set(0,0,1,0);
        led_face_set(26,10,10,10,0);
    }
}


void TmSerial::slot_wheelcode_query_info()
{
    //同时进行左右位移查询
    walk_query_wheel_code_info(true);
    walk_query_wheel_code_info(false);
}
void TmSerial::slot_wheelcode_feedback_info(QByteArray bArry)
{
    int t_value =(int)( (((uchar)bArry[4])<<16)|(((uchar)bArry[5])<<8) | (uchar)bArry[6]);

    S_DOUBLE_VALUE_COUNT = S_DOUBLE_VALUE_COUNT + 1;

    if((uchar)bArry[3] > 0){
        if(t_value >0){
            Robot_Right_WheelCode_Num =0xffffff - t_value ;
        }
    }else{
         Robot_Left_WheelCode_Num = t_value;
    }

    if((S_DOUBLE_VALUE_COUNT % 2) == 0){
        emit sig_tmserial_cruise_info(Robot_Left_WheelCode_Num,Robot_Right_WheelCode_Num,
                                      Compass_Current_Roll,Compass_Current_Pitch,Compass_Current_Yaw);


        S_DOUBLE_VALUE_COUNT = 0;
    }





    //    g_log("bsf --------------------------------------slot_wheelcode_feedback_info");
    //    QString strOut;


    //    "01 22 59 01 FF 81 8C 89 AA"
    //    strOut.sprintf("%02X %02X %02X %02X %02X %02X %02X %02X %02X",
    //                   (uchar)bArry[0],(uchar)bArry[1],(uchar)bArry[2],(uchar)bArry[3],(uchar)bArry[4],(uchar)bArry[5],(uchar)bArry[6],(uchar)bArry[7],(uchar)bArry[8]);
    //    qDebug()<<strOut;


//    qDebug("%02X",( (((uchar)bArry[4])<<16)|(((uchar)bArry[5])<<8) | (uchar)bArry[6]));

//    qDebug("%d",fRoll);

//    int num = 0;
//    char str[]={bArry[4], bArry[5] ,bArry[6]}; //13D620 ->1300000
//    for( int i = 0; i < sizeof( str ); i++ )
//    {
//    num *= 16*16;
//    num += (unsigned char)str[i];
//    }

//    qDebug("%d",num);



//


//    bool isModify = false;
//    if(m_wheelStatus == TmSerial::Moving && isModify){

//        double t_angle = Compass_Yaw_Current_Angle - Compass_Yaw_Start_Angle;

//        if(t_angle > 1){
//            g_log("进行移动修正。");
//            qDebug() << "1-------------启动角度为"<< Compass_Yaw_Start_Angle;
//            qDebug() << "2-------------当前角度为"<< Compass_Yaw_Current_Angle;
//            qDebug() << "3-------------偏移角度为" << t_angle;
//            this->motor_wheel_rotate(true,qAbs(t_angle),1);
//            Compass_Yaw_Start_Angle = Compass_Yaw_Current_Angle;
//        }
//        if(t_angle < -1){

//            g_log("进行移动修正。");
//            qDebug() << "1-------------启动角度为"<< Compass_Yaw_Start_Angle;
//            qDebug() << "2-------------当前角度为"<< Compass_Yaw_Current_Angle;
//            qDebug() << "3-------------偏移角度为" << t_angle;
//            this->motor_wheel_rotate(false,qAbs(t_angle),1);
//            Compass_Yaw_Start_Angle = Compass_Yaw_Current_Angle;
//        }


//    }
//    if(m_wheelStatus == TmSerial::Rotating && isModify){
//        g_log("进行旋转修正。");

//        double t_angle = Compass_Yaw_Current_Angle - (Compass_Yaw_Start_Angle + m_inputMoveAngle);
//        if(t_angle > 1){
//            this->motor_wheel_rotate(true,qAbs(t_angle),1);
//        }

//        if(t_angle < -1){
//            this->motor_wheel_rotate(false,qAbs(t_angle),1);
//        }


//    }





}

void TmSerial::slot_wheel_feedback_stop(QByteArray bArry)
{
    //    "01 22 56 01 FF 81 8C 89 AA"
    QString strOut;
    strOut.sprintf("%02X %02X %02X %02X %02X %02X %02X %02X %02X",
                   (uchar)bArry[0],(uchar)bArry[1],(uchar)bArry[2],(uchar)bArry[3],(uchar)bArry[4],(uchar)bArry[5],(uchar)bArry[6],(uchar)bArry[7],(uchar)bArry[8]);
    qDebug()<<strOut;

    emit sig_tmserial_wheel_stop_normal_status(true);

}


void TmSerial::slot_compass_feedback_info(double fYoll, double fPitch, double fYaw)
{
    //记录变化了得角度
    Compass_Current_Roll = fYoll;
    Compass_Current_Pitch = fPitch;
    Compass_Current_Yaw = fYaw;
    emit sig_tmserial_compass_info(fYoll,fPitch,fYaw);
}


void TmSerial::led_face_set_by_name(QString strName)
{
    int nIndex = 0;
    if( "zhayan" == strName ){
        nIndex = 0;
    }else if( "wunai" == strName ){
        nIndex = 1;
    }else if( "henwunai" == strName ){
        nIndex = 2;
    }else if( "wushi" == strName ){
        nIndex = 3;
    }else if( "baoliu" == strName ){
        nIndex = 4;
    }else if( "jingqi" == strName ){
        nIndex = 5;
    }else if( "xin" == strName ){
        nIndex = 6;
    }else if( "weixiao" == strName ){
        nIndex = 7;
    }else if( "shangxin" == strName ){
        nIndex = 8;
    }else if( "shengqi" == strName ){
        nIndex = 9;
    }else if( "kunle" == strName ){
        nIndex = 10;
    }else if( "duiyan" == strName ){
        nIndex = 16;
    }else if( "fashe" == strName ){
        nIndex = 17;
    }else if( "faguang" == strName ){
        nIndex = 18;
    }else if( "fangkong" == strName ){
        nIndex = 19;
    }else if( "fennu" == strName ){
        nIndex = 20;
    }else if( "gantan" == strName ){
        nIndex = 21;
    }else if( "haixiu" == strName ){
        nIndex = 22;
    }else if( "huanxing" == strName ){
        nIndex = 23;
    }else if( "jujiao" == strName ){
        nIndex = 24;
    }else if( "jujiao2" == strName ){
        nIndex = 25;
    }else if( "kaixin" == strName ){
        nIndex = 26;
    }else if( "shuidi" == strName ){
        nIndex = 27;
    }else if( "sikao" == strName ){
        nIndex = 28;
    }else if( "xihuan" == strName ){
        nIndex = 29;
    }else if( "xintiao" == strName ){
        nIndex = 30;
    }else if( "xuanyun" == strName ){
        nIndex = 31;
    }else if( "zhayan2" == strName ){
        nIndex = 32;
    }else if( "zhixianzhayan" == strName ){
        nIndex = 33;
    }else if( "zhuanyanqiu" == strName ){
        nIndex = 34;
    }else if( "xielunyan" == strName ){
        nIndex = 35;
    }else if( "chijing" == strName ){
        nIndex = 36;
    }else if( "chuchudongrenshanshuo" == strName ){
        nIndex = 37;
    }else if( "gaoxingshanshuo" == strName ){
        nIndex = 38;
    }else if( "guilainshanshuo" == strName ){
        nIndex = 39;
    }else if( "guilianfangdian" == strName ){
        nIndex = 40;
    }else if( "haixiu1shanshuo" == strName ){
        nIndex = 41;
    }else if( "haixiu2shanshuo" == strName ){
        nIndex = 42;
    }else if( "huaixiao" == strName ){
        nIndex = 43;
    }else if( "keaishanshuo" == strName ){
        nIndex = 44;
    }else if( "qidaishanshuo" == strName ){
        nIndex = 45;
    }else if( "renzhenshanshuo" == strName ){
        nIndex = 46;
    }else if( "sikaoshanshuo" == strName ){
        nIndex = 47;
    }else if( "xihuanni" == strName ){
        nIndex = 48;
    }else if( "xiashanshuo" == strName ){
        nIndex = 49;
    }else if( "faguangshanshuo" == strName ){
        nIndex = 50;
    }else if( "zhayanjing" == strName ){
        nIndex = 100;
    }else{
        nIndex = -1;
    }
    led_face_set_by_id(nIndex);
}

void TmSerial::led_face_set_by_id(int nIndex)
{
    char buff[] = { 0x86, 0x01, (uchar)nIndex, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA };
    switch (nIndex) {
    case 0:
        {
            buff[3] = 0x02;
            buff[4] = 0x01;
            buff[5] = 0x02;
        }
        break;
    case 1:
        {
            buff[3] = 0x03;
            buff[4] = 0x03;
            buff[5] = 0x08;
        }
        break;
    case 2:
        {
            buff[3] = 0x04;
            buff[4] = 0x04;
            buff[5] = 0x08;
        }
        break;
    case 3:
        {
            buff[3] = 0x0b;
            buff[4] = 0x06;
            buff[5] = 0x04;
        }
        break;
    case 4:
        {
            buff[3] = 0x04;
            buff[4] = 0x06;
            buff[5] = 0x04;
        }
        break;
    case 5:
        {
            buff[3] = 0x04;
            buff[4] = 0x01;
            buff[5] = 0x02;
        }
        break;
    case 6:
        {
            buff[3] = 0x08;
            buff[4] = 0x02;
            buff[5] = 0x04;
        }
        break;
    case 7:
        {
            buff[3] = 0x02;
            buff[4] = 0x02;
            buff[5] = 0x02;
        }
        break;
    case 8:
        {
            buff[3] = 0x10;
            buff[4] = 0x04;
            buff[5] = 0x04;
        }
        break;
    case 9:
        {
            buff[3] = 0x0a;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 10:
        {
            buff[3] = 0x10;
            buff[4] = 0x0a;
            buff[5] = 0x04;
        }
        break;
    case 16:
        {
            buff[3] = 0x0a;
            buff[4] = 0x0a;
            buff[5] = 0x02;
        }
        break;
    case 17:
        {
            buff[3] = 0x0a;
            buff[4] = 0x03;
            buff[5] = 0x01;
        }
        break;
    case 18:
        {
            buff[3] = 0x08;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 19:
        {
            buff[3] = 0x0a;
            buff[4] = 0x06;
            buff[5] = 0x02;
        }
        break;
    case 20:
        {
            buff[3] = 0x0a;
            buff[4] = 0x08;
            buff[5] = 0x06;
        }
        break;
    case 21:
        {
            buff[3] = 0x04;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 22:
        {
            buff[3] = 0x10;
            buff[4] = 0x08;
            buff[5] = 0x02;
        }
        break;
    case 23:
        {
            buff[3] = 0x08;
            buff[4] = 0x08;
            buff[5] = 0x02;
        }
        break;
    case 24:
        {
            buff[3] = 0x10;
            buff[4] = 0x06;
            buff[5] = 0x02;
        }
        break;
    case 25:
        {
            buff[3] = 0x10;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 26:
        {
            buff[3] = 0x08;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 27:
        {
            buff[3] = 0x06;
            buff[4] = 0x06;
            buff[5] = 0x01;
        }
        break;
    case 28:
        {
            buff[3] = 0x16;
            buff[4] = 0x0a;
            buff[5] = 0x20;
        }
        break;
    case 29:
        {
            buff[3] = 0x10;
            buff[4] = 0x0a;
            buff[5] = 0x20;
        }
        break;
    case 30:
        {
            buff[3] = 0x08;
            buff[4] = 0x04;
            buff[5] = 0x04;
        }
        break;
    case 31:
        {
            buff[3] = 0x06;
            buff[4] = 0x06;
            buff[5] = 0x04;
        }
        break;
    case 32:
        {
            buff[3] = 0x06;
            buff[4] = 0x06;
            buff[5] = 0x02;
        }
        break;
    case 33:
        {
            buff[3] = 0x08;
            buff[4] = 0x08;
            buff[5] = 0x02;
        }
        break;
    case 34:
        {
            buff[3] = 0x04;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 35:
        {
            buff[3] = 0x08;
            buff[4] = 0x08;
            buff[5] = 0x04;
        }
        break;
    case 36:
        {
            buff[3] = 0x04;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 37:
        {
            buff[3] = 0x08;
            buff[4] = 0x08;
            buff[5] = 0x04;
        }
        break;
    case 38:
        {
            buff[3] = 0x04;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 39:
        {
            buff[3] = 0x08;
            buff[4] = 0x08;
            buff[5] = 0x02;
        }
        break;
    case 40:
        {
            buff[3] = 0x08;
            buff[4] = 0x08;
            buff[5] = 0x02;
        }
        break;
    case 41:
        {
            buff[3] = 0x06;
            buff[4] = 0x06;
            buff[5] = 0x04;
        }
        break;
    case 42:
        {
            buff[3] = 0x10;
            buff[4] = 0x06;
            buff[5] = 0x04;
        }
        break;
    case 43:
        {
            buff[3] = 0x06;
            buff[4] = 0x06;
            buff[5] = 0x04;
        }
        break;
    case 44:
        {
            buff[3] = 0x08;
            buff[4] = 0x08;
            buff[5] = 0x02;
        }
        break;
    case 45:
        {
            buff[3] = 0x10;
            buff[4] = 0x08;
            buff[5] = 0x08;
        }
        break;
    case 46:
        {
            buff[3] = 0x10;
            buff[4] = 0x08;
            buff[5] = 0x10;
        }
        break;
    case 47:
        {
            buff[3] = 0x10;
            buff[4] = 0x0a;
            buff[5] = 0x10;
        }
        break;
    case 48:
        {
            buff[3] = 0x08;
            buff[4] = 0x10;
            buff[5] = 0x08;
        }
        break;
    case 49:
        {
            buff[3] = 0x08;
            buff[4] = 0x06;
            buff[5] = 0x02;
        }
        break;
    case 50:
        {
            buff[3] = 0x04;
            buff[4] = 0x04;
            buff[5] = 0x02;
        }
        break;
    case 100:
        {
            buff[3] = 0x06;
            buff[4] = 0x06;
            buff[5] = 0x02;
        }
        break;
    default:
        {
            if( nIndex < 0 ){
                buff[1] = 0x02; // 静态表情
                buff[2] = 0x00;
            }else{
                buff[2] = 0x64; // 100
                buff[3] = 0x06;
                buff[4] = 0x06;
                buff[5] = 0x02;
            }
        }
        break;
    }
    emit writeSerialData(QByteArray(buff, 9));
}




















