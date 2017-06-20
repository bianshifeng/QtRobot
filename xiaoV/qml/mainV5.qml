import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.2
import "./PagesV5"
import "./Component/XmsPwdCom"
import "./Component"
import "./Component/UIConstants.js" as UI
import QtQuick.Layouts 1.1
import TmRobot 1.0
import "./Models"

import QtMultimedia 5.5


FocusScope{
    id: id_window
    property real g_ScaleValue: Screen.width / 1920
    visible: true
    x:0
    y:0
    width: Screen.width+1
    height: Screen.height + 1
//    height: 600

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
        onSig_change_page_by_name: {
            console.log(pageName);
            id_g_pageManager.navigatPage("",pageName,false)

        }

    }


    PageManager{
        id: id_g_pageManager
    }

    PageBase{
        id: id_container
        anchors.fill: parent
        anchors.bottomMargin: 1
        anchors.rightMargin: 1

        Loader{
            id: id_page_loader
            anchors.fill: parent
        }

        Loader{
            id: id_pwd_loader
            anchors.fill: parent
        }

    }


//    RobotWheelStep{
//        anchors.centerIn: parent
//    }


    Component.onCompleted: {
        id_g_pageManager.__loadPageMain()
    }

}
