import QtQuick 2.0
import "./PageBaseCom"
import "../Component"

FocusScope{
    id: id_root
    focus: true
    property alias pageName:id_root.objectName
    property alias backImage:id_backImg.source
    property alias isShowBackBt:id_backBt.visible
    property alias isShowExitBt: id_exitBt.visible
    property alias isShowBtckBtImage:id_backBt.showImage
    property alias isShowLogo:id_logo.visible
    property string pageJsName:""
    implicitHeight: 800
    implicitWidth: 600

    signal emitBack()
    signal emitExit()

    Image{
        id: id_backImg
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
        source: "qrc:/image/qml/PagesV5/PageImages/load/loading_bg_back.png"
    }

    function init(){

    }

//    BackupButton{
//        id: id_backBt
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//        onEmitBackClicked: emitBack()
//        visible: false
//    }

    XmsImageButton{
        id: id_backBt
        z:1000
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onEmitClicked: emitBack()
        visible: false
        showImage: false
    }

    XmsImageButton{
        id: id_exitBt
        z:1000
        foreImage: "qrc:/image/qml/PagesV5/PageImages/main/shutdown_22.png"
        backImage: foreImage
        anchors.right: parent.right
        anchors.top: parent.top
        onEmitClicked: {
            emitExit()
        }
        visible: false
    }



    Image {
        id: id_logo
        source: "qrc:/image/qml/PagesV5/PageImages/main/icon_logo.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
    }



    Component.onCompleted: {
        if(parent){
            anchors.fill = parent
        }
        if(id_root.pageJsName)
            id_robot.pageJsName = id_root.pageJsName;
    }

}

