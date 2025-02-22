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
            border.color: forward ? "transparent" : "white"
            border.width: 2
            radius: directionView.radius

            Text {
                anchors.centerIn: parent
                font.pixelSize: Math.min(parent.width, parent.height) / 1.5
                text: "D"
                color: forward ? "black" : 'white'
                font.family: webFont.name
            }
        }

        Rectangle {
            id: rectangle1
            color: forward ? "transparent" : "#55AAFF"
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2 - 20
            border.color: forward ? "white" : "transparent"
            border.width: 2
            radius: directionView.radius

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
