import QtQuick 2.0
import QtGraphicalEffects 1.0

Item{
    id: id_root
    property var source
    property int depth:0
    anchors.fill: parent
    state: "z-" + Math.max(0,Math.min(3,depth))
    DropShadow{
        id: id_topShadow
        anchors.fill: parent
        source: id_root.source
        color:"#00000000"
        spread: 0.0
        samples: 32
        transparentBorder: true
        fast: true
    }

    DropShadow{
        id: id_bottomShadow
        anchors.fill: parent
        source: id_root.source
        color:"#00000000"
        spread:0.0
        samples:32
        transparentBorder: true
        fast:true
    }

    states:[
        State{
            name:"z-0"
            PropertyChanges {
                target: id_topShadow
                verticalOffset:0
                radius:0
                color:"#00000000"
            }
            PropertyChanges {
                target: id_bottomShadow
                verticalOffset:0
                radius:0
                color:"#00000000"

            }
        },
        State{
            name:"z-1"
            PropertyChanges {
                target: id_topShadow
                verticalOffset: 1*g_dp
                radius: 1.5 * g_dp
                color:"#1e000000"
            }
            PropertyChanges {
                target: id_bottomShadow
                verticalOffset: 1*g_dp
                radius:1*g_dp
                color:"#3d000000"
            }
        },
        State{
            name:"z-2"
            PropertyChanges {
                target: id_topShadow
                verticalOffset: 3*g_dp
                radius: 3 * g_dp
                color:"#28000000"
            }
            PropertyChanges {
                target: id_bottomShadow
                verticalOffset: 3*g_dp
                radius:3*g_dp
                color:"#3a000000"
            }
        },

        State{
            name:"z-3"
            PropertyChanges {
                target: id_topShadow
                verticalOffset: 10*g_dp
                radius: 10 * g_dp
                color:"#30000000"
            }
            PropertyChanges {
                target: id_bottomShadow
                verticalOffset: 6*g_dp
                radius:3*g_dp
                color:"#3a000000"
            }
        }

    ]

    transitions: [
        Transition {
            NumberAnimation{
                targets: [id_topShadow,id_bottomShadow]
                properties: "verticalOffset, radius"
                duration: 280
                easing.type: Easing.Bezier
                easing.bezierCurve: [0.4,0,0.2,1,1,1]
            }

            ColorAnimation {
                targets: [id_topShadow,id_bottomShadow]
                duration: 200
            }
        }
    ]
}
