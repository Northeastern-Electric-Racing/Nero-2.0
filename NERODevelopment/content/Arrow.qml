import QtQuick 2.0
import QtQuick.Shapes
import QtQuick

Item {
    id: arrow
    property variant stops: []
    property int arrowWidth: 2
    property real angle: -Math.PI / 6
    property string arrowColor: "white"

    property real x1: stops[stops.length - 2].x
    property real y1: stops[stops.length - 2].y
    property real x2: stops[stops.length - 1].x
    property real y2: stops[stops.length - 1].y

    property real yDiff: y1 - y2
    property real xDiff: x1 - x2

    property real l1: Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2))
    property real l2: 15

    property real x3: x2 + l2 / l1 * (xDiff * Math.cos(
                                          angle) + yDiff * Math.sin(angle))
    property real y3: y2 + l2 / l1 * (yDiff * Math.cos(
                                          angle) - xDiff * Math.sin(angle))

    property real x4: x2 + l2 / l1 * (xDiff * Math.cos(
                                          angle) - yDiff * Math.sin(angle))
    property real y4: y2 + l2 / l1 * (yDiff * Math.cos(
                                          angle) + xDiff * Math.sin(angle))

    // Explanation of math https://math.stackexchange.com/questions/1314006/drawing-an-arrow
    Repeater {
        model: stops.length - 1
        Shape {
            ShapePath {
                strokeColor: arrowColor
                strokeWidth: arrowWidth
                startX: stops[index].x
                startY: stops[index].y
                PathLine {
                    x: stops[index + 1].x
                    y: stops[index + 1].y
                }
            }
        }
    }

    Shape {
        ShapePath {
            strokeColor: arrowColor
            strokeWidth: arrowWidth
            fillColor: arrowColor
            startX: stops[stops.length - 1].x
            startY: stops[stops.length - 1].y
            capStyle: ShapePath.RoundCap

            PathLine {
                x: x3
                y: y3
            }

            PathLine {
                x: x4
                y: y4
            }

            PathLine {
                x: stops[stops.length - 1].x
                y: stops[stops.length - 1].y
            }
        }
    }
}
