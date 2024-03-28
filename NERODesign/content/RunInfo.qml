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

            ValueText {
                id: valueText
                text: value
                font.pixelSize: rightBlackRectangle.height * 0.8
                x: rightBlackRectangle.width * 0.5 - rightBlackRectangle.height / 4.5
                y: rightBlackRectangle.height * -0.1
                color: "white"
                font.bold: true
            }
        }

        LabelText {
            id: labelText
            text: label
            x: runInfo.width * 0.05
            y: runInfo.isTop ? runInfo.height * 0.6 - font.pixelSize + topRect.height * 0.1
                             : (runInfo.isBottom ? runInfo.height * 0.6
                                                   - font.pixelSize + bottomRect.height * 0.15
                                                 : runInfo.height * 0.6 - font.pixelSize )
            font.pixelSize: Math.min(runInfo.height * 0.6, runInfo.width * 0.08)
            color: "black"
            font.bold: true
        }
    }
}
