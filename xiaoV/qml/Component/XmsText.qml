import QtQuick 2.0
import "./UIConstants.js" as UI

Text{
    id: id_root
    property int size:UI.font_size_h4
    TextFontLoader{
        id: id_font
    }
    font.family: id_font.name
    font.pixelSize: size
    smooth: true
    antialiasing: true
    color:UI.COLOR_BASE_BLACK_DARK

}
