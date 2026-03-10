import QtQuick
import QtQuick.Controls
import NERO

Rectangle {
    id: root
    width: 132
    height: 40
    radius: 10

    property bool highlighted: false
    property string text: "FLAPPY BIRD"

    gradient: Gradient {
        GradientStop { position: 0.0; color: highlighted ? Theme.iconHighlightedBackground : Theme.iconBackground }
        GradientStop { position: 1.0; color: highlighted ? Theme.iconHighlightedGradientStop : Theme.iconGradientStop }
    }

    LabelText {
        id: label
        text: root.text
        font.pixelSize: 15
        width: root.width
        height: root.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.centerIn: parent
    }
}
