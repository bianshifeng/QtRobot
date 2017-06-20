import QtQuick 2.0
import QtCanvas3D 1.0
import "./3rd/xmscloudcanvas.js" as GLCode

Item {
    id: id_root
    implicitHeight: 500
    implicitWidth: 700
    Timer{
        id: id_timer
        interval: 100
        repeat: true
        running: true
        onTriggered: {
            id_canvas.update()
            console.log("sssssssssssssss");
        }

    }

    Canvas3D {
        id: id_canvas
        anchors.fill: parent
        focus: true

        onInitializeGL: {
            GLCode.initializeGL(id_canvas);
        }

        onPaintGL: {
            GLCode.paintGL(id_canvas);
        }

        onResizeGL: {
            GLCode.resizeGL(id_canvas);
        }
    }

//    Component.onCompleted: id_timer.restart()
}

