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

function jsEvent(event, val){
    Robot.log("js Event:"+event+" ->"+val);
}

function jsLoop(){
    if( (jsState.nFrameID % 50) == 1 ){
        var nVal = Robot.sensor_hongwai_distance(0);
        Robot.log("Hongwai: 0 -> "+nVal);
        nVal = Robot.sensor_hongwai_distance(1);
        Robot.log("Hongwai: 1 -> "+nVal);
        nVal = Robot.sensor_hongwai_distance(2);
        Robot.log("Hongwai: 2 -> "+nVal);
        nVal = Robot.sensor_hongwai_distance(3);
        Robot.log("Hongwai: 3 -> "+nVal);
        nVal = Robot.sensor_shengbo_distance(0);
        Robot.log("Chaosheng: 0 -> "+nVal);
        nVal = Robot.sensor_shengbo_distance(1);
        Robot.log("Chaosheng: 1 -> "+nVal);
    }
    // over
    if( jsState.nFrameID >= 6000 ){
        Robot.log("main loop over");
        jsState.bStopped = true;
    }
}

