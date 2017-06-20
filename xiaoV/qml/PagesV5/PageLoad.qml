import QtQuick 2.0
import "../Component"
import QtQuick.Controls 1.2
PageBase {
    id: id_root
    pageJsName: "pageload"

    Image {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -260
        source: "qrc:/image/qml/PagesV5/PageImages/load/loading_bg_logo.png"
    }

    XmsLoadingCom{
        id: id_loader
        anchors.centerIn: parent
        loadBackImage: "qrc:/image/qml/PagesV5/PageImages/load/p_bg.png"
        loadForeImage: "qrc:/image/qml/PagesV5/PageImages/load/p_fg.png"
        loadPercent: 0
    }


    Button{
        onClicked:  id_loader.loadPercent = id_loader.loadPercent + 2
    }


    Image {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 140
        source: "qrc:/image/qml/PagesV5/PageImages/load/loading_bg_name.png"
    }




}

