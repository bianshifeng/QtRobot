import QtQuick 2.0
import "../../Component/UIConstants.js" as UI
import "../../Component"

Item{
    id: id_root
    property real scaleValue: 1
    property int btRadius:34 *scaleValue
    property int value: 1
    property string iconText:UI.ICON_ACTION_DELETE
    property bool isIconText:false
    implicitHeight: btRadius *2
    implicitWidth: btRadius *2
    enabled: id_root.value < 0 ? false: true

    signal emitClicked()

    property color borderColor:UI.COLOR_BASE_BLUE_DARK
    property color backColor:UI.COLOR_BASE_BLUE_BASE
    property color foreColor:UI.COLOR_BASE_GRAY_BASE
    property color textEnterColor:UI.COLOR_BASE_GRAY_DARK
    property color textExitColor:UI.COLOR_BASE_WHITE_BASE


    Rectangle{
        id: id_back
        visible: id_root.enabled
        width: 2*btRadius
        height: 2*btRadius
        radius: btRadius
        border.color:borderColor
        border.width: 1
        anchors.centerIn: parent
        color:backColor


        Rectangle{
            id: id_fore
            width: 2*btRadius
            height: 2*btRadius
            radius: btRadius
            anchors.fill: parent
            color:foreColor
            opacity: 0
        }

        Text {
            id: id_text
            text: id_root.value
            anchors.centerIn: parent
            font.pixelSize: UI.font_size_h1 * scaleValue
            font.bold: false
            color: textExitColor
            smooth: true
            antialiasing: true
            visible: isIconText ? false : true
        }
        XmsIcon{
            text: id_root.iconText
            anchors.centerIn: parent
            font.pixelSize: UI.font_size_h2 * scaleValue
            font.bold: false
            color: textExitColor
            visible: isIconText
            smooth: true
            antialiasing: true

        }



    }

    MouseArea{
        id: id_mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            id_text.font.bold = true
            id_text.color = id_root.textEnterColor
            id_fore.opacity = 0.4

        }
        onPressed: {
            id_text.font.bold = true
            id_text.color = id_root.textEnterColor
            id_fore.opacity = 1
        }
        onReleased: {
            id_text.font.bold = true
            id_text.color = id_root.textEnterColor
            id_fore.opacity = 0.4
        }
        onExited: {
            id_text.font.bold = false
            id_text.color = id_root.textExitColor
            id_fore.opacity =0
        }


        onClicked: emitClicked()

    }


}

//Image{
//    id:id_back

//    signal emitBackClicked()
//    source: "qrc:/image/qml/Images/page/base/btn_ret_on.png"
//    Image{
//        id:id_fore
//        source: "qrc:/image/qml/Images/page/base/btn_ret_pressed.png"
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//        visible: false

//    }

//    MouseArea{
//        id: id_mouseArea
//        anchors.fill: parent
//        onPressed: {
//            id_fore.visible = true
//        }
//        onReleased: {
//            id_fore.visible = false

//        }

//        onClicked: {
//            emitBackClicked()
//        }
//    }

//}
