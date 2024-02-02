import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles

Rectangle {
    id: detailDisplay
    property string shutdownFlowTask: "Task"
    property string status: "CLEAR"
    property bool ok: true
    property bool clear: false
    property bool highlight: true
    height: 50
    width: 200
    color: "black"

    Row {
        id: row
        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        Rectangle {
            id: taskDisplay
            Rectangle{
                id: gradientBackground
                rotation: -90
                visible: highlight
                anchors.centerIn: parent
                width: parent.height
                height: parent.width
                radius: 10
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        SequentialAnimation on color {
                            loops: Animation.Infinite
                            ColorAnimation { from: "black"; to: "#969696"; duration: 1500 }
                            ColorAnimation { from: "#969696"; to: "black"; duration: 3000 }
                        }
                    }


                    GradientStop {
                        position: 1.0
                        SequentialAnimation on color {
                            loops: Animation.Infinite
                            ColorAnimation { from: "black"; to: "black"; duration: 1500 }
                            ColorAnimation { from: "black"; to: "#969696"; duration: 1500 }
                            ColorAnimation { from: "#969696"; to: "black"; duration: 1500 }
                        }
                    }
                }
            }

            border.color: "transparent"
            width: parent.width * 0.5
            height: parent.height
            radius: 10

            LabelText {
                id: taskText
                color: "white"
                text: detailDisplay.shutdownFlowTask
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                font.bold: false
                leftPadding: 10
            }
        }

        Rectangle {
            id: statusDisplay
            color: clear ? "transparent" : (ok === true ? "#14FF00" : (ok === false ? "#FF0000" : "transparent"))
            width: Math.max(parent.width * 0.5, statusText.implicitWidth + 20) // Ensure minimum width
            height: parent.height
            radius: 10
            border.color: clear ? "white" : "transparent"
            border.width: clear ? 3 : 0

            LabelText {
                id: statusText
                visible: !clear
                color: "black"
                text: detailDisplay.status
                anchors.centerIn: parent
            }
        }
    }
}
