import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: runInfo

    property string backgroundColor: "white"
    property string label: ""
    property int value: 0
    property int topRadius: 0
    property int radius: 0
    property bool isRight: false
    property bool isLeft: false
    property bool isTop: false
    property bool isBottom: false
    property bool vertical: false

    function formatTime(milliseconds) {
        var minutes = Math.floor(milliseconds / 60000)
        var seconds = Math.floor((milliseconds % 60000) / 1000)
        var ms = Math.trunc(milliseconds % 1000 / 10)

        var formattedMinutes = minutes > 0 ? minutes + ":" : ""
        var formattedSeconds = seconds + ":"
        var formattedMs = ms + ""

        return formattedMinutes + formattedSeconds + formattedMs
    }

    Rectangle {
        id: rightRect
        visible: !runInfo.vertical && runInfo.isRight
        width: runInfo.radius
        height: runInfo.height
        color: runInfo.backgroundColor

        anchors {
            left: runInfo.left
            bottom: runInfo.bottom
        }
    }

    Rectangle {
        id: leftRect
        visible: !runInfo.vertical && runInfo.isLeft
        width: runInfo.radius
        height: runInfo.height
        color: runInfo.backgroundColor

        anchors {
            right: runInfo.right
            top: runInfo.top
        }
    }

    Rectangle {
        id: topRect
        visible: runInfo.vertical && runInfo.isTop
        width: runInfo.width
        height: runInfo.radius
        color: runInfo.backgroundColor

        anchors {
            left: runInfo.left
            bottom: runInfo.bottom
        }
    }

    Rectangle {
        id: bottom
        visible: runInfo.vertical && runInfo.isBottom
        width: runInfo.width
        height: runInfo.radius
        color: runInfo.backgroundColor

        anchors {
            left: runInfo.left
            top: runInfo.top
        }
    }

    Rectangle {
        height: runInfo.height
        width: runInfo.width
        color: backgroundColor
        radius: runInfo.radius

        anchors {
            left: runInfo.left
            top: runInfo.top
        }

        Rectangle {
            id: rightBlackRectangle
            width: runInfo.width / 2.3
            height: runInfo.height * 0.8
            color: "black"
            x: runInfo.width / 1.85

            anchors {
                verticalCenter: parent.verticalCenter
            }

            ValueText {
                id: valueText
                text: formatTime(runInfo.value)
                font.pixelSize: rightBlackRectangle.height * 0.9
                anchors.centerIn: parent
                color: "white"
                font.bold: true
                font.letterSpacing: 0
            }
        }

        LabelText {
            id: labelText
            text: label
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: parent.width / 40
            }
            font.pixelSize: Math.min(runInfo.height * 0.45)
            color: "black"
            font.bold: true
        }
    }
}
