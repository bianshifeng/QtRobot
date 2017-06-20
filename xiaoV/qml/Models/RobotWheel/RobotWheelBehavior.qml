import QtQuick 2.0
import TmRobot 1.0
import "../RobotWheelModel.js" as Util

Item{
    id: id_root
    implicitHeight: 200
    implicitWidth: 800
    property Robot qmlRobot:id_robot
    property bool isActive: false
    property int status: Util.Wheel_State_WaitForReady
    property int turnAngle:0
    property int moveDistance:0
    property bool turnRight: true
    property bool moveForward: true

    property int baseSpeed:4

    property real startX:0
    property real startY:0
    property real currentX:0
    property real currentY:0
    property real currentAngle:0
    property real endX:0
    property real endY:0

    property int leftCode:0
    property int rightCode:0


    function loopDoBehavior(){

        function doReset(){
            qmlRobot.resetWheelCode()
            status = Util.Wheel_State_Ready
            isActive = true
        }
        function doMove(){
            qmlRobot.moveStart()
//            qmlRobot.moveDiffBehavior(moveForward,moveDistance,10)
            qmlRobot.moveBehavior(moveForward,moveDistance,5)
             console.log("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
            status = Util.Wheel_State_Moving

        }
        function doTurn(){
            qmlRobot.moveStart()

            qmlRobot.rotateBehavior(turnRight,turnAngle,2)
            status = Util.Wheel_State_Turning
        }
        function doPause(){
            qmlRobot.moveStop()
        }



        if(status === Util.Wheel_State_WaitForReady){
            doReset()
        }
        else if(status === Util.Wheel_State_WaitForMove){
            doMove()
        }
        else if(status === Util.Wheel_State_WaitForTurn){
            doTurn()
        }
        else if(status === Util.Wheel_State_WaitForPause){
            doPause()
        }

    }

    signal emitMoveComplete()
    signal emitTurnComplete()
    signal emitPaused()
    signal emitStoped()

    Connections{
        target: qmlRobot
        ignoreUnknownSignals: true
        onSig_qmlrobot_wheel_stop_normal_status: {

            if(!id_root.isActive) return;
            if(status ===  Util.Wheel_State_Moving){
                console.log('tttttttttttttttttttttttttttttttttttttttttt')
                emitMoveComplete()
                id_timer.restart()
            }
            else if(status === Util.Wheel_State_Turning){
                console.log('tttttttttttttttttttttttttttttttttttttttttt')
                emitTurnComplete()
                id_timer.restart()
            }
            else if(status === Util.Wheel_State_WaitForPause){
                status = Util.Wheel_State_Pause
                emitPaused()

            }else{
                emitStoped()

            }

            id_timer.restart()

        }
        onSig_qmlrobot_cruise_info: {
            currentAngle = yaw
            leftCode= leftWheelCode
            rightCode = rightWheelCode
        }
    }



    Timer{
        id: id_timer
        interval: 400
        repeat: true
        onTriggered: {
            loopDoBehavior()
        }
    }

    Component.onCompleted: {
        qmlRobot.queryWheelCodeStart()
        id_timer.start()
    }
    Component.onDestruction: {
        qmlRobot.queryWheelCodeStop()
        id_timer.stop()
    }
}
