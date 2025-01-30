import QtQuick 2.11
import QtQuick.Shapes 1.15

Item {
    id: gauge
    property double value: 0
    property double minValue: 0
    property double maxValue: 100
    property int verticalPadding: gauge.height / 10
    property int horizontalPadding: gauge.width / 10
    property int innerStrokeWidth: 3
    property int outerStrokeWidth: 30
    property int mainTextTopPadding: 0
    property int heightOffset: 100
    property string label: "mph"
    property string color: "purple"
    property string unitLabel: ""

    Text {
        id: valueText
        text: Math.round(gauge.value) + unitLabel
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: gauge.width / 5
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
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        antialiasing: true

        property real centerX: ring.width / 2
        property real centerY: ring.height / 2
        property real borderWidth: gauge.outerStrokeWidth
        property real ringRadius: Math.sqrt(
                                      Math.pow(
                                          ring.height - gauge.verticalPadding * 2,
                                          2) + Math.pow(
                                          ring.width - gauge.horizontalPadding * 2,
                                          2)) / 3
        property real pi: Math.PI
        property double startAngle: pi / 2
        property double endAngle: (5 * pi) / 2
        property double step: ((gauge.value - gauge.minValue)
                               / (gauge.maxValue - gauge.minValue) * (endAngle - startAngle))

        Component.onCompleted: {
            console.log('radius: ', ringRadius)
        }

        onPaint: {
            const context = getContext("2d")
            context.reset()

            // Draw the progress ring
            const gradient = context.createLinearGradient(0, 0, ring.width, 0)
            gradient.addColorStop(0, "black")
            gradient.addColorStop(1, gauge.color)
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
            context.strokeStyle = gauge.color
            context.lineWidth = gauge.innerStrokeWidth

            context.beginPath()
            context.arc(ring.centerX, ring.centerY, innerBorderRadius,
                        ring.startAngle, ring.startAngle + ring.step, false)
            context.stroke()
        }
    }

    Text {
        id: mph
        text: gauge.label
        color: "gray"
        font.family: webFont.name
        font.pixelSize: gauge.width / 10
        anchors.top: valueText.bottom
        anchors.horizontalCenter: valueText.horizontalCenter
    }
}
