import QtQuick 2.0

Image{
    id:id_back

    signal emitBackClicked()
    source: "qrc:/image/qml/PagesV5/PageImages/base/btn_ret_on.png"
    Image{
        id:id_fore
        source: "qrc:/image/qml/PagesV5/PageImages/base/btn_ret_pressed.png"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        opacity: 0

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
            emitBackClicked()
        }
    }

}
