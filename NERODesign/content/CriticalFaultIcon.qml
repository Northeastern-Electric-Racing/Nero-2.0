import QtQuick 2.15
import QtQuick.Shapes 1.15

Item {
    id: critical
    property int dimension: 200
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
            simplify: true
            fillColor: "red"
            strokeColor: "transparent"
            strokeWidth: 1
            capStyle: ShapePath.RoundCap

            startX: (shape.width)/2
            startY: 0

            PathLine { x: 0; y: (shape.height) }
            PathLine { x: (shape.width); y: (shape.height) }
            PathLine { x: (shape.width)/2; y: 0 }

        }

        Text {
            id: text1
            x: (shape.width)/2 -20
            y: 25
            color: "white"
            text: qsTr("!")
            font.pixelSize: .75 * shape.width
        }

        Rectangle {
            id: faultCircle
            x: 131
            y: 154
            width: (critical.dimension/2) * 1.2
            height: (critical.dimension/2) * 1.2
            radius: 100

            Text {
                id: faultNum
                anchors.centerIn: parent
                color: "black"
                text: numWarnings
                font.pixelSize: 0.75 * faultCircle.width
            }

        }



    }

}
