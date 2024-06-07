import QtQuick 2.15
import QtQuick.Shapes

Item {
    id: critical
    property int dimension: 20
    property int numWarnings: 1

    width: dimension * 1.2
    height: dimension * 1.2

    Shape {
        id: shape
        width: critical.width
        height: critical.height
        anchors.centerIn: parent

        ShapePath {
            id: triangle
            dashPattern: 5.6
            fillColor: "red"
            strokeColor: "transparent"
            strokeWidth: 1
            capStyle: ShapePath.RoundCap

            startX: (shape.width) / 2
            startY: 0

            PathLine {
                x: 0
                y: (shape.height)
            }
            PathLine {
                x: (shape.width)
                y: (shape.height)
            }
            PathLine {
                x: (shape.width) / 2
                y: 0
            }
        }

        Text {
            id: text1
            x: (shape.width) / 2 - font.pixelSize / 8
            y: (shape.height - (font.pixelSize * 1.2))
            color: "white"
            text: qsTr("!")
            font.pixelSize: .75 * shape.width
        }

        Rectangle {
            id: faultCircle
            anchors.right: parent.right
            anchors.rightMargin: -dimension / 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -dimension / 10
            width: (critical.dimension / 2)
            height: (critical.dimension / 2)
            radius: 100

            visible: numWarnings > 0

            Text {
                id: number
                text: numWarnings
                font.pixelSize: 0.75 * parent.width
                font.weight: Font.Bold
                font.bold: false
                anchors.centerIn: parent
            }
        }
    }
}
