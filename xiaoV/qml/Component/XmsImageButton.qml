import QtQuick 2.0

Item{
    id: id_root
    implicitHeight: id_back.height
    implicitWidth: id_back.width
    property alias backImage:id_back.source
    property alias foreImage: id_fore.source
    property alias showImage:id_container.visible
    signal emitClicked()

    Item{
        id:id_container
        anchors.fill: parent
        Image{
            id:id_back
            source: "qrc:/image/qml/PagesV5/PageImages/base/btn_ret_on.png"
            opacity: 0.6
        }
        Image{
            id:id_fore
            source: "qrc:/image/qml/PagesV5/PageImages/base/btn_ret_pressed.png"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            opacity: 0
        }
    }
    MouseArea{
        id: id_mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            id_fore.opacity = 0.4

        }
        onExited: {
            id_fore.opacity = 0
        }

        onPressed: {
            id_fore.opacity = 1
        }

        onClicked: {
            emitClicked()
        }
    }


}

