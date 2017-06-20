import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.2
import "./Pages"
import TmRobot 1.0




FocusScope{
    id: id_window
    property real g_ScaleValue: 1920 / 1920
    visible: true
    x:0
    y:0
    width: Screen.width
    height: 500
    PageManager{
        id: id_g_pageManger
    }

    Robot{
        id: id_robot
        signal emitEnterPwdOK()
        signal emitEnterPwdFailture()
        function pwdEnter(pwd){
            if(pwd === "12345"){
                emitEnterPwdOK()
            }else{
                emitEnterPwdFailture()
            }

        }
    }

    PageEditor{}

//    PageCamera{}

    PageTalk{}

    Component.onCompleted: {
//        id_g_pageManger.__loadPageLoad()
    }

    Loader{
        id: id_page_loader
        anchors.fill: parent
    }
    Loader{
        id: id_pwd_loader
        anchors.fill: parent
    }

    Button{
        anchors.top:parent.top
        anchors.right: parent.right
        width: 40
        height: 40
        text:"X"
        onClicked: Qt.quit()
    }

}
