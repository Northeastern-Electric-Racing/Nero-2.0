import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 200
    height: 200

    property int value: 130

    // Rectangle {
    //     anchors.fill: parent
    //     color: "black"
    // }

    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")
            var centerX = width / 2
            var centerY = height / 2
            var radius = Math.min(width, height) / 2 - 25
            var startAngle = -Math.PI / 2
            var endAngle = startAngle + (value / 300) * 2 * Math.PI

            ctx.beginPath()
            ctx.arc(centerX, centerY, radius, startAngle, endAngle, false)
            ctx.strokeStyle = '#14f804'
            ctx.lineWidth = 12
            ctx.stroke()
        }
    }

    LabelText {
        anchors.centerIn: parent
        textFormat: Text.RichText
        text: "<b><font size='40'>" + value + "</font><font size='5'>A</font></b>"
        horizontalAlignment: Text.AlignHCenter
        color: "white"
        verticalAlignment: Text.AlignVCenter
        }

    onValueChanged: canvas.requestPaint()
}
