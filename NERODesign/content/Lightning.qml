import QtQuick 2.15
import QtQuick.Shapes

Item {
    id: lightning
    property int dimension: 150
    property string primaryColor: "#14f504"
    property string secondaryColor: "black"

    width: dimension
    height: dimension

    Shape {
        id: shape2
        width: lightning.width + dimension / 5
        height: lightning.height + dimension / 5
        anchors.centerIn: parent

        ShapePath {
            id: background
            fillColor: lightning.secondaryColor
            strokeColor: "transparent"
            strokeWidth: 1

            startX: 47 / 80 * shape2.width
            startY: 32 / 80 * shape2.width

            PathLine { x: 60 / 80 * shape2.width; y: 32 / 80 * shape2.width }

            // rounded right corner
            PathQuad {
                x: 62 / 80 * shape2.width
                y: 37.75 / 80 * shape2.width
                controlX: 65 / 80 * shape2.width
                controlY: 33.75 / 80 * shape2.width
            }

            PathLine { x: 38 / 80 * shape2.width; y: 69.75 / 80 * shape2.width }

            // rounded bottom corner
            PathQuad {
                x: 33 / 80 * shape2.width
                y: 68.75 / 80 * shape2.width
                controlX: 35 / 80 * shape2.width
                controlY: 73.75 / 80 * shape2.width
            }

            PathLine { x: 33 / 80 * shape2.width; y: 48 / 80 * shape2.width }
            PathLine { x: 20 / 80 * shape2.width; y: 48 / 80 * shape2.width }

            // rounded left corner
            PathQuad {
                x: 18 / 80 * shape2.width
                y: 42.25 / 80 * shape2.width
                controlX: 15 / 80 * shape2.width
                controlY: 46.25 / 80 * shape2.width
            }

            PathLine { x: 42 / 80 * shape2.width; y: 10.25 / 80 * shape2.width }

            // rounded top corner
            PathQuad {
                x: 47 / 80 * shape2.width
                y: 11.25 / 80 * shape2.width
                controlX: 45 / 80 * shape2.width
                controlY: 6.25 / 80 * shape2.width
            }
        }
    }

    Shape {
        id: shape
        width: lightning.width
        height: lightning.height
        anchors.centerIn: parent

        ShapePath {
            id: foreground
            fillColor: lightning.primaryColor
            strokeColor: "transparent"
            strokeWidth: 1

            startX: 45 / 80 * shape.width
            startY: 33.75 / 80 * shape.width

            PathLine { x: 60 / 80 * shape.width; y: 33.75 / 80 * shape.width }

            // rounded right corner
            PathQuad {
                x: 62 / 80 * shape.width
                y: 37.75 / 80 * shape.width
                controlX: 65 / 80 * shape.width
                controlY: 33.75 / 80 * shape.width
            }

            PathLine { x: 38 / 80 * shape.width; y: 69.75 / 80 * shape.width }

            // rounded bottom corner
            PathQuad {
                x: 35 / 80 * shape.width
                y: 68.75 / 80 * shape.width
                controlX: 35 / 80 * shape.width
                controlY: 73.75 / 80 * shape.width
            }

            PathLine { x: 35 / 80 * shape.width; y: 46.25 / 80 * shape.width }
            PathLine { x: 20 / 80 * shape.width; y: 46.25 / 80 * shape.width }

            // rounded left corner
            PathQuad {
                x: 18 / 80 * shape.width
                y: 42.25 / 80 * shape.width
                controlX: 15 / 80 * shape.width
                controlY: 46.25 / 80 * shape.width
            }

            PathLine { x: 42 / 80 * shape.width; y: 10.25 / 80 * shape.width }

            // rounded top corner
            PathQuad {
                x: 45 / 80 * shape.width
                y: 11.25 / 80 * shape.width
                controlX: 45 / 80 * shape.width
                controlY: 6.25 / 80 * shape.width
            }
        }
    }
}
