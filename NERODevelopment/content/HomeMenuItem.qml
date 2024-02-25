import QtQuick 2.15

Rectangle {
    property bool highlighted: false
    property string text: "Off"
    color: 'transparent'
    border.width: highlighted ? 2 : 1

    border.color: highlighted ? "lightsteelblue" : "transparent"
    height: label.implicitHeight
    width: parent.width

    LabelText {
        id: label
        padding: 8
        text: parent.text
    }
}
