import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Rectangle {
    id: background

    property int dimension: 200

    width: dimension
    height: dimension

    color: "transparent"

    property int value: 130

    Canvas {
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
            ctx.strokeStyle = '#14f804'
            ctx.lineWidth = 12 / 200 * background.dimension
            ctx.stroke()
        }
    }

    RowLayout {
        spacing: 0
        anchors.centerIn: parent
        LabelText {
            text: value.toString()
            font.pixelSize: 50 / 200 * background.dimension
            font.bold: true
            color: "white"
        }

        LabelText {
            text: "A"
            font.pixelSize: 30 / 200 * background.dimension
            font.bold: true
            color: "white"
            Layout.topMargin: 12 / 200 * background.dimension
        }
    }


    onValueChanged: canvas.requestPaint()
}
