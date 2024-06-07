import QtQuick 2.0

Item {

    id: arrow

    property var origin: null
    property var end: null
    property int duration: 2000
    property color color: "red"

    // our bounding box must include (origin) and (end) and we
    // add an (arbitrary) 50px margin
    x: Math.min(origin.x, end.x) - 50
    y: Math.min(origin.y, end.y) - 50
    width: Math.abs(end.x - origin.x) + 100
    height: Math.abs(end.y - origin.y) + 100

    function start() {
        pathAnimation.start()
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        antialiasing: true

        property var path: []
        property double angle: 0

        property int arrowHeadLength: 40 //px

        onPaint: {

            if (path.length < 2) {
                return
            }

            var i = 0
            var ctx = canvas.getContext('2d')

            ctx.reset()

            ctx.lineJoin = "round"
            ctx.lineCap = "round"

            ctx.lineWidth = 10

            ctx.strokeStyle = arrow.color
            ctx.fillStyle = arrow.color

            var p1 = path[0]
            var p2 = path[1]

            ctx.beginPath()
            ctx.moveTo(p1.x, p1.y)

            for (i = 1; i < path.length - 1; i++) {
                // we pick the point between pi+1 & pi+2 as the
                // end point and p1 as our control point
                var midPoint = midPointBtw(p1, p2)
                ctx.quadraticCurveTo(p1.x, p1.y, midPoint.x, midPoint.y)
                p1 = path[i]
                p2 = path[i + 1]

                if (Math.pow(p2.x - path[path.length - 1].x,
                             2) + Math.pow(p2.y - path[path.length - 1].y,
                                           2) < Math.pow(arrowHeadLength, 2)) {
                    break
                }
            }
            ctx.lineTo(p2.x, p2.y)

            ctx.stroke()

            ctx.beginPath()
            ctx.translate(p2.x, p2.y)
            ctx.rotate(angle * Math.PI / 180)
            ctx.lineTo(0, 20)
            ctx.lineTo(arrowHeadLength, 0)
            ctx.lineTo(0, -20)
            ctx.closePath()
            ctx.fill()
        }

        function midPointBtw(p1, p2) {
            return {
                "x": p1.x + (p2.x - p1.x) / 2,
                "y": p1.y + (p2.y - p1.y) / 2
            }
        }

        function addPoint(x, y, pathangle) {
            path.push(Qt.point(x - arrow.x, y - arrow.y))
            angle = pathangle
            canvas.requestPaint()
        }
    }

    PathInterpolator {
        id: pathInterpolate

        path: Path {
            startX: arrow.origin.x
            startY: arrow.origin.y

            PathCubic {
                x: arrow.end.x
                y: arrow.end.y

                control1X: x
                control1Y: 10
                control2X: 10
                control2Y: y
            }
        }
        NumberAnimation on progress {
            id: pathAnimation
            running: false
            from: 0
            to: 1
            duration: arrow.duration
        }

        onProgressChanged: {
            canvas.addPoint(pathInterpolate.x, pathInterpolate.y,
                            pathInterpolate.angle)
        }
    }
}
