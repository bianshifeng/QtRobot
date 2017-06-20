import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import TmRobot 1.0


PageBase {
    id: id_root


    GridLayout{
        id:id_layout
        anchors.centerIn: parent
        columnSpacing: 4
        rowSpacing: 4
        columns: 4
        rows:4

        Button{
            text:"left"
            onClicked: {
                id_robot.moveBehavior(Robot.MoveBack,1,10,1);
            }
        }

        Button{
            text:"right"
            onClicked: {
                id_robot.changePageName("bianshifeng")
            }
        }
        Button{
            text:"fore"
        }
        Button{
            text:"back"
        }
    }


    Component.onCompleted: {
        id_robot.pageJsName = id_root.pageJsName
    }
}

