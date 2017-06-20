import QtQuick 2.0
import "../Component/XmsPwdCom"
import "../Component"
PageBase{
    id:id_root
    pageJsName: "js_passwordpage"

    property string currentPageName:""
    property string wantEnterPageName:""


    XmsPwdCtrl{
        id: id_pwdctrl
        anchors.centerIn: parent
        onEmitPasswordChanged: {
            id_autoCloseTimer.restart()
            id_robot.pwdEnter(password)
        }
        onEmitBackCurrentPage: {
            id_g_pageManager.__closePwdPage()
        }
    }


    Connections{
        target: id_robot
        onEmitEnterPwdOK:{
            id_g_pageManager.navigatPage(id_root.currentPageName,id_root.wantEnterPageName,false);
        }
        onEmitEnterPwdFailture:{
            id_pwdctrl.worngMessage = qsTr("密码错误")
        }
    }


    Timer{
        id: id_autoCloseTimer
        interval: 20000
        onTriggered: id_g_pageManager.__closePwdPage()

    }

    Component.onCompleted: {
        id_autoCloseTimer.restart()
    }
}
