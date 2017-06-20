import QtQuick 2.0
import "./UIConstants.js" as UI

Item{
    id: id_root
    property bool bold: false
    property alias size: id_text.size
    property alias text: id_text.text
    property alias color: id_text.color
    property color backColor:UI.COLOR_BASE_TRANSPARENT
    property int radius:4
    objectName: "button"
    implicitHeight: 60
    implicitWidth: 100
    signal clicked()

    Rectangle{
        anchors.fill: parent
        color:backColor
        radius: id_root.radius
    }

    XmsText{
        id:id_text
        anchors.centerIn: parent
        font.bold: id_root.bold
        color: UI.COLOR_BASE_WHITE_BASE
    }

    MouseArea{
        id: id_mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            id_text.opacity = 0.9
            id_text.font.bold = true
        }
        onExited: {
            id_text.opacity = 0.7
            id_text.font.bold = id_root.bold
        }
        onReleased: {
            id_text.opacity = 0.9
            id_text.font.bold = false

        }

        onPressed: {
            id_text.opacity = 1
            id_text.font.bold = true

        }
        onClicked: {
            id_root.clicked()
        }

    }


}
