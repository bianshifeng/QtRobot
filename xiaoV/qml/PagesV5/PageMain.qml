import QtQuick 2.4
import "../Component"

PageBase {
    id: id_root
    pageJsName: "MainPage"
    isShowExitBt: false


    Row{
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        z:1000
//        XmsImageButton{
//            foreImage: "qrc:/image/qml/PagesV5/PageImages/main/config_1.png"
//            backImage: "qrc:/image/qml/PagesV5/PageImages/main/config_2.png"
//            onEmitClicked: Qt.quit()

//        }
        XmsImageButton{
            foreImage: "qrc:/image/qml/PagesV5/PageImages/main/shutdown_22.png"
            backImage: foreImage
            onEmitClicked: Qt.quit()
        }

    }


    Row{
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 200
        XmsImageButton{
            backImage:"qrc:/image/qml/PagesV5/PageImages/main/btn_mode_1.png"
            foreImage: backImage
            onEmitClicked: {
                id_g_pageManager.navigatPage(id_root.pageName,id_g_pageManager.pageCruiseName,false);
            }
        }
        XmsImageButton{
            backImage:"qrc:/image/qml/PagesV5/PageImages/main/btn_mode_2.png"
            foreImage: backImage
            onEmitClicked: {
                console.log("11111111111111111111111111111111111111")
                id_g_pageManager.navigatPage(id_root.pageName,id_g_pageManager.pageShowCaseName,true)
            }
        }
    }


}

