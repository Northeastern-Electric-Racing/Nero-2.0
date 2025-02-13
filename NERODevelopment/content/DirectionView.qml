import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

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
            color: forward ? "#55AAFF" : "transparent"
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: forward ? "transparent" : "white"  // Outline color
            border.width: 2      // Outline width
            radius: directionView.radius          // Border radius to round the corners


            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                text: "D"
                color: "black"
                font.family: webFont.name
            }
        }

        Rectangle {
            id: rectangle1
            color: forward ? "transparent" : "Red"
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: forward ? "#55AAFF" : "transparent"  // Outline color
            border.width: 2      // Outline width
            radius: directionView.radius          // Border radius to round the corners

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                color: "white"
                font.family: webFont.name
                text: "R"
            }
        }
    }
}
