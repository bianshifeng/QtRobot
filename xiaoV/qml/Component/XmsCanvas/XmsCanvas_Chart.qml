import QtQuick 2.0
import QtCanvas3D 1.0
import "./3rd/XmsCanvas_Chart.js" as GLCode

Item {
    id: id_root
    implicitHeight: 500
    implicitWidth: 700
    Timer{
        id: id_timer
        interval: 100
        repeat: true
        running: false
        onTriggered: {}

    }

    Canvas3D {
        id: id_canvas
        anchors.fill: parent

        focus: true

        onInitializeGL: {
            GLCode.initializeGL(id_canvas,id_domElement);
        }

        onPaintGL: {
            GLCode.paintGL(id_canvas);
        }

        onResizeGL: {
            GLCode.resizeGL(id_canvas);
        }

        XmsControlEventSource{
            id: id_domElement
            focus: true
            enabled: true
            anchors.fill: parent
        }
    }

//    Component.onCompleted: id_timer.restart()
}

