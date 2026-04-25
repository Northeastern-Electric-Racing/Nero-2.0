import QtQuick 2.11
import QtQuick.Shapes 1.15

Item {
    id: gauge
    property double value: 0
    property double minValue: 0
    property double maxValue: 100
    property int verticalPadding: 100
    property int horizontalPadding: 20
    property int innerStrokeWidth: 3
    property int outerStrokeWidth: 30
    property int mainTextTopPadding: 0
    width: 300
    height: width / 2 + verticalPadding

    Shape {
        id: shape
        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        Text {
            id: valueText
            text: Math.round(gauge.value)
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: gauge.width / 5
            font.family: webFont.name
            color: "white"

            anchors.bottom: shape.bottom
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 15
            anchors.bottomMargin: gauge.verticalPadding / (3 / 2)
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
        y: shape.y - gauge.verticalPadding + (gauge.verticalPadding / 10)
        width: parent.width
        height: parent.height
        antialiasing: true

        property real centerX: ring.width / 2
        property real centerY: ring.height / 2
        property real borderWidth: gauge.outerStrokeWidth
        property real ringRadius: (ring.width - gauge.horizontalPadding * 2) / 3
        property real pi: Math.PI
        property double startAngle: pi / 2
        property double endAngle: 2 * pi
        property double step: 1 - (gauge.value / (gauge.maxValue - gauge.minValue) * (endAngle - startAngle))

        onPaint: {
            const context = getContext("2d");
            context.reset();

            // Draw the progress ring
            const gradient = context.createLinearGradient(0, 0, ring.width, 0);
            gradient.addColorStop(0.1, "blue");
            gradient.addColorStop(0.5, "black");
            context.fillStyle = gradient;
            context.strokeStyle = gradient;

            context.lineWidth = ring.borderWidth;
            context.beginPath();
            context.clearRect(0, 0, ring.width, ring.height);
            context.arc(ring.centerX, ring.centerY, ring.ringRadius, ring.startAngle, ring.startAngle - ring.step / 10, false);
            context.stroke();

            // Draw the inner border
            const innerBorderRadius = ring.ringRadius - ring.borderWidth / 2;
            context.strokeStyle = "#87CEEB";
            context.lineWidth = gauge.innerStrokeWidth;

            context.beginPath();
            context.arc(ring.centerX, ring.centerY, innerBorderRadius, ring.startAngle, ring.startAngle - ring.step / 10, false);
            context.stroke();
        }
    }

    Text {
        id: mph
        text: "mph"
        color: "gray"
        font.family: webFont.name
        font.pointSize: gauge.verticalPadding / 2
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 70
        anchors.leftMargin: gauge.horizontalPadding / 2
    }
}
