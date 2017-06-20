#ifndef TMROBOTINTERFACE_H
#define TMROBOTINTERFACE_H

#include <QObject>
#include <QTimer>

// some interface for windows system
//#ifdef Q_OS_WIN
//#include <windows.h>
//#endif

class TmRobotInterface : public QObject
{
    Q_OBJECT
public:
    explicit TmRobotInterface(QObject *parent = 0);
    void initConfig();

signals:
    void sig_ui_jump_name(bool bOk);
    void sig_ui_video_show(QString strFilePath);
    void sig_ui_background_set(QString strModeName, bool bShow, QString strFileName);

public slots: // mothod
    void log(QString strOut); // 输出日志
    void msleep(int nMinSec); // 强制休眠
    int nrandom(int nMin, int nMax); // 取区间 [nMin, nMax] 之间的随机数

    // sensor status
    int sensor_camera_persons(); // 获取视频中人脸的数目
    int sensor_shengbo_distance(int nID); // 获取超声波的距离
	int sensor_hongwai_distance(int nID); // 获取红外测距的距离
    bool sensor_remin_isMoving(); // 检测热敏传感器是否能检测到人

    // special functions
    void behavior_lookat_open(bool bIsOpen); // 盯着看
    void behavior_auto_action(bool bStart);  // 自动配动作和表情
    void behavior_auto_talk_open(bool bStart); // 自由对话功能
    void behavior_auto_talk_pause(bool bPause); // 自由对话功能

    // special
    //void behavior_cmd_talk_stop();

    // tool
    void tool_ask_answer(QString strAsk); // 通过网络库，返回给脚本一个答案，jsEvent("tuling", "xxxx");
    QList<int> tool_kinect_get_data(); // 获取Kinect的肢体数据
    void tool_kinect_start(); // 开启Kinect骨骼跟踪和语音方向判断功能
    bool tool_kinect_is_opened(); // 判断Kinect是否开启正常
    void tool_kinect_stop(); // 停止Kinect所有功能

    // head control
    void motor_head_enable(bool bEnable); // 头部电机使能
    void motor_head_zero();  // 头部电机初始化
    void motor_head_LR(int nAngle, int nSpd); // 头部Left/Right控制,nAngle[-60,60],nSpd[1-3]
    void motor_head_UD(int nAngle, int nSpd); // 头部Up/Down控制,nAngle[-10,10],nSpd[1-3]
    // hand left control
    void motor_hand_left_enable(bool bEnable); // 左手电机使能
    void motor_hand_left_zero(); // 左手电机归零
    void motor_hand_left_Expand(int nAngle, int nSpd); // 左手Expand控制,nAngle[0,180],nSpd[1-3]
    void motor_hand_left_Swing(int nAngle, int nSpd); // 左手Swing控制,nAngle[-20,180],nSpd[1-3]
    // hand right control
    void motor_hand_right_enable(bool bEnable); // 右手使能
    void motor_hand_right_zero(); // 右手电机初始化
    void motor_hand_right_Expand(int nAngle, int nSpd); // 右手Expand控制,nAngle[0,180],nSpd[1-3]
    void motor_hand_right_Swing(int nAngle, int nSpd); // 右手Swing控制,nAngle[-20,180],nSpd[1-3]
    // wheel control
    void motor_wheel_enable(bool bEnable); // 轮子的使能
    void motor_wheel_stop(bool bSlow); // 轮子停止，bShow -> true 缓停。false 急停
    void motor_wheel_move(bool bUp, int nDistance, int nSpd); // 移动固定的距离，单位mm, spd=[1-3]
    void motor_wheel_rotate(bool bRight, int nAngle, int nSpd); // 旋转固定的角度，单位度，spd=[1-3]
    void motor_wheel_diff_run(bool bForward, int nLeftSpeed, int nRightSpeed); // 差速行走，不了解的情况下不要使用。

    // led control
    void led_face_set_up(int nFaceID); // 设置当前设置的面部,0 前面，1后面，其他双面
    void led_face_set_by_name(QString strName); // 通过名字设置表情
    void led_face_set_by_id(int nIndex); // 通过FaceID设置表情
    void led_face_set(int nIndex, int nTmMid, int nTmOver, int nTmFrame, int nNumRepeat); // 通过参数设置表情
    void led_body_set(int nR, int nG, int nB, int nDelay); // 身体LED设置
    void led_head_set(int nR, int nG, int nB, int nDelay); // 头部LED设置

    // nMode: 1 闪烁模式， 2 呼吸灯模式
    // nColor: 0x00红色  0x01绿色  0x02蓝色  0x03紫色  0x04青色  0x05黄色  0x06白色
    // nFreq: 0x00全彩灯不闪烁，长亮。0x01 0x02 0x03全彩灯闪烁频率一次增大。
    void led_touch_set(int nMode, int nColor, int nFreq);
    // 主动查询一次
    void led_touch_get(); // 主动查询LED

    // script json
    bool json_load_by_name(QString strName); // 通过名称加载 base/stand_actions.json中的固定动作
    bool json_load_isRunning(); // 判断固定动作是否运行
    void json_load_stop(); // 强制结束固定动作

    // js module
    bool js_sub_load_by_name(QString strName); // 在Sub Js引擎中运行base/xxx.js脚本.
    bool js_sub_load_content(QString strJsContent); // 在Sub Js引擎中运行strJsContent中的内容
    bool js_sub_isRunning(); // 判断Sub js脚本是否在运行
    void js_sub_stop(); // 强制结束sub js脚本

    bool js_main_load_by_name(QString strName); // 在主js引擎中运行usr/scripts/xxx.js脚本.
    bool js_main_isRunning(); // 判断js引擎是否在运行状态
    void js_main_stop(); // 强制停止JS脚本

    // music and tts
    bool audio_play_tts(QString strTxt); // 以默认参数播放TTS语音
    bool audio_play_tts_params(QString strTxt, QString strSpeaker, int nSpeed, int nPitch, int nSpecial); // 播放自定义参数TTS语音
    void audio_play_system(QString strName); // 播放data/audio/xxx.wav文件
    bool audio_play_isPlaying(); // 判断音频是否在播放状态
    void audio_stop(); // 强制停止音频
    void audio_play_system_bg(QString strName); // 在后台播放某个音频。data/audio/xxx.wav
    bool audio_play_isPlaying_bg(); // 判断后台音频是否在播放状态
    void audio_stop_bg(); // 强制停止播放

    // ui
    void ui_background_set(QString strModeName, bool bShow, QString strFileName); // 设置背景图片(modeset/main true xxx.png)
    void ui_video_show(QString strFilePath); // 播放某个固定的视频，播放结束后自动隐藏。路径为相对或者绝对路径。
    bool ui_video_isPlaying(); // 判断视频是否在播放状态
    void ui_video_stopped(); // 强制停止视频
    void ui_pics_btn_show(QString strModeName, bool bShow); // 控制是否显示图片展示按钮
    void ui_pics_btn_name(QString strModeName, QString strName); // 控制图片展示按钮显示的文本
    void ui_pics_show(QString strFilePath); // 弹出图片展示界面,内容为路径文件夹下的图片。路径为相对或者绝对路径。
    void ui_pics_hide(); // 隐藏图片展示界面

    void ui_jump_name(QString strName); // 跳转界面逻辑，同时会触发对应界面的脚本

    // config
    QString config_file_get(QString fileName, QString strRootKey); // 读取配置文件。路径为相对或者绝对路径。
    void config_file_set(QString fileName, QString strRootKey, QString val); // 写入配置文件。路径为相对或者绝对路径。

    // system control
    void system_set_volume(int nVolume); // 设置音频播放音量大小。不要随意使用

    // plugin
    void exscreen_pic_dir(QString strDirName); // 设置扩展屏幕的轮询图片路径。路径相对或者绝对。
    void exscreen_pic_file(QString strFileFullName); // 设置扩展屏展示的图片。路径相对或者绝对。
    void exscreen_video_dir(QString strDirName); // 设置扩展屏轮播的视频路径。路径相对或者绝对。
    void exscreen_video_file(QString strFileFullName); // 设置扩展屏播放的视频。路径相对或者绝对。

private slots:
    void timeout_lookat();
private:
    QTimer pr_tmLookAt;
};

#endif // TMROBOTINTERFACE_H
