import QtQuick 2.15

Rectangle {
    property bool highlighted: false
    property string text: "Off"
    color: 'transparent'
    border.width: highlighted ? 2 : 1

    border.color: highlighted ? "white" : "transparent"
    height: label.implicitHeight
    width: parent.width

    LabelText {
        id: label
        padding: 8
        font.pixelSize: 12
        text: parent.text
    }
}
