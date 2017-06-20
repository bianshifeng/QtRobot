import QtQuick 2.0
import "./RobotWheelModel.js" as Util
import "./RobotWheel"
import QtQuick.Controls 1.2

Rectangle {
    id: id_root
    scale: 3
    width: 500
    height: 200
    color:"gray"

    property real stepStartAngle:0
    property real stepTurnEndAngle:0
    property real stepMoveEndAngle:0
    property real stepEndAngle: 0

    function reset(){
        stepStartAngle = id_wheel.currentAngle
        stepEndAngle = id_wheel.currentAngle
        stepMoveEndAngle = 0
        stepTurnEndAngle = 0
    }

    RobotWheelBehavior{
        id: id_behavior
        turnAngle: 90
        turnRight: true
        moveDistance: 200
        moveForward: true

        onEmitTurnComplete: {
            console.log("onEmitTurnComplete")
            status = Util.Wheel_State_WaitForMove
            console.log("onEmitTurnComplete")
        }
        onEmitMoveComplete: {
            console.log("onEmitMoveComplete")
        }
    }



    Row{
        id: id_row
        anchors.centerIn: parent
        Button{
            text:"start ready"
            onClicked: id_behavior.status = Util.Wheel_State_WaitForReady

        }
        Button{
            text:"start turn right"
            onClicked: {

                id_behavior.status = Util.Wheel_State_WaitForTurn
                id_behavior.turnRight = id_behavior.turnRight ? false: true
            }


        }
        Button{
            text:"start turn left"
            onClicked: {

                id_behavior.status = Util.Wheel_State_WaitForTurn
                id_behavior.turnRight = false
            }

        }

        Button{
            text:"start move"
            onClicked: id_behavior.status = Util.Wheel_State_WaitForMove
        }

        Button{
            text:"start pause"
            onClicked: id_behavior.status = Util.Wheel_State_WaitForPause

        }
    }

    Row{
        id: id_info_row
        anchors.bottom: id_row.top
        Text{
            text: id_behavior.currentAngle
            width: 200
        }
        Text{
            text: id_behavior.leftCode
            width: 200
        }
        Text{
            text: id_behavior.rightCode
            width: 200
        }

    }







}

