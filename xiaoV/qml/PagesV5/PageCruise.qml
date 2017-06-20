import QtQuick 2.0
import QtQuick.Controls 1.2
import "../Models"
import "../Component"


PageBase {
    id: id_root
    pageJsName: "JsCruisePage"
    isShowBackBt: true
    isShowBtckBtImage: true
    onEmitBack: id_g_pageManager.navigatPage(id_root.pageName,id_g_pageManager.pageMainName,true);
    isShowExitBt: true
    onEmitExit: Qt.quit()


    Column{
        anchors.centerIn: parent


        RobotWheelModel{
            id: id_wheel_02
            property int stepNum: 0
            onEmitGotoNextStep: {
                stepNum = stepNum + 1
                id_run_bt.text = stepNum

//                startStep()
            }

            function startStep(){
                var t_model = id_move_listModel.get(stepNum)
                id_wheel_02.leftWheelSpeed = t_model.leftSpeed
                id_wheel_02.rightWheelSpeed = t_model.rightSpeed
                id_wheel_02.turnAngle = t_model.turnAngle
                id_wheel_02.distanceWheelCode = t_model.moveDistance
                id_wheel_02.isTurnRight = t_model.isTurnRight
                id_wheel_02.isForward = t_model.isForward
                id_wheel_02.currentBehaviorType = t_model.behaviorType

                id_wheel_02.autoStart()
            }


        }

        Button{
            id: id_run_bt
            text:"run"
    //            onClicked: id_wheel_01.moveDistance(30000,2)
            onClicked: id_wheel_02.rotateAngle(90,1,1,true);
        }


    }


    ListModel{
        id: id_move_listModel
        ListElement{
            index:1
            behaviorType:1
            moveDistance:600000
            isForward:true
            turnAngle:0
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:2
            moveDistance:0
            isForward:true
            turnAngle:90
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:1
            moveDistance:60000
            isForward:true
            turnAngle:0
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:2
            moveDistance:0
            isForward:true
            turnAngle:90
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:1
            moveDistance:600000
            isForward:true
            turnAngle:0
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:2
            moveDistance:0
            isForward:true
            turnAngle:90
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:1
            moveDistance:60000
            isForward:true
            turnAngle:0
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:2
            moveDistance:0
            isForward:true
            turnAngle:90
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:1
            moveDistance:600000
            isForward:true
            turnAngle:0
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:2
            moveDistance:0
            isForward:true
            turnAngle:90
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:1
            moveDistance:60000
            isForward:true
            turnAngle:0
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }
        ListElement{
            index:1
            behaviorType:2
            moveDistance:0
            isForward:true
            turnAngle:90
            isTurnRight:true
            leftSpeed:4
            rightSpeed:4
        }


    }




}

