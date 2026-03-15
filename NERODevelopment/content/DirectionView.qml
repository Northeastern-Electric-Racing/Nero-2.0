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
            color: forward ? Theme.directionBackground : "transparent"
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: forward ? "transparent" : Theme.primaryForeground
            border.width: 2
            radius: directionView.radius

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                text: "D"
                color: forward ? Theme.inverseForeground : Theme.primaryForeground
                font.family: webFont.name
            }
        }

        Rectangle {
            id: rectangle1
            color: forward ? "transparent" : Theme.directionBackground
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: forward ? Theme.primaryForeground : "transparent"
            border.width: 2
            radius: directionView.radius

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                color: forward ? Theme.primaryForeground : Theme.inverseForeground
                font.family: webFont.name
                text: "R"
            }
        }
    }
}
