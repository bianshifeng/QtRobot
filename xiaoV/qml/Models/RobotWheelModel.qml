import QtQuick 2.4
import TmRobot 1.0
import "../Component"
import "../Component/UIConstants.js" as UI
import "./RobotWheelModel.js" as Util
import QtQuick.Controls 1.2


Item{
    id: id_root
    implicitHeight: 200
    implicitWidth: 800
    property Robot qmlRobot:id_robot
    property int wheelBehaviorState: Util.Wheel_Behavior_Close
    property bool isActive: false
    property int counter: 0
    signal emitGotoNextStep(var currentX, var currentY,var currentAngle)








    property real startAngle:0
    property real referAngle:0
    property real borderX:0
    property real borderY:0
    property real borderWidth:0
    property real borderLength:0

    property real stepMoveLength:0
    property real stepTurnAngle:0
    property real startX:0
    property real startY:0
    property real currentX:0
    property real currentY:0




    function reset(){
        startAngle = 0
        referAngle = currentAngle
        borderWidth = 10000
        borderLength = 20000

    }

    signal alarmOverflow(var endX,var endY)
    function doCheckBoundAlarm(){
        var t_angle = stepTurnAngle - referAngle
        var t_length = stepMoveLength

        var t_xLength = Math.cos(t_angle) * t_length
        var t_yLength = Math.sin(t_angle) * t_length

        currentX = startX + t_xLength
        currentY = startY + t_yLength

        if(Math.abs(currentX)>borderWidth/2){
            wheelBehaviorState = Util.Wheel_Behavior_WaitAlarmStop

        }

        if(Math.abs(currentY)>borderLength/2){
            wheelBehaviorState = Util.Wheel_Behavior_WaitAlarmStop
        }


    }



    property int currentBehaviorType: 1 //
    property real turnAngle:90
    property real currentAngle:0
    property var turnAngleDic:null
    property bool isTurnRight:true




    //
    property real moveStartAngle:0
    property real moveEndAngle:0
    property int currentWheelCode:0
    property int currentLeftWheelCode:0
    property int currentRightWheelCode:0
    property int startWheelCode:0
    property int endWheelCode:3000000
    property int distanceWheelCode:30000000

    property int baseWheelSpeed: 10
    property int rightWheelSpeed: 10
    property int leftWheelSpeed: 10
    property bool isForward:true

    property bool speedChanged: false

    property int stopCount:0

    function autoStart(){
        if(currentBehaviorType === 1){
            id_root.moveDistance(id_root.distanceWheelCode,
                                 id_root.baseWheelSpeed,
                                 id_root.isForward)
        }
        if(currentBehaviorType === 2){
            id_root.rotateAngle(id_root.turnAngle,
                                id_root.leftWheelSpeed,
                                id_root.rightWheelSpeed,
                                id_root.isTurnRight)
        }
    }


    function rotateAngle(dAngle,nLSpeed,nRSpeed,isRight){
        if(dAngle > 180){
            return
        }

        turnAngle = dAngle
        id_root.open()

        if(isRight){
            leftWheelSpeed = Math.abs(nLSpeed)
            rightWheelSpeed =  -Math.abs(nRSpeed)
        }else{

            leftWheelSpeed =  -Math.abs(nLSpeed)
            rightWheelSpeed =  Math.abs(nRSpeed)
        }
        currentBehaviorType = 2
        turnAngle = dAngle
        turnAngleDic= Util.getAngleZoneDic(currentAngle,isRight,dAngle,2)
        id_root.move()

    }
    function moveDistance(nDistance,nSpeed,isForward){
        id_root.open()

        if(isForward){
            leftWheelSpeed = Math.abs(nSpeed)
            rightWheelSpeed = Math.abs(nSpeed)
        }else{
            leftWheelSpeed = -Math.abs(nSpeed)
            rightWheelSpeed = -Math.abs(nSpeed)
        }


        moveStartAngle = moveEndAngle =  currentAngle

        currentBehaviorType = 1
        startWheelCode = 0
        currentWheelCode = 0
        endWheelCode = startWheelCode + nDistance

        id_root.move()
    }
    function refreshWheelBehaivor(){
        if(!id_root.isActive) return;
        if(wheelBehaviorState===Util.Wheel_Behavior_Paused){
            return;
        }else if(wheelBehaviorState === Util.Wheel_Behavior_Open ){
            return;
        }
        else if(wheelBehaviorState === Util.Wheel_Behavior_Close){
            return;
        }
        else if(wheelBehaviorState === Util.Wheel_Behavior_StartMoving){
            qmlRobot.moveStart();
            wheelBehaviorState = Util.Wheel_Behavior_Moving;

        }
        else if(wheelBehaviorState === Util.Wheel_Behavior_Moving){
            if(currentBehaviorType === 2){
                var isToend =Util.isTurnToEnd(currentAngle,turnAngleDic.zone1,turnAngleDic.zone2)
                if(isToend) {
                    wheelBehaviorState = Util.Wheel_Behavior_FinishMoving
                    return;
                }
            }
            if( currentBehaviorType === 1){
                var t_isMoveToEnd = Util.isMoveToEnd(currentWheelCode,startWheelCode,endWheelCode)
                if(t_isMoveToEnd){
                    wheelBehaviorState = Util.Wheel_Behavior_FinishMoving
                    return;
                }
            }
            if(isActive){


                var t_leftSpeed = moveDiffAngleModify_Left()
                var t_rightSpeed =moveDiffAngleModify_right()

                id_tx_leftWheelSpeed.text = t_leftSpeed
                id_tx_rightWheelSpeed.text = t_rightSpeed

                qmlRobot.moveDiffBehaviorAdvance(t_leftSpeed,t_rightSpeed)
            }

            return;
        }

        else if(wheelBehaviorState === Util.Wheel_Behavior_Pausing) {

            qmlRobot.moveStop()
            id_timer.stop()
            return;
        }
        else if(wheelBehaviorState === Util.Wheel_Behavior_FinishMoving){
            qmlRobot.moveStop()
            id_timer.stop()
            return;
        }
        else if(wheelBehaviorState === Util.Wheel_Behavior_WaitAlarmStop){
            qmlRobot.moveStop()
            id_timer.stop()
            return;
        }

    }

    function moveDiffAngleModify_Left(){

        var t_modify = 0
        if(currentBehaviorType === 1){
            var diffCode =  currentRightWheelCode - currentLeftWheelCode
            if(diffCode > 0){
                t_modify = 2//Math.round(diffCode/1000)
            }
        }
        return leftWheelSpeed + t_modify

    }

    function moveDiffAngleModify_right(){

        var t_modify = 0
        if(currentBehaviorType === 1){
            var diffCode =  currentLeftWheelCode - currentRightWheelCode
            if(  diffCode > 0){
                t_modify = 2//Math.round(diffCode/1000)
            }

        }
        return rightWheelSpeed + t_modify
    }


    function open(){
        wheelBehaviorState = Util.Wheel_Behavior_Open
        qmlRobot.resetWheelCode()
        id_timer.restart()
        id_root.isActive = true
    }
    function close(){
        id_root.isActive = false
        wheelBehaviorState = Util.Wheel_Behavior_Close
        id_timer.stop()
    }
    function move(){
        wheelBehaviorState = Util.Wheel_Behavior_StartMoving
        id_timer.restart()
    }
    function pause(){
        wheelBehaviorState = Util.Wheel_Behavior_Pausing
        id_timer.restart()
    }
    function finish(){
        wheelBehaviorState = Util.Wheel_Behavior_FinishMoving
        id_timer.restart()
    }
    function stop(){
        wheelBehaviorState = Util.Wheel_Behavior_Paused
        id_timer.stop()
        qmlRobot.moveStop()
    }


    onEmitGotoNextStep: {
        id_root.close()
    }

    Component.onCompleted: {
        qmlRobot.queryWheelCodeStart()
    }
    Component.onDestruction: {
        qmlRobot.queryWheelCodeStop()
    }

    Timer{
        id: id_timer
        interval: 100
        repeat: true
        onTriggered: {
            counter = counter + 1
            refreshWheelBehaivor()
        }
    }


    Connections{
        target: qmlRobot
        onSig_qmlrobot_wheel_stop_normal_status:{
            if(!id_root.isActive) return;
            if(wheelBehaviorState === Util.Wheel_Behavior_Pausing){
               wheelBehaviorState =Util.Wheel_Behavior_Paused
            }

            if(wheelBehaviorState === Util.Wheel_Behavior_FinishMoving){
                wheelBehaviorState = Util.Wheel_Behavior_FinishedMove
                emitGotoNextStep(currentX,currentY,currentAngle)
            }

            if(wheelBehaviorState === Util.Wheel_Behavior_WaitAlarmStop){
                wheelBehaviorState = Util.Wheel_Behavior_AlarmStop
                alarmOverflow(currentX,currentY,currentAngle)
            }



        }
        onSig_qmlrobot_cruise_info:{

            var t_leftWheelValue = leftWheelCode
            var t_rightWheelValue = rightWheelCode
            var t_yaw = yaw

            id_tx_currentAngle.text = t_yaw
            id_wheelcode_left.text = t_leftWheelValue
            id_wheelcode_right.text = t_rightWheelValue




            currentAngle = t_yaw
            if(!id_root.isActive) return;
            currentLeftWheelCode = t_leftWheelValue
            currentRightWheelCode = t_rightWheelValue
            currentWheelCode = currentLeftWheelCode


        }

    }

    TextFontLoader{
        id: id_font
    }

    Column{
        scale: 1
        anchors.centerIn: parent
        Row{
            spacing: 10

            Button{
                text:"Move"
                onClicked: id_root.move()
            }
            Button{
                text:"Pause"
                onClicked: id_root.pause()
            }
            Button{
                text:"gotonextStep"
                onClicked: id_root.finish()
            }

            Button{
                text:"急停"
                onClicked: {
                    qmlRobot.moveStop()
                }
            }
        }

        Row{
            spacing: 10
            XmsText {
                text: "目标码盘值："
            }
            TextInput {
                id: id_wheelcode_input
                font.family: id_font.name
                font.pixelSize: id_font.fontSize
                width: 100


                text:id_root.endWheelCode
            }
            Button{
                text:"码盘重置"
                onClicked: qmlRobot.resetWheelCode()
            }

            XmsText {
                text: "左轮速度："
            }
            XmsText {
                id: id_tx_leftWheelSpeed
                text: id_root.leftWheelSpeed
                width: 100
            }
            XmsText {
                text: "右轮速度："
            }
            XmsText {
                id: id_tx_rightWheelSpeed
                text:id_root.rightWheelSpeed
                width: 100
            }

        }


        Row{
            spacing: 10
            XmsText {
                text: "当前码盘值："

            }
            XmsText {
                id: id_wheelcode
                width: 100
            }
            XmsText {
                text: "左轮码盘值："
            }
            XmsText {
                id: id_wheelcode_left
                width: 100
            }
            XmsText {
                text: "右轮码盘值："
            }
            XmsText {
                id: id_wheelcode_right
                width: 100
            }
        }
        Row{
            spacing: 10
            XmsText {
                text: "当前偏航值："
            }
            XmsText {
                id: id_tx_currentAngle
                width: 100
                clip:true
            }
            XmsText {
                text: "旋转角度："
            }
            XmsText {
                text:id_root.turnAngle
                width: 100
                clip:true
            }
            XmsText {
                text: "偏航范围："

            }
            XmsText {
                width: 300

                text:JSON.stringify(turnAngleDic)
            }

        }

        Button{
            text:"启动"
            onClicked: id_root.autoStart()
        }
    }





}


//function updateWheelSpeed(leftWheelCode,rightWheelCode){
//    var t_value = Math.abs(leftWheelCode - rightWheelCode)
//    if(t_value > 2000){
//        if(leftWheelCode > rightWheelCode){
//            if(leftWheelSpeed > baseWheelSpeed){
//                leftWheelSpeed = leftWheelSpeed - 1
//            }else{
//                rightWheelSpeed = rightWheelSpeed + 1
//            }
//        }else{
//            if(rightWheelSpeed > baseWheelSpeed){
//                rightWheelSpeed = rightWheelSpeed - 1
//            }else{
//                leftWheelSpeed = leftWheelSpeed + 1
//            }
//        }
//    }
//}

