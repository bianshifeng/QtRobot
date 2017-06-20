/* -------------jsState模块---------------------------
 * jsState.nFrameID : int, 当前桢的序号
 * jsState.bStopped : bool, 是否结束
 * jsState.nFrameMSleep : int, 每一帧的停顿时间
 * jsState.bIsModule : bool, 判断自身是否当作模块运行
 * --------------Robot模块-----------------------
 * Robot.log(str) : 日志输出
 * Robot.msleep(nMinSec) : 延时毫秒
 * Robot.audio_play_system("name"); // 播放系统提供的音频文件
 * int nrandom(int nMin, int nMax); // 获取 [nMin, nMax]区间内的随机数
 */
function isr_getAnswer( strAsk ){
    var nCounts = Robot.config_file_get("./user/scripts/xiaovGrammarEdit.ini", "Grammar_stand/count");
    for(var i=1;i<=nCounts;i++){
        var askTxt = Robot.config_file_get("./user/scripts/xiaovGrammarEdit.ini", "Grammar_stand/ask"+i);
        //Robot.log(i);
        if( askTxt == strAsk ){
            return Robot.config_file_get("./user/scripts/xiaovGrammarEdit.ini", "Grammar_stand/ans"+i);
        }
    }
    return "";
}
function jsEvent(event, val){
    //Robot.log("js Event:"+event+" ->"+val);
    if( event == "isr" ){
        var strAns = isr_getAnswer(val);
        Robot.audio_play_tts(strAns);
        Robot.log("[talk]"+val+"->"+strAns);
    }
}

function jsLoop(){
    if( jsState.nFrameID >= 2000 ){
        Robot.log("run ziding js over");
        jsState.bStopped = true;
    }
}

