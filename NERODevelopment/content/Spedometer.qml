import QtQuick 2.11
import QtQuick.Shapes 1.15

Item {
    id: gauge
    property double value: 0
    property double minValue: 0
    property double maxValue: 100
    property int verticalPadding: 0
    property int horizontalPadding: 20
    property int innerStrokeWidth: 3
    property int outerStrokeWidth: 30
    property int mainTextTopPadding: 0
    property int heightOffset: 100
    width: 300
    height: width / 2 + gauge.heightOffset

    Text {
        id: valueText
        text: Math.round(gauge.value)
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: gauge.width / 5
        font.family: webFont.name
        color: "white"

        anchors.centerIn: ring
        anchors.verticalCenterOffset: -gauge.height / 20
    }

    Behavior on value {

        NumberAnimation {
            duration: 100
        }
    }

    onValueChanged: ring.requestPaint()

    Canvas {
        id: ring
        // x: shape.x
        // y: shape.y - gauge.verticalPadding + (gauge.verticalPadding / 10)
        anchors.bottom: parent.bottom
        anchors.bottomMargin: gauge.verticalPadding
        anchors.top: parent.top
        anchors.topMargin: gauge.verticalPadding
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: gauge.horizontalPadding
        anchors.leftMargin: gauge.horizontalPadding
        antialiasing: true

        property real centerX: ring.width / 2
        property real centerY: ring.height / 2
        property real borderWidth: gauge.outerStrokeWidth
        property real ringRadius: (ring.width - gauge.horizontalPadding * 2) / 3
        property real pi: Math.PI
        property double startAngle: pi / 2
        property double endAngle: (5 * pi) / 2
        property double step: ((gauge.value - gauge.minValue)
                               / (gauge.maxValue - gauge.minValue) * (endAngle - startAngle))

        onPaint: {
            const context = getContext("2d")
            context.reset()

            // Draw the progress ring
            const gradient = context.createLinearGradient(0, 0, ring.width, 0)
            gradient.addColorStop(0, "black")
            gradient.addColorStop(1, "purple")
            context.fillStyle = gradient
            context.strokeStyle = gradient

            context.lineWidth = ring.borderWidth
            context.beginPath()
            context.clearRect(0, 0, ring.width, ring.height)
            context.arc(ring.centerX, ring.centerY, ring.ringRadius,
                        ring.startAngle, ring.startAngle + ring.step, false)
            context.stroke()

            // Draw the inner border
            const innerBorderRadius = ring.ringRadius - ring.borderWidth / 2
            context.strokeStyle = "#FF49F8"
            context.lineWidth = gauge.innerStrokeWidth

            context.beginPath()
            context.arc(ring.centerX, ring.centerY, innerBorderRadius,
                        ring.startAngle, ring.startAngle + ring.step, false)
            context.stroke()
        }
    }

    Text {
        id: mph
        text: "mph"
        color: "gray"
        font.family: webFont.name
        font.pointSize: gauge.width / 10
        anchors.top: valueText.bottom
        anchors.horizontalCenter: valueText.horizontalCenter
    }
}
