import QtQuick 2.15
import QtQuick.Shapes 1.15

Item {
    id: critical
    property int dimension: 200
    property int numWarnings: 3

    width: dimension * 1.2
    height: dimension * 1.2

    Shape {
        id: shape
        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        ShapePath {
            id: triangle
            dashPattern: 5.6
            simplify: true
            fillColor: "red"
            strokeColor: "transparent"
            strokeWidth: 1
            capStyle: ShapePath.RoundCap

            startX: 200/2
            startY: 0

            PathLine { x: 0; y: 200 }
            PathLine { x: 200; y: 200 }
            PathLine { x: 200/2; y: 0 }

        }

        Text {
            id: text1
            x: 200/2 -20
            y: 25
            color: "white"
            text: qsTr("!")
            font.pixelSize: 150
        }

        Rectangle {
            id: faultCircle
            x: 115
            y: 128
            width: 100
            height: 100
            radius: 100

            Text {
                id: faultNum
                anchors.centerIn: parent
                color: "black"
                text: numWarnings
                font.pixelSize: 0.75 * parent.width
            }

        }



    }

}
