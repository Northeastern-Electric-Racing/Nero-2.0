import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles

Rectangle {
    id: detailDisplay
    property string shutdownFlowTask: "Task"
    property string statusDisplay: "CLEAR"
    property int status: 0
    property bool highlight: false
    height: 60
    width: 250
    color: "black"

    onStatusChanged: {
        console.log("Changing status")
        if (status === 0) {
            statusDisplay = "OFF"
        } else if (status == 1) {
            statusDisplay = "GOOD"
        } else {
            statusDisplay = "FAULTED"
        }
    }

    Rectangle {
        id: taskDisplay

        Rectangle {
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
                        ColorAnimation {
                            from: "black"
                            to: "#969696"
                            duration: 1500
                        }
                        ColorAnimation {
                            from: "#969696"
                            to: "black"
                            duration: 3000
                        }
                    }
                }

                GradientStop {
                    position: 1.0
                    SequentialAnimation on color {
                        loops: Animation.Infinite
                        ColorAnimation {
                            from: "black"
                            to: "black"
                            duration: 1500
                        }
                        ColorAnimation {
                            from: "black"
                            to: "#969696"
                            duration: 1500
                        }
                        ColorAnimation {
                            from: "#969696"
                            to: "black"
                            duration: 1500
                        }
                    }
                }
            }
        }
        color: "transparent"

        border.color: "transparent"
        width: Math.max(parent.width * 0.6,
                        taskText.implicitWidth) // Ensure minimum width
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
        id: statusDisplayContainer
        color: status == 0 ? "transparent" : (status === 1 ? "#14FF00" : (status === 2 ? "#FF0000" : "transparent"))
        width: Math.max(parent.width * 0.4,
                        statusText.implicitWidth + 20) // Ensure minimum width
        height: parent.height
        anchors.left: taskDisplay.right
        radius: 10
        border.color: status == 0 ? "white" : "transparent"
        border.width: status == 0 ? 3 : 0

        LabelText {
            id: statusText
            visible: !clear
            color: "black"
            text: detailDisplay.statusDisplay
            anchors.centerIn: parent
        }
    }
}
