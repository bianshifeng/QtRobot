import QtQuick 2.0

XmsText{
    id: id_root
    property alias size: id_root.font.pixelSize
    IconFontLoader{
        id: id_fonter
    }
    font.family: id_fonter.name
}
