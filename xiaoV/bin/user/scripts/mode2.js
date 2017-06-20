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

var modeArray = new Array();
var curDelay = 0;
var nDelayCount = 0;

function isCheckPerson(){
    var nPersons = Robot.sensor_camera_persons();
    if( nPersons > 0 ){
        return true;
    }
    var nChaosheng1 = Robot.sensor_shengbo_distance(0);
    if( nChaosheng1 > 10 && nChaosheng1 < 2000 ){
        return true;
    }
    var nChaosheng2 = Robot.sensor_shengbo_distance(1);
    if( nChaosheng2 > 10 && nChaosheng2 < 2000 ){
        return true;
    }
    return false;
}

function getWelcomeIndex(){
    if( modeArray.length < 1 ){
        // 重新生成
        var nCounts = Robot.config_file_get("./user/scripts/welcome.ini","Welcome/counts");
        var nMode = Robot.config_file_get("./user/scripts/welcome.ini","WelPlayMode/mode");
        var tmpArray = new Array(nCounts);
        for(var i=0;i<nCounts;i++){
            tmpArray[i] = i+1;
        }
        //Robot.log("mode"+nMode);
        for(var i=0;i<nCounts;i++){
            if( nMode > 0 ){
                var nRandval = Robot.nrandom(0,nCounts-1-i);
                modeArray[i] = tmpArray[nRandval];
                tmpArray[nRandval] = tmpArray[nCounts-1-i];
            }else{
                modeArray[i] = i+1;
            }
        }
    }
    return modeArray.shift();
}

function jsEvent(event, val){
    Robot.log("js Event:"+event+" ->"+val);
}

function jsLoop(){
    if( jsState.nFrameID == 1 ){

        Robot.audio_play_tts("固定位置宣传");
    }
    if( jsState.nFrameID < 90 ){
        jsState.nFrameID = 95; 
    }
    //Robot.log(jsState.nFrameID);
    // 100 - 130 招揽
    if( jsState.nFrameID == 100 ){
        var nIndex = getWelcomeIndex();
        curDelay = Robot.config_file_get("./user/scripts/welcome.ini","Welcome/time"+nIndex);
        var randomsay = Robot.config_file_get("./user/scripts/welcome.ini","Welcome/voice"+nIndex);
        Robot.audio_play_tts(randomsay);
        nDelayCount = 0;
        // 切换图片
        var nPicCounts = Robot.config_file_get("./user/scripts/welcome.ini","WelFilePath/counts");
        var nPicIndex = Robot.nrandom(1,nPicCounts);
        var picPath = Robot.config_file_get("./user/scripts/welcome.ini","WelFilePath/picture"+nPicIndex);
        Robot.ui_background_set("modeshow", true, picPath);
    }
    if( jsState.nFrameID == 110 ){
        if( Robot.audio_play_isPlaying() ){
            jsState.nFrameID = 105;
        }
        if( isCheckPerson() ){
        //if(1){
            jsState.nFrameID = 135;
            Robot.log("Check person!!!!!!!!");
        }
    }
    if( jsState.nFrameID == 120 ){
        nDelayCount += 1;
        if( nDelayCount > curDelay ){
            jsState.nFrameID = 124;
            
        }else{
            jsState.nFrameID = 111;
        }
        if( isCheckPerson() ){
        //if(1){
            jsState.nFrameID = 135;
            Robot.log("Check person!!!!!!!!");
        }
    }
    if( jsState.nFrameID == 125 ){
        jsState.nFrameID = 100;
    }
    // 130 之后进行迎宾宣传等
    if( jsState.nFrameID == 140 ){
        Robot.js_sub_load_by_name("xuanchuan");
    }
    if( jsState.nFrameID == 170 ){
        if( Robot.js_sub_isRunning() ){
            jsState.nFrameID = 145;
        }else{
            jsState.nFrameID = 171;
            Robot.log("sub js stopped");
        }
    }
    // 171 之后进行循环
    if( jsState.nFrameID == 190 ){
        jsState.nFrameID = 10;
    }
    // over
    if( jsState.nFrameID >= 2000 ){
        Robot.log("main loop over");
        jsState.bStopped = true;
    }
}

