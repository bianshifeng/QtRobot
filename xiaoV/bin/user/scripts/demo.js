/* -------------jsState模块---------------------------
 * jsState.nFrameID : int, 当前桢的序号
 * jsState.bStopped : bool, 是否结束
 * jsState.nFrameMSleep : int, 每一帧的停顿时间
 * jsState.bIsModule : bool, 判断自身是否当作模块运行
 * --------------Robot模块-----------------------
 * void log(QString strOut); // 输出日志
 * void msleep(int nMinSec); // 强制休眠
 * int nrandom(int nMin, int nMax); // 取区间 [nMin, nMax] 之间的随机数

 * // sensor status
 * int sensor_camera_persons(); // 获取视频中人脸的数目
 * int sensor_shengbo_distance(int nID); // 获取超声波的距离
 * bool sensor_hongwai_distance(); // 检测热敏传感器是否能检测到人

 * // special functions
 * void behavior_lookat_open(bool bIsOpen); // 盯着看
 * void behavior_auto_action(bool bStart);  // 自动配动作和表情
 * void behavior_auto_talk_open(bool bStart); // 自由对话功能
 * void behavior_auto_talk_pause(bool bPause); // 自由对话功能

 * // special
 * double navi_get_angle(); // 获取角度传感器角度
 * QList<double> navi_get_pos(); // 获取坐标位置（x,y）,单位mm
 * void navi_to_id(int nID); // 导航到某个定位点
 * bool navi_is_moving();  // 检测导航是否在进行中
 * void navi_set_stop(); // 强制停止导航

 * // tool
 * void tool_ask_answer(QString strAsk); // 通过网络库，返回给脚本一个答案，jsEvent("tuling", "xxxx");
 * QList<int> tool_kinect_get_data(); // 获取Kinect的肢体数据
 * void tool_kinect_start(); // 开启Kinect骨骼跟踪和语音方向判断功能
 * bool tool_kinect_is_opened(); // 判断Kinect是否开启正常
 * void tool_kinect_stop(); // 停止Kinect所有功能

 * // wheel control
 * void motor_wheel_enable(bool bEnable); // 轮子的使能
 * void motor_wheel_stop(bool bSlow); // 轮子停止，bShow -> true 缓停。false 急停
 * void motor_wheel_move(bool bUp, int nDistance, int nSpd); // 移动固定的距离，单位mm, spd=[1-3]
 * void motor_wheel_rotate(bool bRight, int nAngle, int nSpd); // 旋转固定的角度，单位度，spd=[1-3]
 * void motor_wheel_diff_run(bool bForward, int nLeftSpeed, int nRightSpeed); // 差速行走，不了解的情况下不要使用。

 * // led control
 * void led_face_set_up(int nFaceID); // 设置当前设置的面部,0 前面，1后面，其他双面
 * void led_face_set_by_name(QString strName); // 通过名字设置表情
 * void led_face_set_by_id(int nIndex); // 通过FaceID设置表情
 * void led_face_set(int nIndex, int nTmMid, int nTmOver, int nTmFrame, int nNumRepeat); // 通过参数设置表情
 * void led_body_set(int nR, int nG, int nB, int nDelay); // 身体LED设置
 * void led_head_set(int nR, int nG, int nB, int nDelay); // 头部LED设置

 * // nMode: 1 闪烁模式， 2 呼吸灯模式
 * // nColor: 0x00红色  0x01绿色  0x02蓝色  0x03紫色  0x04青色  0x05黄色  0x06白色
 * // nFreq: 0x00全彩灯不闪烁，长亮。0x01 0x02 0x03全彩灯闪烁频率一次增大。
 * void led_touch_set(int nMode, int nColor, int nFreq);
 * // 主动查询一次
 * void led_touch_get(); // 主动查询LED

 * // script json
 * bool json_load_by_name(QString strName); // 通过名称加载 base/stand_actions.json中的固定动作
 * bool json_load_isRunning(); // 判断固定动作是否运行
 * void json_load_stop(); // 强制结束固定动作

 * // js module
 * bool js_sub_load_by_name(QString strName); // 在Sub Js引擎中运行base/xxx.js脚本.
 * bool js_sub_load_content(QString strJsContent); // 在Sub Js引擎中运行strJsContent中的内容
 * bool js_sub_isRunning(); // 判断Sub js脚本是否在运行
 * void js_sub_stop(); // 强制结束sub js脚本

 * bool js_main_load_by_name(QString strName); // 在主js引擎中运行usr/scripts/xxx.js脚本.
 * bool js_main_isRunning(); // 判断js引擎是否在运行状态
 * void js_main_stop(); // 强制停止JS脚本

 * // music and tts
 * bool audio_play_tts(QString strTxt); // 以默认参数播放TTS语音
 * bool audio_play_tts_params(QString strTxt, QString strSpeaker, int nSpeed, int nPitch, int nSpecial); // 播放自定义参数TTS语音
 * void audio_play_system(QString strName); // 播放data/audio/xxx.wav文件
 * bool audio_play_isPlaying(); // 判断音频是否在播放状态
 * void audio_stop(); // 强制停止音频
 * void audio_play_system_bg(QString strName); // 在后台播放某个音频。data/audio/xxx.wav
 * bool audio_play_isPlaying_bg(); // 判断后台音频是否在播放状态
 * void audio_stop_bg(); // 强制停止播放

 * // ui
 * void ui_background_set(QString strModeName, bool bShow, QString strFileName); // 设置背景图片(modeset/main true xxx.png)
 * void ui_video_show(QString strFilePath); // 播放某个固定的视频，播放结束后自动隐藏。路径为相对或者绝对路径。
 * bool ui_video_isPlaying(); // 判断视频是否在播放状态
 * void ui_video_stopped(); // 强制停止视频

 * void ui_jump_name(QString strName); // 跳转界面逻辑，同时会触发对应界面的脚本

 * // config
 * QString config_file_get(QString fileName, QString strRootKey); // 读取配置文件。路径为相对或者绝对路径。
 * void config_file_set(QString fileName, QString strRootKey, QString val); // 写入配置文件。路径为相对或者绝对路径。

 * // system control
 * void system_set_volume(int nVolume); // 设置音频播放音量大小。不要随意使用
 
 */
 
function jsEvent(event, val){
    Robot.log("js Event:"+event+" ->"+val);
}

function jsLoop(){
	if( jsState.nFrameID == 1 ){
		Robot.log("run 1");
		Robot.audio_play_tts("我是测试样例");
	}
	if( jsState.nFrameID == 100 ){
		jsState.bStopped = true;
	}
}

