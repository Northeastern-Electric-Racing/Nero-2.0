import QtQuick 2.15
import QtQuick.Controls 2.15


Item {
    id: runInfo
        width: 200
        height: 50

    property string backgroundColor: "white"
    property string label: ""
    property string value: ""


    Rectangle {
        height: parent.height
        width: parent.width
        color: backgroundColor

        anchors {
            left: parent.left
            top: parent.top
        }

        Rectangle {
            id: rightBlackRectangle
            width: parent.width / 4
            height: parent.height * 0.8
            color: "black"

            x: parent.width * 0.7
            y: parent.height / 10
        }

        LabelText {
            id: labelText
            text: label
            x: 10
            y: 10
            font.pixelSize: parent.width * 0.1
            color: "black"
        }

        ValueText {
            id: valueText
            text: value
            x: parent.width * 0.8
            y: parent.height * 0.15
            font.pixelSize: parent.width * 0.1
            color: "white"
        }

    }
}
