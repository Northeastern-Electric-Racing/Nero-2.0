import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: runInfo

    property int dimension: 200
    width: dimension
    height: dimension / 4

    property string backgroundColor: "white"
    property string label: ""
    property string value: ""
    property int topRadius: 0
    property int radius: 0
    property bool isTop: false
    property bool isBottom: false


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
            width: runInfo.width / 4
            height: runInfo.height * 0.8
            color: "black"

            x: parent.width * 0.7
            y: parent.height / 10
        }

        LabelText {
            id: labelText
            text: label
            x: runInfo.width * 0.05
            y: runInfo.height * 0.2
            font.pixelSize: runInfo.dimension * 0.08
            color: "black"
        }

        ValueText {
            id: valueText
            text: value
            x: runInfo.width * 0.78
            y: runInfo.height * 0.08
            font.pixelSize: runInfo.dimension * 0.15
            color: "white"
        }
    }
}
