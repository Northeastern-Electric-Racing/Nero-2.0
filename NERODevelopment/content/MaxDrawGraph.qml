import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NERO

Rectangle {
    id: background

    property int dimension: 200

    width: dimension
    height: dimension

    color: "transparent"

    property int value: 130

    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")
            var centerX = dimension / 2
            var centerY = dimension / 2
            var radius = Math.min(dimension, dimension) / 2 - 25
            var startAngle = -Math.PI / 2
            var endAngle = startAngle + (value / 300) * 2 * Math.PI

            ctx.beginPath()
            ctx.arc(centerX, centerY, radius, startAngle, endAngle, false)
            ctx.strokeStyle = Theme.accentGreen
            ctx.lineWidth = 0.06 * background.dimension
            ctx.stroke()
        }
    }

    RowLayout {
        spacing: 0
        anchors.centerIn: parent
        LabelText {
            text: value.toString()
            font.pixelSize: 0.25 * background.dimension
            font.bold: true
            color: Theme.accentGreen
        }

        LabelText {
            text: "A"
            font.pixelSize: 0.15 * background.dimension
            font.bold: true
            color: Theme.accentGreen
            Layout.topMargin: 0.06 * background.dimension
        }
    }

    onValueChanged: canvas.requestPaint()
}
