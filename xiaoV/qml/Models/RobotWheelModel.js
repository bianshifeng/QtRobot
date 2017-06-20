var Wheel_State_WaitForReady = 0;
var Wheel_State_Ready = 1;
var Wheel_State_WaitForMove =2;
var Wheel_State_Moving =3;
var Wheel_State_WaitForTurn = 4;
var Wheel_State_Turning = 5;
var Wheel_State_WaitForPause = 6;
var Wheel_State_Pause = 7;
var Wheel_State_Stop = 8;

var Wheel_Behavior_Open = 1000;
var Wheel_Behavior_Close = 10001;

var Wheel_Behavior_WaitAlarmStop = 1007;
var Wheel_Behavior_AlarmStop = 10071;

var Wheel_Behavior_Pause = 1002;
var Wheel_Behavior_Pausing = 10021;

var Wheel_Behavior_StartMoving = 1004;
var Wheel_Behavior_Moving = 1005;

var Wheel_Behavior_Paused = 1002;
var Wheel_Behavior_Pausing = 10021;

var Wheel_Behavior_FinishMoving = 1006;
var Wheel_Behavior_FinishedMove = 10061;

var Wheel_BehaviorType_Move = 2001;
var Wheel_BehaviorType_Turn = 2002;



function getAngleZoneDic(dCurrentAngle, bTurnRight, dAngle, diffAngle){

    var dic_zone ={
        "zone1":[],
        "zone2":[]
    }

    var t_zone1 =[0,0]
    var t_zone2=[0,0]

    var t_endAngle = 0
    var t_diffAngle = 0

    dAngle = dAngle - diffAngle

    console.log('[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]');
    console.log(dCurrentAngle)

    if(bTurnRight){
        dCurrentAngle = dCurrentAngle -diffAngle
        t_endAngle = dCurrentAngle + dAngle
        if(dCurrentAngle >= 0){
            t_diffAngle = t_endAngle - 180;
            if(t_diffAngle <= 0){
                t_zone1[0]= dCurrentAngle
                t_zone1[1]= t_endAngle
                t_zone2[0]= dCurrentAngle
                t_zone2[1]= t_endAngle
            }else{
                t_zone1[0] = dCurrentAngle
                t_zone1[1] = 180
                t_zone2[0] = -180;
                t_zone2[1] = -180+t_diffAngle
            }
        }else{
            if(t_endAngle < 0){
                t_zone1[0] = dCurrentAngle
                t_zone1[1] = t_endAngle
                t_zone2[0] = dCurrentAngle
                t_zone2[1] = t_endAngle
            }else{
                t_zone1[0] = dCurrentAngle
                t_zone1[1] = 0
                t_zone2[0] = 0
                t_zone2[1] = t_endAngle
            }
        }

    }else{
        dCurrentAngle = dCurrentAngle + diffAngle

        if(dCurrentAngle >=0){

            t_endAngle = dCurrentAngle - dAngle
            if(t_endAngle <=0){
                t_zone1[0]= t_endAngle
                t_zone1[1]= 0
                t_zone2[0]= 0
                t_zone2[1]= dCurrentAngle
            }else{
                t_zone1[0]= t_endAngle
                t_zone1[1]= dCurrentAngle
                t_zone2[0]= t_endAngle
                t_zone2[1]= dCurrentAngle
            }

        }else{
            t_endAngle = dCurrentAngle - dAngle
            t_diffAngle = t_endAngle + 180
            if(t_diffAngle >=0){
                t_zone1[0]= t_endAngle
                t_zone1[1]= dCurrentAngle
                t_zone2[0]= t_endAngle
                t_zone2[1]= dCurrentAngle
            }else{
                t_zone1[0]= -180
                t_zone1[1]= dCurrentAngle
                t_zone2[0]= t_diffAngle
                t_zone2[1]= 180
            }
        }

    }



    dic_zone.zone1 = t_zone1
    dic_zone.zone2 = t_zone2

    return dic_zone
}

function getQuadrantAngle(resetAngle,startAngle,endAngle){


}

function isTurnToEnd(angleValue,zone1,zone2){
    var t_zone1 = zone1
    var t_zone2 = zone2
    if(angleValue >= t_zone1[0] && angleValue <= t_zone1[1] ){
        return false;
    }
    if(angleValue >= t_zone2[0] && angleValue <= t_zone2[1] ){
        return false;
    }

    console.log("ST.Wheel_Behavior_FinishMoving")
    console.log(angleValue)
    return true;
}


function isMoveToEnd(currentWheelValue,startWheelValue,endWheelValue){

    console.log("startWheelValue :" + startWheelValue)
    console.log("currentWheelValue :" + currentWheelValue)
    console.log("endWheelValue : " + endWheelValue)
    if(currentWheelValue >= startWheelValue && currentWheelValue <= endWheelValue){
        return false;
    }
    console.log("ST.Wheel_Behavior_FinishMoving")
    console.log(currentWheelValue)
    return true;
}
