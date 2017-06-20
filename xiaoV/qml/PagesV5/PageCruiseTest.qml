import QtQuick 2.0

PageBase {
    Rectangle {
        id: rectangle1
        x: 158
        y: 106
        width: 474
        height: 356
        color: "#ffffff"
        border.width: 2

        Rectangle {
            id: rectangle2
            x: -116
            y: 171
            width: 99
            height: 29
            color: "#d12929"

            TextInput {
                id: textInput1
                x: 0
                y: 0
                width: 99
                height: 29
                text: qsTr("Text Input")
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: rectangle3
            x: 188
            y: 367
            width: 99
            height: 29
            color: "#d12929"
            TextInput {
                id: textInput2
                x: 0
                y: 0
                width: 99
                height: 29
                text: qsTr("Text Input")
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: rectangle4
            x: 480
            y: 156
            width: 99
            height: 29
            color: "#d12929"
            TextInput {
                id: textInput3
                x: 0
                y: 0
                width: 99
                height: 29
                text: qsTr("Text Input")
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: rectangle5
            x: 194
            y: -38
            width: 99
            height: 29
            color: "#d12929"
            TextInput {
                id: textInput4
                x: 0
                y: 0
                width: 99
                height: 29
                text: qsTr("Text Input")
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: rectangle6
            x: 402
            y: 18
            width: 48
            height: 29
            color: "#ed9c20"
            TextInput {
                id: textInput5
                x: 0
                y: 0
                width: 55
                height: 29
                text: qsTr("Text Input")
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: rectangle7
            x: 24
            y: 18
            width: 48
            height: 29
            color: "#ed9c20"
            TextInput {
                id: textInput6
                x: 0
                y: 0
                width: 55
                height: 29
                text: qsTr("Text Input")
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: rectangle8
            x: 402
            y: 308
            width: 48
            height: 29
            color: "#ed9c20"
            TextInput {
                id: textInput7
                x: 0
                y: 0
                width: 55
                height: 29
                text: qsTr("Text Input")
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: rectangle9
            x: 24
            y: 308
            width: 48
            height: 29
            color: "#ed9c20"
            TextInput {
                id: textInput8
                x: 0
                y: 0
                width: 55
                height: 29
                text: qsTr("Text Input")
                font.pixelSize: 12
            }
        }
    }

}

