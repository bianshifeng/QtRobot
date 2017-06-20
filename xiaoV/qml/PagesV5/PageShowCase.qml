import QtQuick 2.0
import QtMultimedia 5.0
import "../Component"
import "../Component/UIConstants.js" as UI
import "./PageShowCaseCom"
import TmRobot 1.0


PageBase{
    id: id_root
    pageJsName: "mode2"
    property Robot rootRobot: id_robot
    property alias backImage: id_backImage.source
    property alias foreMedia: id_player.mediaSource
    isShowBackBt: true

    Connections{
        target: rootRobot
        ignoreUnknownSignals: true

        onSig_qmlrobot_ui_background_set:{
            console.log(strFileName)
            if(strFileName){
                id_root.backImage = "file:///" + app_root_dir + strFileName
            }

        }
        onSig_qmlrobot_ui_video_show:{

            console.log(strFilePath)

            if(strFilePath){
                id_root.foreMedia ="file:///"+ app_root_dir + strFilePath
            }

        }
    }

    onEmitBack: id_g_pageManager.navigatPage(pageName,id_g_pageManager.pageMainName,true);

    Item{
        anchors.fill: parent
        Image {
            id: id_backImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            opacity: id_backImage.status == Image.Ready ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: 1000 } }

        }
    }

    ShowPlayer{
        id: id_player
        anchors.centerIn: parent
    }
    ListModel{
        id: id_listModel
        property int currentIndex:0
        ListElement{
            backImage:"/user/data/视频广告-百事可乐.jpg"
            foreMedia:"/user/data/视频广告-百事可乐.flv"
            jsFile:""
        }
        ListElement{
            backImage:"/user/data/图片广告-iphone.png"
            foreMedia:"/user/data/视频广告-百事可乐.flv"
            jsFile:""
        }
        ListElement{
            backImage:"/user/data/图片广告-macbook.png"
            foreMedia:"/user/data/视频广告-百事可乐.flv"
            jsFile:""
        }
        ListElement{
            backImage:"/user/data/图片广告-超市发.png"
            foreMedia:"/user/data/视频广告-百事可乐.flv"
            jsFile:""
        }
    }


    XmsImageButton{
        id: id_left_page_bt
        visible: id_listModel.currentIndex <= 0 ? false : true
        backImage: "qrc:/image/qml/PagesV5/PageImages/showcase/btn_last.png"
        foreImage: backImage
        anchors.left: parent.left
        anchors.leftMargin: -6
        anchors.verticalCenter: parent.verticalCenter
        onEmitClicked: {
            id_listModel.currentIndex = id_listModel.currentIndex - 1
            loadData()
        }

    }

    XmsImageButton{
        id: id_right_page_bt
        visible: id_listModel.currentIndex >= (id_listModel.count - 1) ? false : true
        backImage: "qrc:/image/qml/PagesV5/PageImages/showcase/btn_next.png"
        foreImage: backImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: -6
        onEmitClicked: {
            id_listModel.currentIndex = id_listModel.currentIndex + 1
            loadData()
        }
    }


    XmsButton{
        anchors.centerIn: parent
        text:"sdfsdfsdfsd"
        onClicked:  rootRobot.runJsPageModel()
    }

    Component.onCompleted: {

    }

    function loadData(){

        console.log("----------------------------------------------------------");
        var xx = id_listModel.get(id_listModel.currentIndex)
        if(xx){
            id_root.backImage = "file:///"+ app_root_dir + xx.backImage
            id_root.foreMedia ="file:///"+ app_root_dir + xx.foreMedia
        }
    }

    Connections{
        target: id_robot
        ignoreUnknownSignals: true
    }
}
