import QtQuick 2.3
import "../UIConstants.js" as UI
import "../"

Column{
    id: id_root
    property real scaleValue:1

    property alias worngMessage: id_info_text.text
    signal emitPasswordChanged(var password)
    signal emitBackCurrentPage()

    spacing: 10


    Column{
        height: childrenRect.height
        width: parent.width
        spacing: 12

        XmsText{
            id: id_info_text
            size:UI.font_size_h3
            text:" "
            color:UI.COLOR_BASE_RED
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextInput{
            id: id_password_text
            width: 200
            anchors.horizontalCenter: parent.horizontalCenter
            readOnly: true
            echoMode: TextInput.Password
            antialiasing: true
            smooth: true
            horizontalAlignment:TextInput.AlignHCenter
            opacity: 0.7
            font.pixelSize: UI.font_size_h2 * scaleValue
            color:UI.COLOR_BASE_GRAY_DARK
            maximumLength:6
            onLengthChanged: {
                if(id_password_text.length < id_password_text.maximumLength){
                    id_info_text.text =  " "
                }
            }

            onTextChanged: {
                emitPasswordChanged(id_password_text.text)
            }

        }
        Rectangle{
            height: 2
            radius: 1
            opacity: 0.5
            width: parent.width - 30
            color:id_password_text.color
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Item{
        width: 1
        height: 10
    }

    Grid{
        id: id_pwd_grid
        rows:5
        rowSpacing: 22 * id_root.scaleValue
        columns: 3
        columnSpacing: 36 * id_root.scaleValue

        Repeater{
            model:NumListModel{}
            NumButton{
                value:model.value
                scaleValue: id_root.scaleValue
                onEmitClicked:{
                    id_password_text.text += value
                }

            }
        }

        NumButton{
            isIconText: true
            iconText: UI.ICON_ACTION_EXIT
            scaleValue: id_root.scaleValue
            backColor: UI.COLOR_BASE_ORANGE_DARK
            foreColor: UI.COLOR_BASE_WHITE
            borderColor: UI.COLOR_BASE_ORANGE_DARK
            onEmitClicked: emitBackCurrentPage()
        }
        NumButton{
            value:0
            scaleValue: id_root.scaleValue
            onEmitClicked:{
                id_password_text.text += value
            }
        }
        NumButton{
            isIconText: true
            scaleValue: id_root.scaleValue
            backColor: UI.COLOR_BASE_ORANGE_DARK
            foreColor: UI.COLOR_BASE_WHITE
            borderColor: UI.COLOR_BASE_ORANGE_DARK
            onEmitClicked: {
                var t_str = id_password_text.text
                t_str=t_str.substring(0,t_str.length - 1)
                id_password_text.text = t_str
            }
        }
    }



}

