import QtQuick 2.11
import QtQuick.Shapes 1.15

Item {
    id: gauge
    property double value: 0
    property double minValue: 0
    property double maxValue: 100
    property int verticalPadding: 100
    property int horizontalPadding: 20
    property int innerStrokeWidth: 10
    property int outerStrokeWidth: 20
    property int mainTextTopPadding: 0
    width: 300
    height: width / 2 + verticalPadding

    Shape {
        id: shape
        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        ShapePath {
            id: innerArc
            fillColor: "transparent"
            strokeColor: "white"
            strokeWidth: gauge.innerStrokeWidth

            startX: gauge.horizontalPadding
            startY: shape.height - gauge.verticalPadding
            PathArc {
                relativeX: shape.width - (gauge.horizontalPadding * 2)
                y: shape.height - gauge.verticalPadding
                radiusX: 25
                radiusY: 25
            }
        }


        Text {
            id: valueText
            text: Math.round(gauge.value)
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: gauge.width / 4
            font.family: webFont.name
            color: "white"

            anchors.bottom: shape.bottom
            anchors.bottomMargin: gauge.verticalPadding / (3/2)
        }

    }

    Behavior on value {

        NumberAnimation {
            duration: 100
        }
    }

    onValueChanged: ring.requestPaint()

    Canvas {
        id: ring
        x: shape.x
        y: shape.y
        width: parent.width
        height: parent.height
        antialiasing: true

        property real centerX: ring.width / 2
        property real centerY: ring.height - gauge.verticalPadding + (gauge.verticalPadding / 10)
        property real borderWidth: gauge.outerStrokeWidth
        property real ringRadius: (ring.width - gauge.horizontalPadding * 2) / 2
        property real pi: Math.PI
        property double startAngle: pi
        property double endAngle: 2  * pi
        property double step: gauge.value / (gauge.maxValue - gauge.minValue) * (endAngle - startAngle)

        onPaint: {
            const context = getContext("2d");
            context.reset();

            // Draw the progress ring
            const gradient = context.createLinearGradient(0, 0, ring.width, 0);
            gradient.addColorStop(0, "#14FF00");
            gradient.addColorStop(0.75, "#FFF500");
            context.fillStyle = gradient;
            context.strokeStyle = gradient;

            context.lineWidth = ring.borderWidth;
            context.beginPath();
            context.clearRect(0, 0, ring.width, ring.height);
            context.arc(ring.centerX, ring.centerY, ring.ringRadius,
                        ring.startAngle, ring.startAngle + ring.step, false);
            context.stroke();
        }
    }

    Text {
        id: minValueText
        text: "0"
        color: "white"
        font.family: webFont.name
        font.pointSize: gauge.verticalPadding / 2
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: gauge.horizontalPadding / 2
    }

    Text {
        id: maxValueText
        text: gauge.maxValue
        font.pointSize: gauge.verticalPadding / 2
        color: "white"
        font.family: webFont.name
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: gauge.horizontalPadding / 2
    }
}
