/* -------------jsState模块---------------------------
 * jsState.nFrameID : int, 当前桢的序号
 * jsState.bStopped : bool, 是否结束
 * jsState.nFrameMSleep : int, 每一帧的停顿时间
 * jsState.bIsModule : bool, 判断自身是否当作模块运行
 * --------------Robot模块-----------------------
 * Robot.log(str) : 日志输出
 * Robot.msleep(nMinSec) : 延时毫秒
 * Robot.audio_play_system("name"); // 播放系统提供的音频文件
 * int Robot.nrandom(int nMin, int nMax); // 获取 [nMin, nMax]区间内的随机数
 */

var bOpenTalk = false;
var timeMax = 600*2;

function jsEvent(event, val){
    Robot.log("js Event:"+event+" ->"+val);
    if(bOpenTalk){
        if( event == "isr" ){
            Robot.tool_ask_answer(val);
        }
        if( event == "tuling" ){
            Robot.audio_play_tts(val);
            Robot.behavior_auto_action(true);
        }
    }
}

function jsLoop(){
    if( jsState.nFrameID == 1 ){
        Robot.log("Run yyy js");
    }
    if( jsState.nFrameID == 3 ){
        //Robot.ui_jump_name("ziding");
        Robot.behavior_auto_talk_open(true);
        bOpenTalk = true;
        Robot.behavior_auto_talk_pause(false);
    }
    if( (jsState.nFrameID % 20) == 18 ){
        if( Robot.audio_play_isPlaying() ){
            bOpenTalk = false;
            Robot.behavior_auto_talk_pause(true);
        }else{
            bOpenTalk = true;
            Robot.behavior_auto_talk_pause(false);
        }
    }
    if( jsState.nFrameID == timeMax ){
        bOpenTalk = false;
        Robot.behavior_auto_talk_open(false);
        Robot.ui_jump_name("main");
    }
    if( jsState.nFrameID >= (timeMax+20) ){
        jsState.bStopped = true; // exit
    }
}

