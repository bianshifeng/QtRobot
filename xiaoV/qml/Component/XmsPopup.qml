import QtQuick 2.0

Item{
    id: id_root
    anchors.fill: parent
    anchors.margins:  -1
    state:"hidden"
    visible: false

    property bool dim: true
    property bool active:(state == "visible")

    signal opening
    signal closing
    signal backgroundClicked

    MouseArea{
        id: id_mouseArea
        anchors.fill: parent
        hoverEnabled: true
        enabled: (id_root.state == "visible")
        onClicked: {
            //Eats the event, and up to desendants to decide whether to close the dialog
            mouse.accepted = true
            backgroundClicked()
        }

    }

    Rectangle{
        id: id_background
        anchors.fill: parent
        color:id_root.dim ? "#55000000": "transparent"
        opacity:0
    }

    function open(){
        if(state == "hidden"){
            state = "visible"
            opening()
        }

    }


    function close(){
        if(state == "visible"){
            state = "hidden"
            closing()
        }
    }



    states: [
        State{
            name:"visible"
            PropertyChanges {
                target: id_background
                opacity:1
            }
        },
        State {
            name: "hidden"
            PropertyChanges {
                target: id_background
                opacity: 0

            }
        }

    ]

    transitions: [
        Transition {
            from: "hidden"
            to: "visible"
            SequentialAnimation{
                PropertyAction{target:id_root; property: "visible"; value: true}
                NumberAnimation{
                    target: id_background
                    property: "opacity"
                    duration: 200
                    easing.type: Easing.Bezier
                    easing.bezierCurve: [0.4,0,0.2,1,1,1]
                }

            }
        },
        Transition {
            from: "visible"
            to: "hidden"
            SequentialAnimation{

                NumberAnimation {
                    target: id_background
                    property: "opacity"
                    duration: 200
                    easing.type: Easing.Bezier
                    easing.bezierCurve: [0.4,0,0.2,1,1,1]
                }
                PropertyAction{target: id_root; property:"visible"; value: false}
            }

        }

    ]

}
