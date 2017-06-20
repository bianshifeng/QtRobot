#ifndef TMSERIAL_H
#define TMSERIAL_H
/*
 * Work with TmSerialWork, used to control serialport by send signal.
 * It also used to process sample logic and control basic actions.
 * Run in the main thread.
 */
#include <QObject>
#include <tmserialwork.h>
#include <QTimer>
#include "tmnaviangle.h"

class TmSerial : public QObject
{
    Q_OBJECT
public:

    enum WheelStatus{Moving,Rotating,Stop};

    explicit TmSerial(QObject *parent = 0);

    bool initConfig();


    bool initMainSerial();
    bool initCompassSerial();
    bool initEnginJson();



    void start_sensor_chaosheng(bool bOpen);
    void exit();

    // data
    int sensor_shengbo_distance(int nId);
    int sensor_hongwai_distance(int nId);
    bool sensor_remin_isMoving();

    // head control
    void motor_head_enable(bool bEnable);
    void motor_head_zero();
    void motor_head_LR(int nAngle, int nSpd);
    void motor_head_UD(int nAngle, int nSpd);
    // hand left control
    void motor_hand_left_enable(bool bEnable);
    void motor_hand_left_zero();
    void motor_hand_left_Expand(int nAngle, int nSpd);
    void motor_hand_left_Swing(int nAngle, int nSpd);
    // hand right control
    void motor_hand_right_enable(bool bEnable);
    void motor_hand_right_zero();
    void motor_hand_right_Expand(int nAngle, int nSpd);
    void motor_hand_right_Swing(int nAngle, int nSpd);



    //移动模块的通讯端口
    // wheel control
    void motor_wheel_enable(bool bEnable);
    void motor_wheel_stop(bool bSlow);
    void motor_wheel_move(bool bUp, int nDistance, int nSpd);
    void motor_wheel_rotate(bool bRight, int nAngle, int nSpd);
    void motor_wheel_diff_run(int nLeftForward, int nLeftSpeed,int nRightForward, int nRightSpeed); //差速行走

    //移动回馈，码盘数据查询
    //左右非常不好，应用数值enums
    void walk_cmd_wheel_code_reset();
    void walk_query_wheel_code_info(bool bRight);
    void motor_wheel_code_query_start();
    void motor_wheel_code_query_stop();
    void motor_wheel_code_reset();



    // led control
    void led_face_set_by_name(QString strName);
    void led_face_set_by_id(int nIndex);
    void led_face_set_up(int nFaceID);
    void led_face_set(int nIndex, int nTmMid, int nTmOver, int nTmFrame, int nNumRepeat);
    void led_body_set(int nR, int nG, int nB, int nDelay);
    void led_head_set(int nR, int nG, int nB, int nDelay);
    // nMode: 1 闪烁模式， 2 呼吸灯模式
    // nColor: 0x00红色  0x01绿色  0x02蓝色  0x03紫色  0x04青色  0x05黄色  0x06白色
    // nFreq: 0x00全彩灯不闪烁，长亮。0x01 0x02 0x03全彩灯闪烁频率一次增大。
    void led_touch_set(int nMode, int nColor, int nFreq);
    // 主动查询一次
    void led_touch_get();

    // script json
    bool json_load_by_name(QString strName);
    bool json_load_isRunning();
    void json_load_stop();

    // behavior
    void behavior_auto_action(bool bStart);
private:
    int _head_ud[3],_head_lr[3],_hand_left_e[3],_hand_left_s[3],_hand_right_e[3],_hand_right_s[3]; // 电机的限制角度
    TmSerialWork* pr_pSerWork;
    TMNaviAngle* m_ptrNaviCompass;
    QTimer* m_timerWheelCodeQuery;

    int pr_nFaceID;
    // script json
    QTimer pr_enginJson;
    QTimer pr_tmSayAutoAction; // do action when saying
    QMap<QString, QJsonArray> pr_jsonMap;
    int pr_nScriptCount;
    QMap<int, QJsonObject> pr_cmdJsonMap;
    QTimer pr_sensor_chaosheng;

signals:
    void initSerial(QString strCom);
    void writeSerialData(QByteArray bArry);
    void sig_json_engine(bool bStart);
    void sig_say_auto_control(bool bStart);
    void sig_serial_data(QByteArray btyeArrayData);
    void sig_read_wheel_code_info(int,int,int);
    void sig_tmserial_compass_info(double, double, double);
    void sig_tmserial_wheel_stop_normal_status(bool);


    void sig_tmserial_cruise_info(int leftWheelCode,int rightWheelCode, double roll,double pitch,double yaw);

private slots:
    void slot_engineJson_timeout();
    void slot_sensor_chaosheng();
    void slot_json_engine(bool bStart);
    void slot_say_auto_action();
    void slot_say_control(bool bStart);





    void slot_wheelcode_feedback_info(QByteArray btyeArrayData);
    void slot_wheel_feedback_stop(QByteArray byteArrayData);
    void slot_wheelcode_query_info();
    void slot_compass_feedback_info(double,double,double);

};

#endif // TMSERIAL_H
