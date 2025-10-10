import QtQuick 2.15
import QtQuick.Controls 2.15
import NERO

Rectangle {
    id: root
    width: 132
    height: 40
    radius: 10

    property bool highlighted: false
    property string text: "FLAPPY BIRD"

    gradient: Gradient {
        GradientStop { position: 0.0; color: highlighted ? Theme.getColor("iconBackground") : Theme.getColor("iconGradientStop") }
        GradientStop { position: 1.0; color: highlighted ? Theme.getColor("iconHighlightedBackground") : Theme.getColor("iconHighlightedGradientStop") }
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
