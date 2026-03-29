import QtQuick
import QtQuick.Controls
import NERO

Item {
    property int dimension: 200
    property int numWarnings: 1

    width: dimension * 1.5
    height: dimension * 1.5

    Rectangle {
        id: circle
        radius: dimension / 2
        x: (parent.width - dimension) / 2
        y: (parent.height - dimension) / 2
        width: dimension
        height: dimension
        color: Theme.nonCriticalBackground

        Text {
            id: exclamation
            color: Theme.nonCriticalForeground
            text: qsTr("!")
            font.pixelSize: 0.625 * parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.weight: Font.DemiBold
            anchors.centerIn: parent
        }

        Rectangle {
            id: circle_number
            x: 2 / 3 * dimension
            y: 3 / 5 * dimension
            radius: dimension / 4
            width: dimension / 2
            height: dimension / 2
            color: Theme.nonCriticalForeground
            visible: numWarnings > 0

            Text {
                id: number
                text: numWarnings
                font.pixelSize: 0.75 * parent.width
                font.weight: Font.Bold
                font.bold: false
                anchors.centerIn: parent
            }
        }
    }
}
