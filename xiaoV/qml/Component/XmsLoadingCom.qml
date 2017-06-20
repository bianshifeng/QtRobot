import QtQuick 2.0

Item {
    id: id_root
    implicitHeight: id_load_outer.height
    implicitWidth: id_load_outer.width
    property alias loadBackImage:id_load_outer.source
    property alias loadForeImage:id_load_fore.source
    property int loadPercent:0


    Image{
        id: id_load_outer
        smooth: true
        Item{
            id: id_loadMask
            clip: true
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: loadPercent >= 100? id_load_outer.width : (id_load_outer.width * loadPercent / 100)
            Image{
                id: id_load_fore
                smooth: true
                anchors.left: parent.left
            }

            NumberAnimation on width {
                duration: 3000
                easing.type: Easing.InQuart
            }
        }

    }

}

