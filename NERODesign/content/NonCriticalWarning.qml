import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property int dimension: 200
    property int numWarnings: 1
    property bool hide: false


    width: dimension * 1.5
    height: dimension * 1.5

    visible: !hide

    Rectangle {
        id: circle
        radius: dimension / 2
        x: (parent.width - dimension) / 2
        y: (parent.height - dimension) / 2
        width: dimension
        height: dimension
        color: "#ffca62"

        Text {
            id: exclamation
            color: "#ffffff"
            text: qsTr("!")
            font.pixelSize: 0.625 * parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenterOffset: -4
            anchors.horizontalCenterOffset: 0
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
            color: "#ffffff"

            Text {
                id: number
                text: numWarnings
                font.pixelSize: 0.75 * parent.width
                anchors.verticalCenterOffset: -4
                anchors.horizontalCenterOffset: 0
                font.weight: Font.Bold
                font.bold: false
                anchors.centerIn: parent
            }
        }
    }
}
