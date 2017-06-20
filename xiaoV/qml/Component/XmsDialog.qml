import QtQuick 2.0
import "."

XmsPopup{
    id: id_root

    property alias backColor: id_background.color
    property alias dialogWidth: id_body.width
    property alias dialogHeight: id_body.height
    default property alias dialogChildren: id_layout.children

    Item{
        id: id_body
        anchors.centerIn: parent
        implicitHeight: parent.width - 80 *g_dp
        implicitWidth: id_layout.height

        XmsPaperShadow{
            id: id_shadow
            anchors.fill: id_background
            source: id_background
            depth: 3
        }

        Rectangle{
            id: id_background
            anchors.fill: parent
            color: "#fafafa"
            visible: false
            radius: 4
        }

        MouseArea{
            id: id_eventEater
            anchors.fill: parent
            enabled: (id_root.state == "visible")
        }

        Column{
            id: id_layout
            width: parent.width
            clip: true
        }

    }
}
