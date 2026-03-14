import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NERO

Item {
    id: directionView
    width: 1920
    height: 1080
    property bool forward: true
    property int radius: 50

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 20

        Rectangle {
            id: rectangle
            color: forward ? Theme.getColor("directionBackground") : Theme.getColor("transparent")
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: forward ? Theme.getColor("transparent") : Theme.getColor("primaryForeground")
            border.width: 2
            radius: directionView.radius

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                text: "D"
                color: forward ? Theme.getColor("inverseForeground") : Theme.getColor("primaryForeground")
                font.family: webFont.name
            }
        }

        Rectangle {
            id: rectangle1
            color: forward ? Theme.getColor("transparent") : Theme.getColor("directionBackground")
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: forward ? Theme.getColor("primaryForeground") : Theme.getColor("transparent")
            border.width: 2
            radius: directionView.radius

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                color: forward ? Theme.getColor("primaryForeground") : Theme.getColor("inverseForeground")
                font.family: webFont.name
                text: "R"
            }
        }
    }
}
