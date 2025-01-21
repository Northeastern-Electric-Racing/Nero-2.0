import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: runInfo

    property int widthValue: 200
    property int heightValue: 40

    width: widthValue
    height: heightValue

    property string backgroundColor: "white"
    property string label: ""
    property int value: 0
    property int topRadius: 0
    property int radius: 0
    property bool isTop: false
    property bool isBottom: false

    function formatTime(milliseconds) {
        var minutes = Math.floor(milliseconds / 60000);
        var seconds = Math.floor((milliseconds % 60000) / 1000);
        var ms = milliseconds % 1000;

        var formattedMinutes = (minutes < 10 ? "0" + minutes : "" + minutes).split('').join(' ');
        var formattedSeconds = (seconds < 10 ? "0" + seconds : "" + seconds).split('').join(' ');
        var formattedMs = (ms < 100 ? (ms < 10 ? "00" + ms : "0" + ms) : "" + ms).split('').join(' ');

        return formattedMinutes + " : " + formattedSeconds + " : " + formattedMs;
    }


    Rectangle {
        id: bottomRect
        visible: runInfo.isTop
        width: runInfo.width
        height: runInfo.radius
        color: runInfo.backgroundColor

        anchors {
            left: runInfo.left
            bottom: runInfo.bottom
        }
    }

    Rectangle {
        id: topRect
        visible: runInfo.isBottom
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
                text: formatTime(value)
                font.pixelSize: rightBlackRectangle.height * 0.9
                anchors.centerIn: parent
                color: "white"
                font.bold: true
            }
        }

        LabelText {
            id: labelText
            text: label
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
                rightMargin: runInfo.width
            }
            font.pixelSize: Math.min(runInfo.height * 0.6, runInfo.width * 0.08)
            color: "black"
            font.bold: true
        }
    }
}
