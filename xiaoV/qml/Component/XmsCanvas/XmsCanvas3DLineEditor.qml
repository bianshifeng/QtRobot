import QtQuick 2.2
import QtCanvas3D 1.0
import "./3rd/xmscanvas3dlineeditor.js" as GLCode



Item{
    id: id_root
    implicitHeight: 768
    implicitWidth: 1280
    visible: true
    Canvas3D{
        id: id_canvas
        property color backgroudColor: "#FCFCFC"
        property string snapImage:"qrc:/qml/images/test/snap.png"
        property string snapImage2:""
        property int imageWidth:1479
        property int imageHeight:781
        anchors.fill: parent
        property int index:1
        onInitializeGL: {GLCode.initializeGL(id_canvas,id_control)}
        onSnapImageChanged: {
//            GLCode.changeImageGL()
        }

        onPaintGL: {
            GLCode.paintGL(id_canvas)
        }
        onResizeGL: {GLCode.resizeGL(id_canvas)}

        XmsControlEventSource{
            id: id_control
            focus: true
            enabled: true
            anchors.fill: parent
        }

    }


    function __addItem(t_type,t_pointList){
        GLCode.addItem(t_type,t_pointList)
    }

    function __getDrawItemsData(){
        return GLCode.getDrawItemsData()
    }

    function __deleteItem(){
        var t_type = GLCode.deleteItem();
        return t_type;
    }

    function __clearItems(){
        GLCode.clearItems()
    }

    function __reset(){
        GLCode.resetGL()
        GLCode.resizeGL(id_canvas)
    }


    function __changeImage(imgUrl,imgWidth,imgHeight){
        id_canvas.imageWidth = imgWidth
        id_canvas.imageHeight = imgHeight
        GLCode.changeImageGL(imgUrl)
        GLCode.resizeGL(id_canvas)
    }

}
