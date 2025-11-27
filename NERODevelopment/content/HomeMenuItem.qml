import QtQuick 2.15
import NERO

Rectangle {
    property bool highlighted: false
    property string text: "Off"
    color: Theme.getColor("transparent")
    border.width: highlighted ? 2 : 1

    border.color: highlighted ? Theme.getColor("primaryForeground") : Theme.getColor("transparent")
    height: label.implicitHeight
    width: parent.width

    LabelText {
        id: label
        padding: 10
        font.pixelSize: 15
        text: parent.text
    }
}
