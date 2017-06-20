import QtQuick 2.0

import "../Component/UIConstants.js" as UI

import "../Component"



PageBase {
    id: id_root
    pageJsName: "JsPageSetting"
    isShowBackBt: true
    isShowBtckBtImage: true
    onEmitBack: id_g_pageManager.navigatPage(id_root.pageName,id_g_pageManager.pageMainName,true);
    isShowExitBt: true
    onEmitExit: Qt.quit()




    Column{
        anchors.centerIn: parent
        spacing: 20
        XmsButton{
            text:"系统自检"
            backColor: UI.COLOR_BASE_BLUE_BASE
            onClicked: id_robot.runJsPageModel("JsModelSelfCheck")
        }


        XmsButton{
            text:"系统自检"
            backColor: UI.COLOR_BASE_BLUE_BASE
            onClicked: id_robot.runJsPageModel("JsModelSelfCheck")
        }

    }











    Component.onCompleted: {
        id_robot.startSay("你好，这是配置界面")
    }

}

