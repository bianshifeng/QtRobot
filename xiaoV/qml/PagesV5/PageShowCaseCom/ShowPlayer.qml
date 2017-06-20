import QtQuick 2.0
import QtMultimedia 5.0
import "../../Component"
import "../../Component/UIConstants.js"  as UI

Item {
    id: id_root
    implicitWidth: 600
    implicitHeight: 500
    anchors.centerIn: parent
    property alias mediaSource: id_mediaplayer.source
    property bool isTest:true



    MediaPlayer {
        id: id_mediaplayer
        autoPlay: true
        onStopped:{
            id_output.opacity = 0
            id_player_ctrl_bt.text  =UI.ICON_PLAYER_PLAY
        }
        onPlaying: {
            id_output.opacity = 1
            id_player_ctrl_bt.text  =UI.ICON_PLAYER_PAUSE
        }
        onPaused: {
            id_player_ctrl_bt.text  =UI.ICON_PLAYER_PLAY
        }






    }

    VideoOutput {
        id: id_output
        opacity: 0
        anchors.fill: parent
        source: id_mediaplayer

    }
    MouseArea {
        id: playArea
        anchors.fill: parent
        hoverEnabled: true
        onReleased: {

            if(id_mediaplayer.playbackState == MediaPlayer.PlayingState){
                id_mediaplayer.pause();
            }else{
                id_mediaplayer.play();
            }
        }

        onEntered:id_player_bt.opacity = 1
        onExited: id_player_bt.opacity = 0.2
        Rectangle{
            id: id_player_bt
            opacity: 0.2
            anchors.centerIn: parent
            width: 100
            height: 100
            border.width: 2
            color:UI.COLOR_BASE_TRANSPARENT
            border.color: UI.COLOR_BASE_WHITE_LIGHT
            radius: width/2

            XmsIcon{
                id: id_player_ctrl_bt
                text:UI.ICON_PLAYER_PLAY
                anchors.centerIn: parent
                size:UI.font_size_icon_logo_main
                color:UI.COLOR_BASE_WHITE_LIGHT
            }
        }
    }
}
