import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles

Item {
    id: detailDisplay
    property string shutdownFlowTask: "BSPD"
    property int status: 0
    property bool highlight: false
    onHighlightChanged: {
        if (!highlight) {
            background.border.width = 1
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        radius: 5
        color: detailDisplay.status == 0 ? "black" : (detailDisplay.status == 1 ? "#14FF00" : (detailDisplay.status == 2 ? "#FF0000" : "transparent"))
        border.color: "white"
        border.width: 1

        Behavior on border.width {
            PropertyAnimation {
                easing.type: Easing.InOutQuad // Easing curve for smooth start and end
                duration: 1000 // Animation duration in milliseconds
            }
        }

        Timer {
            interval: 2000 // Change border width every 2 seconds
            running: detailDisplay.highlight
            repeat: true
            onTriggered: {
                background.border.width = (background.border.width
                                           === 1) ? 5 : 1 // Toggle between 0 and 5
            }
        }

        LabelText {
            id: taskText
            color: detailDisplay.status == 0 ? "white" : "black"
            text: detailDisplay.shutdownFlowTask
            anchors.centerIn: parent
            font.bold: true
            font.pixelSize: Math.min(parent.width / 3, parent.height / 3)
        }
    }
}
