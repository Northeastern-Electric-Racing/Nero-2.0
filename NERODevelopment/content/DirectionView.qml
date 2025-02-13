import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {
    id: directionView
    property bool forward: true
    property int radius: 10

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 20

        Rectangle {
            id: driveContainer
            color: forward ? "#FF49F8" : "transparent"
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 10
            border.color: "#FF49F8" // Outline color
            border.width: 5 // Outline width
            radius: directionView.radius // Border radius to round the corners

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                text: "D"
                color: forward ? 'black' : "white"
                font.family: webFont.name
            }
        }

        Rectangle {
            id: reverseContainer
            color: forward ? "transparent" : "#FF49F8"
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: "#FF49F8" // Outline color
            border.width: 5 // Outline width
            radius: directionView.radius // Border radius to round the corners

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                color: forward ? "white" : 'black'
                font.family: webFont.name
                text: "R"
            }
        }
    }
}
