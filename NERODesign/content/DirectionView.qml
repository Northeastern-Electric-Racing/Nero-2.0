import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {
    property int width: 1920
    property int height: 1080
    property bool forward: true

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 20
        Rectangle {
            id: rectangle
            color: forward ? "#7CFC00" : "transparent"
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width/2 - 10
            border.color: forward ? "transparent" : "blue"  // Outline color
            border.width: 5      // Outline width
            radius: 50          // Border radius to round the corners


            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height)
                text: "F"
                color: "white"
            }
        }

        Rectangle {
            id: rectangle1
            color: forward ? "transparent" : "Red"
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: forward ? "white" : "transparent"  // Outline color
            border.width: 5      // Outline width
            radius: 50          // Border radius to round the corners

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height)
                color: "white"
                text: "R"
            }
        }
    }
}
